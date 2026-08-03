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

#include <QString>

#include "ktnef_export.h"

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
}
