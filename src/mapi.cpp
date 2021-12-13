/*
    mapi.cpp

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
 * @file
 * This file is part of the API for handling TNEF data and
 * provides functions that convert MAPI keycodes to/from tag strings.
 *
 * @author Michael Goffioul
 */

#include "mapi.h"
#include <KLocalizedString>
#include <QMap>
//@cond IGNORE
#include <KLazyLocalizedString>
static const struct {
    int tag;
    const KLazyLocalizedString str;
} MAPI_TagStrings[] = {{0x0002, kli18n("Alternate Recipient Allowed")},
                       {0x001A, kli18n("Message Class")},
                       {0x0023, kli18n("Originator Delivery Report Requested")},
                       {0x0024, kli18n("Originator Return Address")},
                       {0x0026, kli18n("Priority")},
                       {0x0029, kli18n("Read Receipt Requested")},
                       {0x002B, kli18n("Recipient Reassignment Prohibited")},
                       {0x002E, kli18n("Original Sensitivity")},
                       {0x0031, kli18n("Report Tag")},
                       {0x0036, kli18n("Sensitivity")},
                       {0x0037, kli18n("Subject")},
                       {0x0039, kli18n("Client Submit Time")},
                       {0x003B, kli18n("Sent Representing Search Key")},
                       {0x003D, kli18n("Subject Prefix")},
                       {0x0041, kli18n("Sent Representing Entry ID")},
                       {0x0042, kli18n("Sent Representing Name")},
                       {0x0047, kli18n("Message Submission ID")},
                       {0x004D, kli18n("Original Author Name")},
                       {0x0062, kli18n("Owner Appointment ID")},
                       {0x0063, kli18n("Response Requested")},
                       {0x0064, kli18n("Sent Representing Address Type")},
                       {0x0065, kli18n("Sent Representing E-mail Address")},
                       {0x0070, kli18n("Conversation Topic")},
                       {0x0071, kli18n("Conversation Index")},
                       {0x007F, kli18n("TNEF Correlation Key")},
                       {0x0C17, kli18n("Reply Requested")},
                       {0x0C1A, kli18n("Sender Name")},
                       {0x0C1D, kli18n("Sender Search Key")},
                       {0x0C1E, kli18n("Sender Address Type")},
                       {0x0C1F, kli18n("Sender E-mail Address")},
                       {0x0E01, kli18n("Delete After Submit")},
                       {0x0E02, kli18n("Display Bcc")},
                       {0x0E03, kli18n("Display Cc")},
                       {0x0E04, kli18n("Display To")},
                       {0x0E06, kli18n("Message Delivery Time")},
                       {0x0E07, kli18n("Message Flags")},
                       {0x0E08, kli18n("Message Size")},
                       {0x0E09, kli18n("Parent Entry ID")},
                       {0x0E0A, kli18n("Sent-Mail Entry ID")},
                       {0x0E12, kli18n("Message Recipients")},
                       {0x0E14, kli18n("Submit Flags")},
                       {0x0E1B, kli18n("Has Attachment")},
                       {0x0E1D, kli18n("Normalized Subject")},
                       {0x0E1F, kli18n("RTF In Sync")},
                       {0x0E20, kli18n("Attachment Size")},
                       {0x0E21, kli18n("Attachment Number")},
                       {0x0FF4, kli18n("Access")},
                       {0x0FF7, kli18n("Access Level")},
                       {0x0FF8, kli18n("Mapping Signature")},
                       {0x0FF9, kli18n("Record Key")},
                       {0x0FFA, kli18n("Store Record Key")},
                       {0x0FFB, kli18n("Store Entry ID")},
                       {0x0FFE, kli18n("Object Type")},
                       {0x0FFF, kli18n("Entry ID")},
                       {0x1000, kli18n("Message Body")},
                       {0x1006, kli18n("RTF Sync Body CRC")},
                       {0x1007, kli18n("RTF Sync Body Count")},
                       {0x1008, kli18n("RTF Sync Body Tag")},
                       {0x1009, kli18n("RTF Compressed")},
                       {0x1010, kli18n("RTF Sync Prefix Count")},
                       {0x1011, kli18n("RTF Sync Trailing Count")},
                       {0x1013, kli18n("HTML Message Body")},
                       {0x1035, kli18n("Message ID")},
                       {0x1042, kli18n("Parent's Message ID")},
                       {0x1080, kli18n("Action")},
                       {0x1081, kli18n("Action Flag")},
                       {0x1082, kli18n("Action Date")},
                       {0x3001, kli18n("Display Name")},
                       {0x3007, kli18n("Creation Time")},
                       {0x3008, kli18n("Last Modification Time")},
                       {0x300B, kli18n("Search Key")},
                       {0x340D, kli18n("Store Support Mask")},
                       {0x3414, kli18n("MDB Provider")},
                       {0x3701, kli18n("Attachment Data")},
                       {0x3702, kli18n("Attachment Encoding")},
                       {0x3703, kli18n("Attachment Extension")},
                       {0x3705, kli18n("Attachment Method")},
                       {0x3707, kli18n("Attachment Long File Name")},
                       {0x370B, kli18n("Attachment Rendering Position")},
                       {0x370E, kli18n("Attachment Mime Tag")},
                       {0x3714, kli18n("Attachment Flags")},
                       {0x3A00, kli18n("Account")},
                       {0x3A05, kli18n("Generation")},
                       {0x3A06, kli18n("Given Name")},
                       {0x3A0A, kli18n("Initials")},
                       {0x3A0B, kli18n("Keyword")},
                       {0x3A0C, kli18n("Language")},
                       {0x3A0D, kli18n("Location")},
                       {0x3A11, kli18n("Surname")},
                       {0x3A16, kli18n("Company Name")},
                       {0x3A17, kli18n("Title")},
                       {0x3A18, kli18n("Department Name")},
                       {0x3A26, kli18n("Country")},
                       {0x3A27, kli18n("Locality")},
                       {0x3A28, kli18n("State/Province")},
                       {0x3A44, kli18n("Middle Name")},
                       {0x3A45, kli18n("Display Name Prefix")},

                       /* Some TNEF attributes */
                       {0x0008, kli18n("Owner Appointment ID")},
                       {0x0009, kli18n("Response Requested")},
                       {0x8000, kli18n("From")},
                       {0x8004, kli18n("Subject")},
                       {0x8005, kli18n("Date Sent")},
                       {0x8006, kli18n("Date Received")},
                       {0x8007, kli18n("Message Status")},
                       {0x8008, kli18n("Message Class")},
                       {0x8009, kli18n("Message ID")},
                       {0x800A, kli18n("Parent ID")},
                       {0x800B, kli18n("Conversation ID")},
                       {0x800C, kli18n("Body")},
                       {0x800D, kli18n("Priority")},
                       {0x800F, kli18n("Attachment Data")},
                       {0x8010, kli18n("Attachment Title")},
                       {0x8011, kli18n("Attachment Meta File")},
                       {0x8012, kli18n("Attachment Create Date")},
                       {0x8013, kli18n("Attachment Modify Date")},
                       {0x8020, kli18n("Date Modified")},
                       {0x9001, kli18n("Attachment Transport File Name")},
                       {0x9002, kli18n("Attachment Rendering Data")},
                       {0x9003, kli18n("MAPI Properties")},
                       {0x9004, kli18n("Recipients Table")},
                       {0x9005, kli18n("Attachment MAPI Properties")},
                       {0x9006, kli18n("TNEF Version")},
                       {0x9007, kli18n("OEM Code Page")},
                       {0, KLazyLocalizedString()}},
  MAPI_NamedTagStrings[] = {{0x8005, kli18n("Contact File Under")},
                            {0x8017, kli18n("Contact Last Name And First Name")},
                            {0x8018, kli18n("Contact Company And Full Name")},

                            {0x8080, kli18n("Contact EMail-1 Full")},
                            {0x8082, kli18n("Contact EMail-1 Address Type")},
                            {0x8083, kli18n("Contact EMail-1 Address")},
                            {0x8084, kli18n("Contact EMail-1 Display Name")},
                            {0x8085, kli18n("Contact EMail-1 Entry ID")},

                            {0x8090, kli18n("Contact EMail-2 Full")},
                            {0x8092, kli18n("Contact EMail-2 Address Type")},
                            {0x8093, kli18n("Contact EMail-2 Address")},
                            {0x8094, kli18n("Contact EMail-2 Display Name")},
                            {0x8095, kli18n("Contact EMail-2 Entry ID")},

                            {0x8208, kli18n("Appointment Location")},
                            {0x8208, kli18n("Appointment Location")},
                            {0x820D, kli18n("Appointment Start Date")},
                            {0x820E, kli18n("Appointment End Date")},
                            {0x8213, kli18n("Appointment Duration")},
                            {0x8218, kli18n("Appointment Response Status")},
                            {0x8223, kli18n("Appointment Is Recurring")},
                            {0x8231, kli18n("Appointment Recurrence Type")},
                            {0x8232, kli18n("Appointment Recurrence Pattern")},
                            {0x8502, kli18n("Reminder Time")},
                            {0x8503, kli18n("Reminder Set")},
                            {0x8516, kli18n("Start Date")},
                            {0x8517, kli18n("End Date")},
                            {0x8560, kli18n("Reminder Next Time")},
                            {0, KLazyLocalizedString()}};

