/*
    ktnefwriter.cpp

    SPDX-FileCopyrightText: 2002 Bo Thorsen <bo@sonofthor.dk>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
 * @file
 * This file is part of the API for handling TNEF data and
 * defines the KTNEFWriter class.
 *
 * @author Bo Thorsen
 */

#include "ktnefwriter.h"
using namespace Qt::Literals::StringLiterals;

#include "ktnefdefs.h"
#include "ktnefproperty.h"
#include "ktnefpropertyset.h"

#include "ktnef_debug.h"

#include <QByteArray>
#include <QDataStream>
#include <QDateTime>
#include <QIODevice>
#include <QList>

#include <cassert>

using namespace KTnef;

/**
 * KTNEFWriterPrivate class that helps to provide binary compatibility between releases.
 * @internal
 */
//@cond PRIVATE
class KTnef::KTNEFWriterPrivateData
{
public:
    KTNEFWriterPrivateData()
        : mFirstAttachNum(QDateTime::currentDateTimeUtc().toSecsSinceEpoch())
    {
    }

    KTNEFPropertySet properties;
    const quint16 mFirstAttachNum;
};
//@endcond

KTNEFWriter::KTNEFWriter()
    : d(new KTnef::KTNEFWriterPrivateData)
{
    // This is not something the user should fiddle with
    // First set the TNEF version
    QVariant v(0x00010000);
    addProperty(attTNEFVERSION, atpDWORD, v);

    // Now set the code page to something reasonable. TODO: Use the right one
    QVariant v1((quint32)0x4e4);
    QVariant v2((quint32)0x0);
    QList<QVariant> list;
    list << v1;
    list << v2;
    v = QVariant(list);
    addProperty(attOEMCODEPAGE, atpBYTE, list);
}

KTNEFWriter::~KTNEFWriter() = default;

void KTNEFWriter::addProperty(int tag, int type, const QVariant &value)
{
    d->properties.addProperty(tag, type, value);
}

//@cond IGNORE
void addToChecksum(quint32 i, quint16 &checksum)
{
    checksum += i & 0xff;
    checksum += (i >> 8) & 0xff;
    checksum += (i >> 16) & 0xff;
    checksum += (i >> 24) & 0xff;
}

void addToChecksum(QByteArray &cs, quint16 &checksum)
{
    int len = cs.length();
    for (int i = 0; i < len; i++) {
        checksum += (quint8)cs[i];
    }
}

void writeCString(QDataStream &stream, QByteArray &str)
{
    stream.writeRawData(str.data(), str.length());
    stream << (quint8)0;
}

quint32 mergeTagAndType(quint32 tag, quint32 type)
{
    return ((type & 0xffff) << 16) | (tag & 0xffff);
}
//@endcond

/* This writes a TNEF property to the file.
 *
 * A TNEF property has a 1 byte type (LVL_MESSAGE or LVL_ATTACHMENT),
 * a 4 byte type/tag, a 4 byte length, the data and finally the checksum.
 *
 * The checksum is a 16 byte int with all bytes in the data added.
 */
