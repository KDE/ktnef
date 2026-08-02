/*
    SPDX-FileCopyrightText: 2001-2003 Cornelius Schumacher <schumacher@kde.org>
    SPDX-FileCopyrightText: 2004 Reinhold Kainhofer <reinhold@kainhofer.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
/*!
  @file
  This file is part of the API for handling TNEF data and provides
  static Formatter helpers.

  \brief
  Provides helpers too format @acronym TNEF attachments into different
  formats like eg. a HTML representation.

  @author Cornelius Schumacher
  @author Reinhold Kainhofer
*/

#pragma once

#include <KCalendarCore/MemoryCalendar>
#include <QString>

#include "ktnef_export.h"

namespace KCalUtils
{
class InvitationFormatterHelper;
}

namespace KTnef
{

class KTNEFMessage;

/*!
    Create an iCal representation of an event, todo or invitation in a TNEF message.

    If the message does not contain a calendar object, an empty string is returned.

    \a tnefMsg the TNEF message to convert.
    \since 26.12
*/
[[nodiscard]] KTNEF_EXPORT QString messageToIcal(const KTnef::KTNEFMessage *tnefMsg);

/*!
    Create a vCard representation of a contact contained in a TNEF message.

    If the message does not contain a contact, an empty string is returned.

    \a tnefMsg the TNEF message to convert.
    \since 26.12
*/
[[nodiscard]] KTNEF_EXPORT QByteArray messageToVcard(const KTnef::KTNEFMessage *tnefMsg);

/*!
    Formats a @acronym TNEF attachment to an HTML mail.

    \a tnef is the QByteArray contain the @acronym TNEF data.
    \a cal is a pointer to a Calendar object.
    \a h is a pointer to a InvitationFormatterHelp object.
  */
[[nodiscard]] KTNEF_EXPORT QString formatTNEFInvitation(const QByteArray &tnef,
                                                        const KCalendarCore::MemoryCalendar::Ptr &cal,
                                                        KCalUtils::InvitationFormatterHelper *h);

/*!
    Transforms a @acronym TNEF attachment to an iCal or vCard.

    \a tnef is the QByteArray containing the @acronym TNEF data.

    Returns a string containing the transformed attachment.
  */
[[nodiscard]] KTNEF_EXPORT QString msTNEFToVPart(const QByteArray &tnef);
}
