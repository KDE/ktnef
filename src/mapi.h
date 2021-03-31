/*
    mapi.h

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

#pragma once

#include <QString>
namespace KTnef
{
/**
 * Convert a keycode to a @acronym MAPI tag string.
 * @param key The input code to convert.
 * @return A QString containing the tag string.
 */
Q_REQUIRED_RESULT QString mapiTagString(int key);

/**
 * Convert a keycode to a @acronym MAPI named tag string.
 * @param key The input code to convert.
 * @param tag An input tag.
 * @return A QString containing the named tag string.
 */
Q_REQUIRED_RESULT QString mapiNamedTagString(int key, int tag = -1);
}
