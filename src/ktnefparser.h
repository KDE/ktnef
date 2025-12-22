/*
    ktnefparser.h

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/*!
 * @file
 * This file is part of the API for handling TNEF data and
 * defines the KTNEFParser class.
 *
 * @author Michael Goffioul
 */

#pragma once

#include "ktnef_export.h"
#include <QIODevice>
#include <QString>
#include <memory>

namespace KTnef
{
class KTNEFMessage;
}

namespace KTnef
{
/*!
 * \brief
 * Provides an @acronym TNEF parser.
 */
class KTNEF_EXPORT KTNEFParser
{
public:
    /*!
      Constructs a @acronym TNEF parser object.
    */
    KTNEFParser();

    /*!
      Destroys the @acronym TNEF parser object.
     */
    ~KTNEFParser();

    /*!
      Opens the \a filename for parsing.

      \a filename is the name of the file to open.
      Returns true if the open succeeded; otherwise false.
    */
    [[nodiscard]] bool openFile(const QString &filename) const;

    /*!
      Opens the #QIODevice \a device for parsing.

      \a device is the #QIODevice to open.
      Returns true if the open succeeded; otherwise false.
    */
    [[nodiscard]] bool openDevice(QIODevice *device);

    /*!
      Extracts a @acronym TNEF attachment having filename \a filename
      into the default directory.

      \a filename is the name of the file to extract the attachment into.
      Returns true if the extraction succeeds; otherwise false.
    */
    [[nodiscard]] bool extractFile(const QString &filename) const;

    /*!
      Extracts a @acronym TNEF attachment having filename \a filename
      into the directory \a dirname.

      \a filename is the name of the file to extract the attachment into.
      \a dirname is the name of the directory where the \a filename
      should be written.

      Returns true if the extraction succeeds; otherwise false.
    */
    [[nodiscard]] bool extractFileTo(const QString &filename, const QString &dirname) const;

    /*!
      Extracts all @acronym TNEF attachments into the default directory.

      Returns true if the extraction succeeds; otherwise false.
    */
    [[nodiscard]] bool extractAll();

    /*!
      Sets the default extraction directory to \a dirname.

      \a dirname is the name of the default extraction directory.
    */
    void setDefaultExtractDir(const QString &dirname);

    /*!
      Returns the KTNEFMessage used in the parsing process.

      Returns a pointer to a KTNEFMessage object.
    */
    KTNEFMessage *message() const;

private:
    class ParserPrivate;
    std::unique_ptr<ParserPrivate> const d;

    Q_DISABLE_COPY(KTNEFParser)
};

}
