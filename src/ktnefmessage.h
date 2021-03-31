/*
    ktnefmessage.h

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
 * @file
 * This file is part of the API for handling TNEF data and
 * defines the KTNEFMessage class.
 *
 * @author Michael Goffioul
 */

#pragma once

#include <QList>

#include "ktnef_export.h"
#include "ktnefpropertyset.h"

namespace KTnef
{
class KTNEFAttach;
}

namespace KTnef
{
/**
 * @brief
 * Represents a @acronym TNEF message.
 */
class KTNEF_EXPORT KTNEFMessage : public KTNEFPropertySet
{
public:
    /**
     * Creates a KTNEFMessage message object.
     */
    KTNEFMessage();

    /**
     * Destroys a KTNEFMessage message object.
     */
    ~KTNEFMessage();

    /**
     * Return a QList containing all the message's attachments.
     */
    const QList<KTNEFAttach *> &attachmentList() const;

    /**
     * Find the attachment associated to the specified file name.
     *
     * @param filename is a QString containing the file to search for in the
     * list of message attachments.
     *
     * @return A pointer to KTNEFAttach object, or 0 if the search fails.
     */
    KTNEFAttach *attachment(const QString &filename) const;

    /**
     * Append an attachment to the message.
     * @param attach is a pointer to a KTNEFAttach object to be attached.
     */
    void addAttachment(KTNEFAttach *attach);

    /**
     * Clear the attachments list.
     */
    void clearAttachments();

    /**
     * Returns the Rich Text Format (@acronym RTF) data contained in the message.
     * @return A QString containing the @acronym RTF data.
     */
    Q_REQUIRED_RESULT QString rtfString() const;

private:
    //@cond PRIVATE
    class MessagePrivate;
    MessagePrivate *const d;
    //@endcond

    Q_DISABLE_COPY(KTNEFMessage)
};

}
