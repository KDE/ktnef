/*
    SPDX-FileCopyrightText: 2001 Cornelius Schumacher <schumacher@kde.org>
    SPDX-FileCopyrightText: 2004 Reinhold Kainhofer <reinhold@kainhofer.com>
    SPDX-FileCopyrightText: 2005 Rafal Rzepecki <divide@users.sourceforge.net>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
/**
  @file
  This file is part of the API for handling TNEF data and provides
  static Formatter helpers.

  @brief
  Provides helpers too format @acronym TNEF attachments into different
  formats like eg. a HTML representation.

  @author Cornelius Schumacher
  @author Reinhold Kainhofer
  @author Rafal Rzepecki
*/

#include "formatter.h"
using namespace Qt::Literals::StringLiterals;

#include "ktnefdefs.h"
#include "ktnefmessage.h"
#include "ktnefparser.h"

#include <KContacts/PhoneNumber>
#include <KContacts/VCardConverter>

#include <KCalUtils/IncidenceFormatter>
#include <KCalendarCore/Calendar>
#include <KCalendarCore/ICalFormat>

#include <KLocalizedString>

#include <QBuffer>
#include <QTimeZone>

#include <ctime>

using namespace KCalendarCore;
using namespace KTnef;

/*******************************************************************
 *  Helper functions for the msTNEF -> VPart converter
 *******************************************************************/

//-----------------------------------------------------------------------------
//@cond IGNORE
static QString stringProp(KTNEFMessage *tnefMsg, quint32 key, const QString &fallback = QString())
{
    return tnefMsg->findProp(key < 0x10000 ? key & 0xFFFF : key >> 16, fallback);
}

static QString sNamedProp(KTNEFMessage *tnefMsg, const QString &name, const QString &fallback = QString())
{
    return tnefMsg->findNamedProp(name, fallback);
}

static QDateTime pureISOToLocalQDateTime(const QString &dtStr)
{
    const QStringView dtView{dtStr};
    const int year = dtView.left(4).toInt();
    const int month = dtView.mid(4, 2).toInt();
    const int day = dtView.mid(6, 2).toInt();
    const int hour = dtView.mid(9, 2).toInt();
    const int minute = dtView.mid(11, 2).toInt();
    const int second = dtView.mid(13, 2).toInt();
    QDate tmpDate;
    tmpDate.setDate(year, month, day);
    QTime tmpTime;
    tmpTime.setHMS(hour, minute, second);

    if (tmpDate.isValid() && tmpTime.isValid()) {
        QDateTime dT = QDateTime(tmpDate, tmpTime);

        // correct for GMT ( == Zulu time == UTC )
        if (dtStr.at(dtStr.length() - 1) == u'Z') {
            // dT = dT.addSecs( 60 * KRFCDate::localUTCOffset() );
            // localUTCOffset( dT ) );
            dT = dT.toLocalTime();
        }
        return dT;
    } else {
        return {};
    }
}
//@endcond

