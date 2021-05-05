/*
    ktnefpropertyset.cpp

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/**
 * @file
 * This file is part of the API for handling TNEF data and
 * defines the KTNEFPropertySet class.
 *
 * @author Michael Goffioul
 */

#include "ktnefpropertyset.h"
#include "ktnefproperty.h"

#include "ktnef_debug.h"

#include <QList>

using namespace KTnef;

class KTNEFPropertySetPrivate
{
public:
    QMap<int, KTNEFProperty *> properties_; // used to store MAPI properties
    QMap<int, KTNEFProperty *> attributes_; // used to store TNEF attributes
};

KTNEFPropertySet::KTNEFPropertySet()
    : d(new KTNEFPropertySetPrivate)
{
}

KTNEFPropertySet::~KTNEFPropertySet()
{
    clear(true);

    delete d;
}

void KTNEFPropertySet::addProperty(int key, int type, const QVariant &value, const QVariant &name, bool overwrite)
{
    QMap<int, KTNEFProperty *>::ConstIterator it = d->properties_.constFind(key);
    if (it != d->properties_.constEnd()) {
        if (overwrite) {
            delete (*it);
        } else {
            return;
        }
    }
    auto p = new KTNEFProperty(key, type, value, name);
    d->properties_[p->key()] = p;
}

QString KTNEFPropertySet::findProp(int key, const QString &fallback, bool upper) const
{
    QMap<int, KTNEFProperty *>::Iterator it = d->properties_.find(key);
    if (d->properties_.end() != it) {
        return upper ? KTNEFProperty::formatValue((*it)->value(), false).toUpper() : KTNEFProperty::formatValue((*it)->value(), false);
    } else {
        return fallback;
    }
}

QString KTNEFPropertySet::findNamedProp(const QString &name, const QString &fallback, bool upper) const
{
    for (QMap<int, KTNEFProperty *>::Iterator it = d->properties_.begin(), protEnd = d->properties_.end(); it != protEnd; ++it) {
        if ((*it)->name().isValid()) {
            QString s;
            if ((*it)->name().type() == QVariant::String) {
                s = (*it)->name().toString();
            } else {
                s = QString::asprintf("0X%04X", (*it)->name().toUInt());
            }

            if (s.toUpper() == name.toUpper()) {
                QVariant value = (*it)->value();
                if (value.type() == QVariant::List) {
                    QList<QVariant> l = value.toList();
                    s.clear();
                    for (QList<QVariant>::ConstIterator lit = l.constBegin(), litEnd = l.constEnd(); lit != litEnd; ++lit) {
                        if (!s.isEmpty()) {
                            s += QLatin1Char(',');
                        }
                        s += KTNEFProperty::formatValue(*lit, false);
                    }
                } else {
                    s = KTNEFProperty::formatValue(value, false);
                }
                return upper ? s.toUpper() : s;
            }
        }
    }
    return fallback;
}

QMap<int, KTNEFProperty *> &KTNEFPropertySet::properties()
{
    return d->properties_;
}

const QMap<int, KTNEFProperty *> &KTNEFPropertySet::properties() const
{
    return d->properties_;
}

QVariant KTNEFPropertySet::property(int key) const
{
    QMap<int, KTNEFProperty *>::ConstIterator it = d->properties_.constFind(key);
    if (it == d->properties_.constEnd()) {
        return QVariant();
    } else {
        return (*it)->value();
    }
}

void KTNEFPropertySet::clear(bool deleteAll)
{
    if (deleteAll) {
        for (QMap<int, KTNEFProperty *>::ConstIterator it = d->properties_.constBegin(); it != d->properties_.constEnd(); ++it) {
            delete (*it);
        }
        for (QMap<int, KTNEFProperty *>::ConstIterator it = d->attributes_.constBegin(); it != d->attributes_.constEnd(); ++it) {
            delete (*it);
        }
    }
    d->properties_.clear();
    d->attributes_.clear();
}

void KTNEFPropertySet::addAttribute(int key, int type, const QVariant &value, bool overwrite)
{
    QMap<int, KTNEFProperty *>::ConstIterator it = d->attributes_.constFind(key);
    if (it != d->attributes_.constEnd()) {
        if (overwrite) {
            delete (*it);
        } else {
            return;
        }
    }
    auto *p = new KTNEFProperty(key, type, value, QVariant());
    d->attributes_[p->key()] = p;
}

QMap<int, KTNEFProperty *> &KTNEFPropertySet::attributes()
{
    return d->attributes_;
}

const QMap<int, KTNEFProperty *> &KTNEFPropertySet::attributes() const
{
    return d->attributes_;
}

QVariant KTNEFPropertySet::attribute(int key) const
{
    QMap<int, KTNEFProperty *>::ConstIterator it = d->attributes_.constFind(key);
    if (it == d->attributes_.constEnd()) {
        return QVariant();
    } else {
        return (*it)->value();
    }
}
