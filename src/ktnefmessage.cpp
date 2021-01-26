/*
    ktnefmessage.cpp

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

#include "ktnefmessage.h"
#include "ktnefattach.h"
#include "lzfu.h"

#include <QBuffer>
#include <QDebug>

using namespace KTnef;

/**
 * Private class that helps to provide binary compatibility between releases.
 * @internal
 */
//@cond PRIVATE
class KTnef::KTNEFMessage::MessagePrivate
{
public:
    MessagePrivate()
    {
    }
    ~MessagePrivate();

    void clearAttachments();

    QList<KTNEFAttach *> attachments_;
};

KTNEFMessage::MessagePrivate::~MessagePrivate()
{
    clearAttachments();
}

void KTNEFMessage::MessagePrivate::clearAttachments()
{
    while (!attachments_.isEmpty()) {
        delete attachments_.takeFirst();
    }
}
//@endcond

KTNEFMessage::KTNEFMessage()
    : d(new KTnef::KTNEFMessage::MessagePrivate)
{
}

KTNEFMessage::~KTNEFMessage()
{
    delete d;
}

const QList<KTNEFAttach *> &KTNEFMessage::attachmentList() const
{
    return d->attachments_;
}

KTNEFAttach *KTNEFMessage::attachment(const QString &filename) const
{
    QList<KTNEFAttach *>::const_iterator it = d->attachments_.constBegin();
    const QList<KTNEFAttach *>::const_iterator end = d->attachments_.constEnd();
    for (; it != end; ++it) {
        if ((*it)->name() == filename) {
            return *it;
        }
    }
    return nullptr;
}

void KTNEFMessage::addAttachment(KTNEFAttach *attach)
{
    d->attachments_.append(attach);
}

void KTNEFMessage::clearAttachments()
{
    d->clearAttachments();
}

QString KTNEFMessage::rtfString() const
{
    QVariant prop = property(0x1009);
    if (prop.isNull() || prop.type() != QVariant::ByteArray) {
        return QString();
    } else {
        QByteArray rtf;
        QByteArray propArray(prop.toByteArray());
        QBuffer input(&propArray), output(&rtf);
        if (input.open(QIODevice::ReadOnly) && output.open(QIODevice::WriteOnly)) {
            if (KTnef::lzfu_decompress(&input, &output) == 0) {
                qWarning() << "Error when decompress data";
            }
        }
        return QString::fromLatin1(rtf);
    }
}
