/*
    lzfu.h

    SPDX-FileCopyrightText: 2003 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
 * @file
 * This file is part of the API for handling TNEF data and
 * provides the @acronym LZFU decompression functionality.
 *
 * @author Michael Goffioul
 */

#pragma once
class QIODevice;
namespace KTnef
{
/**
 * @acronym LZFU decompress data in compressed Rich Text Format (@acronym RTF).
 * @param input compressed input data.
 * @param output decompressed output data.
 */
[[nodiscard]] int lzfu_decompress(QIODevice *input, QIODevice *output);
}
