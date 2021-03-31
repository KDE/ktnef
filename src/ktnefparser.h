/*
    ktnefparser.h

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
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

namespace KTnef
{
class KTNEFMessage;
}

namespace KTnef
{
/**
 * @brief
 * Provides an @acronym TNEF parser.
 */
class KTNEF_EXPORT KTNEFParser
{
public:
    /**
      Constructs a @acronym TNEF parser object.
    */
    KTNEFParser();

    /**
      Destroys the @acronym TNEF parser object.
     */
    ~KTNEFParser();

    /**
      Opens the @p filename for parsing.

      @param filename is the name of the file to open.
      @return true if the open succeeded; otherwise false.
    */
    Q_REQUIRED_RESULT bool openFile(const QString &filename) const;

    /**
      Opens the #QIODevice @p device for parsing.

      @param device is the #QIODevice to open.
      @return true if the open succeeded; otherwise false.
    */
    Q_REQUIRED_RESULT bool openDevice(QIODevice *device);

    /**
      Extracts a @acronym TNEF attachment having filename @p filename
      into the default directory.

      @param filename is the name of the file to extract the attachment into.
      @return true if the extraction succeeds; otherwise false.
    */
    Q_REQUIRED_RESULT bool extractFile(const QString &filename) const;

    /**
      Extracts a @acronym TNEF attachment having filename @p filename
      into the directory @p dirname.

      @param filename is the name of the file to extract the attachment into.
      @param dirname is the name of the directory where the @p filename
      should be written.

      @return true if the extraction succeeds; otherwise false.
    */
    Q_REQUIRED_RESULT bool extractFileTo(const QString &filename, const QString &dirname) const;

    /**
      Extracts all @acronym TNEF attachments into the default directory.

      @return true if the extraction succeeds; otherwise false.
    */
    Q_REQUIRED_RESULT bool extractAll();

    /**
      Sets the default extraction directory to @p dirname.

      @param dirname is the name of the default extraction directory.
    */
    void setDefaultExtractDir(const QString &dirname);

    /**
      Returns the KTNEFMessage used in the parsing process.

      @return a pointer to a KTNEFMessage object.
    */
    KTNEFMessage *message() const;

private:
    //@cond PRIVATE
    class ParserPrivate;
    ParserPrivate *const d;
    //@endcond

    Q_DISABLE_COPY(KTNEFParser)
};

}
