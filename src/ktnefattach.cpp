/*
    ktnefattach.cpp

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
 * @file
 * This file is part of the API for handling TNEF data and
 * defines the KTNEFAttach class.
 *
 * @author Michael Goffioul
 */

#include "ktnefattach.h"

using namespace KTnef;

/**
 * Private class that helps to provide binary compatibility between releases.
 * @internal
 */
//@cond PRIVATE
class KTnef::KTNEFAttach::AttachPrivate
{
public:
    int state_;
    int size_;
    int offset_;
    int displaysize_;
    QString name_;
    int index_;
    QString filename_;
    QString displayname_;
    QString mimetag_;
    QString extension_;
};
//@endcond

KTNEFAttach::KTNEFAttach()
    : d(new KTnef::KTNEFAttach::AttachPrivate)
{
    d->state_ = Unparsed;
    d->offset_ = -1;
    d->size_ = 0;
    d->displaysize_ = 0;
    d->index_ = -1;
}

KTNEFAttach::~KTNEFAttach() = default;

void KTNEFAttach::setTitleParsed()
{
    d->state_ |= TitleParsed;
}

void KTNEFAttach::setDataParsed()
{
    d->state_ |= DataParsed;
}

void KTNEFAttach::unsetDataParser()
{
    d->state_ = (d->state_ & ~DataParsed);
}

void KTNEFAttach::setInfoParsed()
{
    d->state_ |= InfoParsed;
}

bool KTNEFAttach::titleParsed() const
{
    return d->state_ & TitleParsed;
}

bool KTNEFAttach::dataParsed() const
{
    return d->state_ & DataParsed;
}

bool KTNEFAttach::infoParsed() const
{
    return d->state_ & InfoParsed;
}

bool KTNEFAttach::checkState(int state) const
{
    return d->state_ & state;
}

int KTNEFAttach::offset() const
{
    return d->offset_;
}

void KTNEFAttach::setOffset(int n)
{
    setDataParsed();
    d->offset_ = n;
}

int KTNEFAttach::size() const
{
    return d->size_;
}

void KTNEFAttach::setSize(int s)
{
    d->size_ = s;
}

int KTNEFAttach::displaySize() const
{
    return d->displaysize_;
}

void KTNEFAttach::setDisplaySize(int s)
{
    d->displaysize_ = s;
}

QString KTNEFAttach::name() const
{
    return d->name_;
}

void KTNEFAttach::setName(const QString &str)
{
    setTitleParsed();
    d->name_ = str;
}

int KTNEFAttach::index() const
{
    return d->index_;
}

void KTNEFAttach::setIndex(int i)
{
    setInfoParsed();
    d->index_ = i;
}

QString KTNEFAttach::fileName() const
{
    return d->filename_;
}

void KTNEFAttach::setFileName(const QString &str)
{
    d->filename_ = str;
}

QString KTNEFAttach::displayName() const
{
    return d->displayname_;
}

void KTNEFAttach::setDisplayName(const QString &str)
{
    d->displayname_ = str;
}

QString KTNEFAttach::mimeTag() const
{
    return d->mimetag_;
}

void KTNEFAttach::setMimeTag(const QString &str)
{
    d->mimetag_ = str;
}

QString KTNEFAttach::extension() const
{
    return d->extension_;
}

void KTNEFAttach::setExtension(const QString &str)
{
    d->extension_ = str;
}