bool KTNEFWriter::writeProperty(QDataStream &stream, int &bytes, int tag) const
{
    QMap<int, KTNEFProperty *> &properties = d->properties.properties();
    QMap<int, KTNEFProperty *>::Iterator it = properties.find(tag);

    if (it == properties.end()) {
        return false;
    }

    KTNEFProperty *property = *it;

    quint32 i;
    quint16 checksum = 0;
    QList<QVariant> list;
    QByteArray cs;
    QByteArray cs2;
    QDateTime dt;
    QDate date;
    QTime time;
    switch (tag) {
    case attMSGSTATUS:
        // quint8
        i = property->value().toUInt() & 0xff;
        checksum = i;

        stream << (quint8)LVL_MESSAGE;
        stream << mergeTagAndType(tag, property->type());
        stream << (quint32)1;
        stream << (quint8)i;

        bytes += 10;
        break;

    case attMSGPRIORITY:
    case attREQUESTRES:
        // quint16
        i = property->value().toUInt() & 0xffff;
        addToChecksum(i, checksum);

        stream << (quint8)LVL_MESSAGE;
        stream << mergeTagAndType(tag, property->type());
        stream << (quint32)2;
        stream << (quint16)i;

        bytes += 11;
        break;

    case attTNEFVERSION:
        // quint32
        i = property->value().toUInt();
        addToChecksum(i, checksum);

        stream << (quint8)LVL_MESSAGE;
        stream << mergeTagAndType(tag, property->type());
        stream << (quint32)4;
        stream << (quint32)i;

        bytes += 13;
        break;

    case attOEMCODEPAGE:
        // 2 quint32
        list = property->value().toList();
        assert(list.count() == 2);

        stream << (quint8)LVL_MESSAGE;
        stream << mergeTagAndType(tag, property->type());
        stream << (quint32)8;

        i = list[0].toInt();
        addToChecksum(i, checksum);
        stream << (quint32)i;
        i = list[1].toInt();
        addToChecksum(i, checksum);
        stream << (quint32)i;

        bytes += 17;
        break;

    case attMSGCLASS:
    case attSUBJECT:
    case attBODY:
    case attMSGID:
        // QCString
        cs = property->value().toString().toLocal8Bit();
        addToChecksum(cs, checksum);

        stream << (quint8)LVL_MESSAGE;
        stream << mergeTagAndType(tag, property->type());
        stream << (quint32)cs.length() + 1;
        writeCString(stream, cs);

        bytes += 9 + cs.length() + 1;
        break;

    case attFROM:
        // 2 QString encoded to a TRP structure
        list = property->value().toList();
        assert(list.count() == 2);

        cs = list[0].toString().toLocal8Bit(); // Name
        cs2 = QString("smtp:"_L1 + list[1].toString()).toLocal8Bit(); // Email address
        i = 18 + cs.length() + cs2.length(); // 2 * sizof(TRP) + strings + 2x'\0'

        stream << (quint8)LVL_MESSAGE;
        stream << mergeTagAndType(tag, property->type());
        stream << (quint32)i;

        // The stream has to be aligned to 4 bytes for the strings
        // TODO: Or does it? Looks like Outlook doesn't do this
        // bytes += 17;
        // Write the first TRP structure
        stream << (quint16)4; // trpidOneOff
        stream << (quint16)i; // totalsize
        stream << (quint16)(cs.length() + 1); // sizeof name
        stream << (quint16)(cs2.length() + 1); // sizeof address

        // if ( bytes % 4 != 0 )
        // Align the buffer

        // Write the strings
        writeCString(stream, cs);
        writeCString(stream, cs2);

        // Write the empty padding TRP structure (just zeroes)
        stream << (quint32)0 << (quint32)0;

        addToChecksum(4, checksum);
        addToChecksum(i, checksum);
        addToChecksum(cs.length() + 1, checksum);
        addToChecksum(cs2.length() + 1, checksum);
        addToChecksum(cs, checksum);
        addToChecksum(cs2, checksum);

        bytes += 10;
        break;

    case attDATESENT:
    case attDATERECD:
    case attDATEMODIFIED:
        // QDateTime
        dt = property->value().toDateTime();
        time = dt.time();
        date = dt.date();

        stream << (quint8)LVL_MESSAGE;
        stream << mergeTagAndType(tag, property->type());
        stream << (quint32)14;

        i = (quint16)date.year();
        addToChecksum(i, checksum);
        stream << (quint16)i;
        i = (quint16)date.month();
        addToChecksum(i, checksum);
        stream << (quint16)i;
        i = (quint16)date.day();
        addToChecksum(i, checksum);
        stream << (quint16)i;
        i = (quint16)time.hour();
        addToChecksum(i, checksum);
        stream << (quint16)i;
        i = (quint16)time.minute();
        addToChecksum(i, checksum);
        stream << (quint16)i;
        i = (quint16)time.second();
        addToChecksum(i, checksum);
        stream << (quint16)i;
        i = (quint16)date.dayOfWeek();
        addToChecksum(i, checksum);
        stream << (quint16)i;
        break;
        /*
        case attMSGSTATUS:
        {
        quint8 c;
        quint32 flag = 0;
        if ( c & fmsRead ) flag |= MSGFLAG_READ;
        if ( !( c & fmsModified ) ) flag |= MSGFLAG_UNMODIFIED;
        if ( c & fmsSubmitted ) flag |= MSGFLAG_SUBMIT;
        if ( c & fmsHasAttach ) flag |= MSGFLAG_HASATTACH;
        if ( c & fmsLocal ) flag |= MSGFLAG_UNSENT;
        d->stream_ >> c;

        i = property->value().toUInt();
        stream << (quint8)LVL_MESSAGE;
        stream << (quint32)type;
        stream << (quint32)2;
        stream << (quint8)i;
        addToChecksum( i, checksum );
        // from reader: d->message_->addProperty( 0x0E07, MAPI_TYPE_ULONG, flag );
        }
        qCDebug(KTNEF_LOG) << "Message Status" << "(length=" << i2 << ")";
        break;
        */

    default:
        qCDebug(KTNEF_LOG) << "Unknown TNEF tag:" << tag;
        return false;
    }

    stream << (quint16)checksum;
    return true;
}

bool KTNEFWriter::writeFile(QIODevice &file) const
{
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream stream(&file);
    return writeFile(stream);
}