QString KTnef::msTNEFToVPart(const QByteArray &tnef)
{
    KTNEFParser parser;
    QByteArray b(tnef);
    QBuffer buf(&b);
    MemoryCalendar::Ptr cal(new MemoryCalendar(QTimeZone::utc()));
    KContacts::Addressee addressee;
    ICalFormat calFormat;
    Event::Ptr event(new Event());

    if (parser.openDevice(&buf)) {
        KTNEFMessage *tnefMsg = parser.message();
        // QMap<int,KTNEFProperty*> props = parser.message()->properties();

        // Everything depends from property PR_MESSAGE_CLASS
        // (this is added by KTNEFParser):
        QString msgClass = tnefMsg->findProp(0x001A, QString(), true).toUpper();
        if (!msgClass.isEmpty()) {
            // Match the old class names that might be used by Outlook for
            // compatibility with Microsoft Mail for Windows for Workgroups 3.1.
            bool bCompatClassAppointment = false;
            bool bCompatMethodRequest = false;
            bool bCompatMethodCancled = false;
            bool bCompatMethodAccepted = false;
            bool bCompatMethodAcceptedCond = false;
            bool bCompatMethodDeclined = false;
            if (msgClass.startsWith("IPM.MICROSOFT SCHEDULE."_L1)) {
                bCompatClassAppointment = true;
                if (msgClass.endsWith(".MTGREQ"_L1)) {
                    bCompatMethodRequest = true;
                } else if (msgClass.endsWith(".MTGCNCL"_L1)) {
                    bCompatMethodCancled = true;
                } else if (msgClass.endsWith(".MTGRESPP"_L1)) {
                    bCompatMethodAccepted = true;
                } else if (msgClass.endsWith(".MTGRESPA"_L1)) {
                    bCompatMethodAcceptedCond = true;
                } else if (msgClass.endsWith(".MTGRESPN"_L1)) {
                    bCompatMethodDeclined = true;
                }
            }
            bool bCompatClassNote = (msgClass == "IPM.MICROSOFT MAIL.NOTE"_L1);

            if (bCompatClassAppointment || "IPM.APPOINTMENT"_L1 == msgClass) {
                // Compose a vCal
                bool bIsReply = false;
                QString prodID = u"-//Microsoft Corporation//Outlook "_s;
                prodID += tnefMsg->findNamedProp(u"0x8554"_s, u"9.0"_s);
                prodID += "MIMEDIR/EN\n"_L1;
                prodID += "VERSION:2.0\n"_L1;
                calFormat.setApplication(u"Outlook"_s, prodID);

                // iTIPMethod method;
                if (bCompatMethodRequest) {
                    // method = iTIPRequest;
                } else if (bCompatMethodCancled) {
                    // method = iTIPCancel;
                } else if (bCompatMethodAccepted || bCompatMethodAcceptedCond || bCompatMethodDeclined) {
                    // method = iTIPReply;
                    bIsReply = true;
                } else {
                    // pending(khz): verify whether "0x0c17" is the right tag ???
                    //
                    // at the moment we think there are REQUESTS and UPDATES
                    //
                    // but WHAT ABOUT REPLIES ???
                    //
                    //

                    if (tnefMsg->findProp(0x0c17) == u'1') {
                        bIsReply = true;
                    }
                    // method = iTIPRequest;
                }

                /// ###  FIXME Need to get this attribute written
                // ScheduleMessage schedMsg( event, method, ScheduleMessage::Unknown );

                QString sSenderSearchKeyEmail(tnefMsg->findProp(0x0C1D));
                if (sSenderSearchKeyEmail.isEmpty()) {
                    sSenderSearchKeyEmail = tnefMsg->findProp(0x0C1f);
                }

                if (!sSenderSearchKeyEmail.isEmpty()) {
                    const int colon = sSenderSearchKeyEmail.indexOf(u':');
                    // May be e.g. "SMTP:KHZ@KDE.ORG"
                    if (colon == -1) {
                        sSenderSearchKeyEmail.remove(0, colon + 1);
                    }
                }

                QString s(tnefMsg->findProp(0x8189));
                const QStringList attendees = s.split(u';');
                if (!attendees.isEmpty()) {
                    for (auto it = attendees.cbegin(), end = attendees.cend(); it != end; ++it) {
                        // Skip all entries that have no '@' since these are
                        // no mail addresses
                        if (!(*it).contains(u'@')) {
                            s = (*it).trimmed();

                            Attendee attendee(s, s, true);
                            if (bIsReply) {
                                if (bCompatMethodAccepted) {
                                    attendee.setStatus(Attendee::Accepted);
                                }
                                if (bCompatMethodDeclined) {
                                    attendee.setStatus(Attendee::Declined);
                                }
                                if (bCompatMethodAcceptedCond) {
                                    attendee.setStatus(Attendee::Tentative);
                                }
                            } else {
                                attendee.setStatus(Attendee::NeedsAction);
                                attendee.setRole(Attendee::ReqParticipant);
                            }
                            event->addAttendee(attendee);
                        }
                    }
                } else {
                    // Oops, no attendees?
                    // This must be old style, let us use the PR_SENDER_SEARCH_KEY.
                    s = sSenderSearchKeyEmail;
                    if (!s.isEmpty()) {
                        Attendee attendee(QString(), QString(), true);
                        if (bIsReply) {
                            if (bCompatMethodAccepted) {
                                attendee.setStatus(Attendee::Accepted);
                            }
                            if (bCompatMethodAcceptedCond) {
                                attendee.setStatus(Attendee::Declined);
                            }
                            if (bCompatMethodDeclined) {
                                attendee.setStatus(Attendee::Tentative);
                            }
                        } else {
                            attendee.setStatus(Attendee::NeedsAction);
                            attendee.setRole(Attendee::ReqParticipant);
                        }
                        event->addAttendee(attendee);
                    }
                }
                s = tnefMsg->findProp(0x3ff8); // look for organizer property
                if (s.isEmpty() && !bIsReply) {
                    s = sSenderSearchKeyEmail;
                }
                // TODO: Use the common name?
                if (!s.isEmpty()) {
                    event->setOrganizer(s);
                }

                QDateTime dt = tnefMsg->property(0x819b).toDateTime();
                if (!dt.isValid()) {
                    dt = tnefMsg->property(0x0060).toDateTime();
                }
                event->setDtStart(dt); // ## Format??

                dt = tnefMsg->property(0x819c).toDateTime();
                if (!dt.isValid()) {
                    dt = tnefMsg->property(0x0061).toDateTime();
                }
                event->setDtEnd(dt);

                s = tnefMsg->findProp(0x810d);
                event->setLocation(s);
                // is it OK to set this to OPAQUE always ??
                // vPart += "TRANSP:OPAQUE\n"; ###FIXME, portme!
                // vPart += "SEQUENCE:0\n";

                // is "0x0023" OK  -  or should we look for "0x0003" ??
                s = tnefMsg->findProp(0x0062);
                event->setUid(s);

                // PENDING(khz): is this value in local timezone? Must it be
                // adjusted? Most likely this is a bug in the server or in
                // Outlook - we ignore it for now.
                s = tnefMsg->findProp(0x8202).remove(u'-').remove(u':');
                // ### kcal always uses currentDateTime()
                // event->setDtStamp( QDateTime::fromString( s ) );

                s = tnefMsg->findNamedProp(u"Keywords"_s);
                event->setCategories(s);

                s = tnefMsg->findProp(0x1000);
                if (s.isEmpty()) {
                    s = tnefMsg->findProp(0x3fd9);
                }
                event->setDescription(s);

                s = tnefMsg->findProp(0x0070);
                if (s.isEmpty()) {
                    s = tnefMsg->findProp(0x0037);
                }
                event->setSummary(s);

                s = tnefMsg->findProp(0x0026);
                event->setPriority(s.toInt());
                // is reminder flag set ?
                if (!tnefMsg->findProp(0x8503).isEmpty()) {
                    Alarm::Ptr alarm(new Alarm(event.data())); // TODO: fix when KCalendarCore::Alarm is fixed
                    QDateTime highNoonTime = pureISOToLocalQDateTime(tnefMsg->findProp(0x8502).remove(u'-').remove(u':'));
                    QDateTime wakeMeUpTime = pureISOToLocalQDateTime(tnefMsg->findProp(0x8560, QString()).remove(u'-').remove(u':'));
                    alarm->setTime(wakeMeUpTime);

                    if (highNoonTime.isValid() && wakeMeUpTime.isValid()) {
                        alarm->setStartOffset(Duration(highNoonTime, wakeMeUpTime));
                    } else {
                        // default: wake them up 15 minutes before the appointment
                        alarm->setStartOffset(Duration(15 * 60));
                    }
                    alarm->setDisplayAlarm(i18n("Reminder"));

                    // Sorry: the different action types are not known (yet)
                    //        so we always set 'DISPLAY' (no sounds, no images...)
                    event->addAlarm(alarm);
                }
                // ensure we have a uid for this event
                if (event->uid().isEmpty()) {
                    event->setUid(CalFormat::createUniqueId());
                }
                cal->addEvent(event);
                // bOk = true;
                // we finished composing a vCal
            } else if (bCompatClassNote || "IPM.CONTACT"_L1 == msgClass) {
                addressee.setUid(stringProp(tnefMsg, attMSGID));
                addressee.setFormattedName(stringProp(tnefMsg, MAPI_TAG_PR_DISPLAY_NAME));
                KContacts::Email email(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_EMAIL1EMAILADDRESS)));
                email.setPreferred(true);
                addressee.addEmail(email);
                addressee.addEmail(KContacts::Email(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_EMAIL2EMAILADDRESS))));
                addressee.addEmail(KContacts::Email(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_EMAIL3EMAILADDRESS))));
                addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-IMAddress"_s, sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_IMADDRESS)));
                addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-SpousesName"_s, stringProp(tnefMsg, MAPI_TAG_PR_SPOUSE_NAME));
                addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-ManagersName"_s, stringProp(tnefMsg, MAPI_TAG_PR_MANAGER_NAME));
                addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-AssistantsName"_s, stringProp(tnefMsg, MAPI_TAG_PR_ASSISTANT));
                addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-Department"_s, stringProp(tnefMsg, MAPI_TAG_PR_DEPARTMENT_NAME));
                addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-Office"_s, stringProp(tnefMsg, MAPI_TAG_PR_OFFICE_LOCATION));
                addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-Profession"_s, stringProp(tnefMsg, MAPI_TAG_PR_PROFESSION));

                QString s = tnefMsg->findProp(MAPI_TAG_PR_WEDDING_ANNIVERSARY).remove(u'-').remove(u':');
                if (!s.isEmpty()) {
                    addressee.insertCustom(u"KADDRESSBOOK"_s, u"X-Anniversary"_s, s);
                }

                KContacts::ResourceLocatorUrl url;
                url.setUrl(QUrl(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_WEBPAGE))));

                addressee.setUrl(url);

                // collect parts of Name entry
                addressee.setFamilyName(stringProp(tnefMsg, MAPI_TAG_PR_SURNAME));
                addressee.setGivenName(stringProp(tnefMsg, MAPI_TAG_PR_GIVEN_NAME));
                addressee.setAdditionalName(stringProp(tnefMsg, MAPI_TAG_PR_MIDDLE_NAME));
                addressee.setPrefix(stringProp(tnefMsg, MAPI_TAG_PR_DISPLAY_NAME_PREFIX));
                addressee.setSuffix(stringProp(tnefMsg, MAPI_TAG_PR_GENERATION));

                addressee.setNickName(stringProp(tnefMsg, MAPI_TAG_PR_NICKNAME));
                addressee.setRole(stringProp(tnefMsg, MAPI_TAG_PR_TITLE));
                addressee.setOrganization(stringProp(tnefMsg, MAPI_TAG_PR_COMPANY_NAME));
                /*
                the MAPI property ID of this (multiline) )field is unknown:
                vPart += stringProp(tnefMsg, "\n","NOTE", ... , "" );
                */

                KContacts::Address adr;
                adr.setPostOfficeBox(stringProp(tnefMsg, MAPI_TAG_PR_HOME_ADDRESS_PO_BOX));
                adr.setStreet(stringProp(tnefMsg, MAPI_TAG_PR_HOME_ADDRESS_STREET));
                adr.setLocality(stringProp(tnefMsg, MAPI_TAG_PR_HOME_ADDRESS_CITY));
                adr.setRegion(stringProp(tnefMsg, MAPI_TAG_PR_HOME_ADDRESS_STATE_OR_PROVINCE));
                adr.setPostalCode(stringProp(tnefMsg, MAPI_TAG_PR_HOME_ADDRESS_POSTAL_CODE));
                adr.setCountry(stringProp(tnefMsg, MAPI_TAG_PR_HOME_ADDRESS_COUNTRY));
                adr.setType(KContacts::Address::Home);
                addressee.insertAddress(adr);

                adr.setPostOfficeBox(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_BUSINESSADDRESSPOBOX)));
                adr.setStreet(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_BUSINESSADDRESSSTREET)));
                adr.setLocality(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_BUSINESSADDRESSCITY)));
                adr.setRegion(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_BUSINESSADDRESSSTATE)));
                adr.setPostalCode(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_BUSINESSADDRESSPOSTALCODE)));
                adr.setCountry(sNamedProp(tnefMsg, QStringLiteral(MAPI_TAG_CONTACT_BUSINESSADDRESSCOUNTRY)));
                adr.setType(KContacts::Address::Work);
                addressee.insertAddress(adr);

                adr.setPostOfficeBox(stringProp(tnefMsg, MAPI_TAG_PR_OTHER_ADDRESS_PO_BOX));
                adr.setStreet(stringProp(tnefMsg, MAPI_TAG_PR_OTHER_ADDRESS_STREET));
                adr.setLocality(stringProp(tnefMsg, MAPI_TAG_PR_OTHER_ADDRESS_CITY));
                adr.setRegion(stringProp(tnefMsg, MAPI_TAG_PR_OTHER_ADDRESS_STATE_OR_PROVINCE));
                adr.setPostalCode(stringProp(tnefMsg, MAPI_TAG_PR_OTHER_ADDRESS_POSTAL_CODE));
                adr.setCountry(stringProp(tnefMsg, MAPI_TAG_PR_OTHER_ADDRESS_COUNTRY));
                adr.setType(KContacts::Address::Dom);
                addressee.insertAddress(adr);

                // problem: the 'other' address was stored by KOrganizer in
                //          a line looking like the following one:
                // vPart += "\nADR;TYPE=dom;TYPE=intl;TYPE=parcel;TYPE=postal;TYPE=work;"
                //          "TYPE=home:other_pobox;;other_str1\nother_str2;other_loc;other_region;"
                //          "other_pocode;other_country"

                QString nr;
                nr = stringProp(tnefMsg, MAPI_TAG_PR_HOME_TELEPHONE_NUMBER);
                addressee.insertPhoneNumber(KContacts::PhoneNumber(nr, KContacts::PhoneNumber::Home));
                nr = stringProp(tnefMsg, MAPI_TAG_PR_BUSINESS_TELEPHONE_NUMBER);
                addressee.insertPhoneNumber(KContacts::PhoneNumber(nr, KContacts::PhoneNumber::Work));
                nr = stringProp(tnefMsg, MAPI_TAG_PR_MOBILE_TELEPHONE_NUMBER);
                addressee.insertPhoneNumber(KContacts::PhoneNumber(nr, KContacts::PhoneNumber::Cell));
                nr = stringProp(tnefMsg, MAPI_TAG_PR_HOME_FAX_NUMBER);
                addressee.insertPhoneNumber(KContacts::PhoneNumber(nr, KContacts::PhoneNumber::Fax | KContacts::PhoneNumber::Home));
                nr = stringProp(tnefMsg, MAPI_TAG_PR_BUSINESS_FAX_NUMBER);
                addressee.insertPhoneNumber(KContacts::PhoneNumber(nr, KContacts::PhoneNumber::Fax | KContacts::PhoneNumber::Work));

                s = tnefMsg->findProp(MAPI_TAG_PR_BIRTHDAY).remove(u'-').remove(u':');
                if (!s.isEmpty()) {
                    addressee.setBirthday(QDateTime::fromString(s));
                }

                // bOk = (!addressee.isEmpty());
            } else if ("IPM.NOTE"_L1 == msgClass) {
            } // else if ... and so on ...
        }
    }

    // Compose return string
    const QString iCal = calFormat.toString(cal);
    if (!iCal.isEmpty()) {
        // This was an iCal
        return iCal;
    }

    // Not an iCal - try a vCard
    KContacts::VCardConverter converter;
    return QString::fromUtf8(converter.createVCard(addressee));
}

QString KTnef::formatTNEFInvitation(const QByteArray &tnef, const MemoryCalendar::Ptr &cal, KCalUtils::InvitationFormatterHelper *h)
{
    const QString vPart = msTNEFToVPart(tnef);
    QString iCal = KCalUtils::IncidenceFormatter::formatICalInvitation(vPart, cal, h);
    if (!iCal.isEmpty()) {
        return iCal;
    } else {
        return vPart;
    }
}
