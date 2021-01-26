/*
    ktnefproperty.cpp

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
 * @file
 * This file is part of the API for handling TNEF data and
 * defines the KTNEFProperty class.
 *
 * @author Michael Goffioul
 */

#include "ktnefproperty.h"
#include "mapi.h"

#include <ctype.h>

using namespace KTnef;

class KTNEFPropertyPrivate
{
public:
    int _key = 0;
    int _type = 0;
    QVariant _value;
    QVariant _name;
};

KTNEFProperty::KTNEFProperty()
    : d(new KTNEFPropertyPrivate)
{
}

KTNEFProperty::KTNEFProperty(int key_, int type_, const QVariant &value_, const QVariant &name_)
    : d(new KTNEFPropertyPrivate)
{
    d->_key = key_;
    d->_type = type_;
    d->_value = value_;
    d->_name = name_;
}

KTNEFProperty::KTNEFProperty(const KTNEFProperty &p)
    : d(new KTNEFPropertyPrivate)
{
    *d = *p.d;
}

KTNEFProperty::~KTNEFProperty()
{
    delete d;
}

KTNEFProperty &KTNEFProperty::operator=(const KTNEFProperty &other)
{
    if (this != &other) {
        *d = *other.d;
    }

    return *this;
}

QString KTNEFProperty::keyString() const
{
    if (d->_name.isValid()) {
        if (d->_name.type() == QVariant::String) {
            return d->_name.toString();
        } else {
            return mapiNamedTagString(d->_name.toUInt(), d->_key);
        }
    } else {
        return mapiTagString(d->_key);
    }
}

QString KTNEFProperty::formatValue(const QVariant &value, bool beautify)
{
    if (value.type() == QVariant::ByteArray) {
        // check the first bytes (up to 8) if they are
        // printable characters
        QByteArray arr = value.toByteArray();
        bool printable = true;
        for (int i = qMin(arr.size(), 8) - 1; i >= 0 && printable; i--) {
            printable = (isprint(arr[i]) != 0);
        }
        if (!printable) {
            QString s;
            int i;
            int txtCount = beautify ? qMin(arr.size(), 32) : arr.size();
            for (i = 0; i < txtCount; ++i) {
                s.append(QString::asprintf("%02X", (uchar)arr[i]));
                if (beautify) {
                    s.append(QLatin1Char(' '));
                }
            }
            if (i < arr.size()) {
                s.append(QLatin1String("... (size=") + QString::number(arr.size()) + QLatin1Char(')'));
            }
            return s;
        }
    }
    // else if ( value.type() == QVariant::DateTime )
    //   return value.toDateTime().toString();
    return value.toString();
}

QString KTNEFProperty::valueString() const
{
    return formatValue(d->_value);
}

int KTNEFProperty::key() const
{
    return d->_key;
}

int KTNEFProperty::type() const
{
    return d->_type;
}

QVariant KTNEFProperty::value() const
{
    return d->_value;
}

QVariant KTNEFProperty::name() const
{
    return d->_name;
}

bool KTNEFProperty::isVector() const
{
    return d->_value.type() == QVariant::List;
}