bool KTNEFWriter::writeFile(QDataStream &stream) const
{
    stream.setByteOrder(QDataStream::LittleEndian);

    // Start by writing the opening TNEF stuff
    stream << TNEF_SIGNATURE;

    // Store the PR_ATTACH_NUM value for the first attachment
    // ( must be stored even if *no* attachments are stored )
    stream << d->mFirstAttachNum;

    // Now do some writing
    bool ok = true;
    int bytesWritten = 0;
    ok &= writeProperty(stream, bytesWritten, attTNEFVERSION);
    ok &= writeProperty(stream, bytesWritten, attOEMCODEPAGE);
    ok &= writeProperty(stream, bytesWritten, attMSGCLASS);
    ok &= writeProperty(stream, bytesWritten, attMSGPRIORITY);
    ok &= writeProperty(stream, bytesWritten, attSUBJECT);
    ok &= writeProperty(stream, bytesWritten, attDATESENT);
    ok &= writeProperty(stream, bytesWritten, attDATESTART);
    ok &= writeProperty(stream, bytesWritten, attDATEEND);
    // ok &= writeProperty( stream, bytesWritten, attAIDOWNER );
    ok &= writeProperty(stream, bytesWritten, attREQUESTRES);
    ok &= writeProperty(stream, bytesWritten, attFROM);
    ok &= writeProperty(stream, bytesWritten, attDATERECD);
    ok &= writeProperty(stream, bytesWritten, attMSGSTATUS);
    ok &= writeProperty(stream, bytesWritten, attBODY);
    return ok;
}

void KTNEFWriter::setSender(const QString &name, const QString &email)
{
    assert(!name.isEmpty());
    assert(!email.isEmpty());

    QVariant v1(name);
    QVariant v2(email);

    const QList<QVariant> list = {v1, v2};

    addProperty(attFROM, 0, list); // What's up with the 0 here ??
}

void KTNEFWriter::setMessageType(MessageType m)
{
    // Note that the MessageType list here is probably not long enough,
    // more entries are most likely needed later

    QVariant v;
    switch (m) {
    case Appointment:
        v = QVariant("IPM.Appointment"_L1);
        break;

    case MeetingCancelled:
        v = QVariant("IPM.Schedule.Meeting.Cancelled"_L1);
        break;

    case MeetingRequest:
        v = QVariant("IPM.Schedule.Meeting.Request"_L1);
        break;

    case MeetingNo:
        v = QVariant("IPM.Schedule.Meeting.Resp.Neg"_L1);
        break;

    case MeetingYes:
        v = QVariant("IPM.Schedule.Meeting.Resp.Pos"_L1);
        break;

    case MeetingTent:
        // Tent?
        v = QVariant("IPM.Schedule.Meeting.Resp.Tent"_L1);
        break;

    default:
        return;
    }

    addProperty(attMSGCLASS, atpWORD, v);
}

void KTNEFWriter::setMethod(Method)
{
}

void KTNEFWriter::clearAttendees()
{
}

void KTNEFWriter::addAttendee(const QString &cn, Role r, PartStat p, bool rsvp, const QString &mailto)
{
    Q_UNUSED(cn)
    Q_UNUSED(r)
    Q_UNUSED(p)
    Q_UNUSED(rsvp)
    Q_UNUSED(mailto)
}

// I assume this is the same as the sender?
// U also assume that this is like "Name <address>"
void KTNEFWriter::setOrganizer(const QString &organizer)
{
    int i = organizer.indexOf(u'<');

    if (i == -1) {
        return;
    }

    QString name = organizer.left(i).trimmed();

    QString email = organizer.right(i + 1);
    email = email.left(email.length() - 1).trimmed();

    setSender(name, email);
}

void KTNEFWriter::setDtStart(const QDateTime &dtStart)
{
    QVariant v(dtStart);
    addProperty(attDATESTART, atpDATE, v);
}

void KTNEFWriter::setDtEnd(const QDateTime &dtEnd)
{
    QVariant v(dtEnd);
    addProperty(attDATEEND, atpDATE, v);
}

void KTNEFWriter::setLocation(const QString & /*location*/)
{
}

void KTNEFWriter::setUID(const QString &uid)
{
    QVariant v(uid);
    addProperty(attMSGID, atpSTRING, v);
}

// Date sent
void KTNEFWriter::setDtStamp(const QDateTime &dtStamp)
{
    QVariant v(dtStamp);
    addProperty(attDATESENT, atpDATE, v);
}

void KTNEFWriter::setCategories(const QStringList &)
{
}

// I hope this is the body
void KTNEFWriter::setDescription(const QString &body)
{
    QVariant v(body);
    addProperty(attBODY, atpTEXT, v);
}

void KTNEFWriter::setSummary(const QString &s)
{
    QVariant v(s);
    addProperty(attSUBJECT, atpSTRING, v);
}

// TNEF encoding: Normal =  3, high = 2, low = 1
// MAPI encoding: Normal = -1, high = 0, low = 1
void KTNEFWriter::setPriority(Priority p)
{
    QVariant v((quint32)p);
    addProperty(attMSGPRIORITY, atpSHORT, v);
}

void KTNEFWriter::setAlarm(const QString &description, AlarmAction action, const QDateTime &wakeBefore)
{
    Q_UNUSED(description)
    Q_UNUSED(action)
    Q_UNUSED(wakeBefore)
}