using TagMap = QMap<int, QString>;
Q_GLOBAL_STATIC(TagMap, MAPI_TagMap)
Q_GLOBAL_STATIC(TagMap, MAPI_NamedTagMap)

//@endcond

QString KTnef::mapiTagString(int key)
{
    if (MAPI_TagMap()->isEmpty()) {
        for (int i = 0; !KLocalizedString(MAPI_TagStrings[i].str).isEmpty(); i++) {
            (*MAPI_TagMap())[MAPI_TagStrings[i].tag] = KLocalizedString(MAPI_TagStrings[i].str).toString();
        }
    }
    auto it = MAPI_TagMap()->constFind(key);
    if (it == MAPI_TagMap()->constEnd()) {
        return QString::asprintf("0x%04X", key);
    } else {
        return QString::asprintf("0x%04X ________: ", key) + *it;
    }
}

QString KTnef::mapiNamedTagString(int key, int tag)
{
    if (MAPI_NamedTagMap()->isEmpty()) {
        for (int i = 0; !KLocalizedString(MAPI_NamedTagStrings[i].str).isEmpty(); i++) {
            (*MAPI_TagMap())[MAPI_TagStrings[i].tag] = KLocalizedString(MAPI_NamedTagStrings[i].str).toString();
        }
    }
    auto it = MAPI_NamedTagMap()->constFind(key);
    if (it != MAPI_NamedTagMap()->constEnd()) {
        if (tag >= 0) {
            return QString::asprintf("0x%04X [0x%04X]: ", tag, key) + *it;
        } else {
            return QString::asprintf("0x%04X ________:", key) + *it;
        }
    } else {
        return QString::asprintf("0x%04X ________:", key);
    }
}
