/*
    lzfu.cpp

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

#include "lzfu.h"

#include <QIODevice>

#include <cstdio>
#include <cstring>
#include <sys/types.h>

// #define DO_DEBUG

//@cond IGNORE
#define LZFU_COMPRESSED 0x75465a4c
#define LZFU_UNCOMPRESSED 0x414c454d

#define LZFU_INITDICT                                                                                                                                          \
    "{\\rtf1\\ansi\\mac\\deff0\\deftab720{\\fonttbl;}"                                                                                                         \
    "{\\f0\\fnil \\froman \\fswiss \\fmodern \\fscrip"                                                                                                         \
    "t \\fdecor MS Sans SerifSymbolArialTimes Ne"                                                                                                              \
    "w RomanCourier{\\colortbl\\red0\\green0\\blue0"                                                                                                           \
    "\r\n\\par \\pard\\plain\\f0\\fs20\\b\\i\\u\\tab"                                                                                                          \
    "\\tx"
#define LZFU_INITLENGTH 207
//@endcond

//@cond PRIVATE
using lzfuheader = struct _lzfuheader {
    quint32 cbSize;
    quint32 cbRawSize;
    quint32 dwMagic;
    quint32 dwCRC;
};
//@endcond

//@cond IGNORE
#define FLAG(f, n) (f >> n) & 0x1

/*typedef struct _blockheader {
  unsigned int offset:12;
  unsigned int length:4;
  } blockheader;*/

#define OFFSET(b) (b >> 4) & 0xFFF
#define LENGTH(b) ((b & 0xF) + 2)
//@endcond

int KTnef::lzfu_decompress(QIODevice *input, QIODevice *output)
{
    unsigned char window[4096];
    unsigned int wlength = 0;
    unsigned int cursor = 0;
    unsigned int ocursor = 0;
    lzfuheader lzfuhdr;
    // blockheader blkhdr;
    quint16 blkhdr;
    char bFlags;
    int nFlags;

    memcpy(window, LZFU_INITDICT, LZFU_INITLENGTH);
    wlength = LZFU_INITLENGTH;
    if (input->read((char *)&lzfuhdr, sizeof(lzfuhdr)) != sizeof(lzfuhdr)) {
        fprintf(stderr, "unexpected eof, cannot read LZFU header\n");
        return -1;
    }
    cursor += sizeof(lzfuhdr);
#ifdef DO_DEBUG
    fprintf(stdout, "total size : %d\n", lzfuhdr.cbSize + 4);
    fprintf(stdout, "raw size   : %d\n", lzfuhdr.cbRawSize);
    fprintf(stdout, "compressed : %s\n", (lzfuhdr.dwMagic == LZFU_COMPRESSED ? "yes" : "no"));
    fprintf(stdout, "CRC        : %x\n", lzfuhdr.dwCRC);
    fprintf(stdout, "\n");
#endif

    while (cursor < lzfuhdr.cbSize + 4 && ocursor < lzfuhdr.cbRawSize && !input->atEnd()) {
        if (input->read(&bFlags, 1) != 1) {
            fprintf(stderr, "unexpected eof, cannot read chunk flag\n");
            return -1;
        }
        nFlags = 8;
        cursor++;
#ifdef DO_DEBUG
        fprintf(stdout, "Flags : ");
        for (int i = nFlags - 1; i >= 0; i--) {
            fprintf(stdout, "%d", FLAG(bFlags, i));
        }
        fprintf(stdout, "\n");
#endif
        for (int i = 0; i < nFlags && ocursor < lzfuhdr.cbRawSize && cursor < lzfuhdr.cbSize + 4; i++) {
            if (FLAG(bFlags, i)) {
                // compressed chunk
                char c1;
                char c2;
                if (input->read(&c1, 1) != 1 || input->read(&c2, 1) != 1) {
                    fprintf(stderr, "unexpected eof, cannot read block header\n");
                    return -1;
                }
                blkhdr = c1;
                blkhdr <<= 8;
                blkhdr |= (0xFF & c2);
                unsigned int offset = OFFSET(blkhdr);
                unsigned int length = LENGTH(blkhdr);
                cursor += 2;
#ifdef DO_DEBUG
                fprintf(stdout, "block : offset=%.4d [%d], length=%.2d (0x%04X)\n", OFFSET(blkhdr), wlength, LENGTH(blkhdr), blkhdr);
#endif
                // if ( offset >= wlength ) {
                //     break;
                //}
#ifdef DO_DEBUG
                fprintf(stdout, "block : ");
#endif
                for (unsigned int i = 0; i < length; i++) {
                    c1 = window[(offset + i) % 4096];
                    // if ( wlength < 4096 ) {
                    window[wlength] = c1;
                    wlength = (wlength + 1) % 4096;
                    //}
#ifdef DO_DEBUG
                    if (c1 == '\n') {
                        fprintf(stdout, "\nblock : ");
                    } else {
                        fprintf(stdout, "%c", c1);
                    }
#endif
                    output->putChar(c1);
                    ocursor++;
                }
#ifdef DO_DEBUG
                fprintf(stdout, "\n");
#endif
            } else {
                // uncompressed chunk (char)
                char c;
                if (!input->getChar(&c)) {
                    if (!input->atEnd()) {
                        fprintf(stderr, "unexpected eof, cannot read character\n");
                        return -1;
                    }
                    break;
                }
#ifdef DO_DEBUG
                fprintf(stdout, "char  : %c\n", c);
#endif
                cursor++;
                // if ( wlength < 4096 ) {
                window[wlength] = c;
                wlength = (wlength + 1) % 4096;
                //}
                output->putChar(c);
                ocursor++;
            }
        }
    }

    return 0;
}
