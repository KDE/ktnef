/*
    SPDX-FileCopyrightText: 2001-2003 Cornelius Schumacher <schumacher@kde.org>
    SPDX-FileCopyrightText: 2004 Reinhold Kainhofer <reinhold@kainhofer.com>

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
/**
    Formats a @acronym TNEF attachment to an HTML mail.

    @param tnef is the QByteArray contain the @acronym TNEF data.
    @param cal is a pointer to a Calendar object.
    @param h is a pointer to a InvitationFormatterHelp object.
  */
Q_REQUIRED_RESULT KTNEF_EXPORT QString formatTNEFInvitation(const QByteArray &tnef,
                                                            const KCalendarCore::MemoryCalendar::Ptr &cal,
                                                            KCalUtils::InvitationFormatterHelper *h);

/**
    Transforms a @acronym TNEF attachment to an iCal or vCard.

    @param tnef is the QByteArray containing the @acronym TNEF data.

    @return a string containing the transformed attachment.
  */
Q_REQUIRED_RESULT KTNEF_EXPORT QString msTNEFToVPart(const QByteArray &tnef);
}

