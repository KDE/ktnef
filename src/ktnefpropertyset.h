/*
    ktnefpropertyset.h

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

#pragma once

#include <QMap>
#include <QVariant>
class KTNEFPropertySetPrivate;
#include "ktnef_export.h"

namespace KTnef
{
class KTNEFProperty;
}

namespace KTnef
{
/**
 * @brief
 * Interface for setting @acronym MAPI properties and @acronym TNEF attributes.
 */
class KTNEF_EXPORT KTNEFPropertySet
{
public:
    /**
      Constructor.
    */
    KTNEFPropertySet();

    /**
      Destructor.
    */
    ~KTNEFPropertySet();

    /**
      Adds a @acronym MAPI property.

      @param key is the property key.
      @param type is the property type.
      @param value is the property value.
      @param name is the property name.
      @param overwrite if true, then remove the property if it already exists.
    */
    void addProperty(int key, int type, const QVariant &value, const QVariant &name = QVariant(), bool overwrite = false);

    /**
      Finds a property by @p key, returning a formatted value.

      @param key is the property key.
      @param fallback is the fallback formatted value to use if the @p key
      is not found.
      @param convertToUpper if true, then return the formatted value in all
      upper case characters.

      @return a formatted value string.
    */
    Q_REQUIRED_RESULT QString findProp(int key, const QString &fallback = QString(), bool convertToUpper = false) const;

    /**
      Finds a property by @p name, returning a formatted value.

      @param name is the property name.
      @param fallback is the fallback formatted value to use if the @p name
      is not found.
      @param convertToUpper if true, then return the formatted value in all
      upper case characters.

      @return a formatted value string.
    */
    Q_REQUIRED_RESULT QString findNamedProp(const QString &name, const QString &fallback = QString(), bool convertToUpper = false) const;

    /**
      Returns a #QMap of all (key,@acronym MAPI) properties
    */
    QMap<int, KTNEFProperty *> &properties();

    /**
      Returns a #QMap of all (key,@acronym MAPI) properties
    */
    const QMap<int, KTNEFProperty *> &properties() const; // krazy:exclude=constref

    /**
      Returns the property associated with the specified @p key.

      @param key is the property key.

      @return the property.q
    */
    Q_REQUIRED_RESULT QVariant property(int key) const;

    /**
      Adds a @acronym TNEF attribute.

      @param key is the attribute key.
      @param type is the attribute type.
      @param value is the attribute value.
      @param overwrite if true, then remove the attribute if it already exists.
    */
    void addAttribute(int key, int type, const QVariant &value, bool overwrite = false);

    /**
      Returns a #QMap of all (key,@acronym TNEF) attributes.
    */
    Q_REQUIRED_RESULT QMap<int, KTNEFProperty *> &attributes();

    /**
      Returns a #QMap of all (key,@acronym TNEF) attributes.
    */
    const QMap<int, KTNEFProperty *> &attributes() const; // krazy:exclude=constref

    /**
      Returns the attribute associated with the specified @p key.

      @param key is the @acronym TNEF key.

      @return the attribute associated with the key.
    */
    Q_REQUIRED_RESULT QVariant attribute(int key) const;

    /**
      Clears the @acronym MAPI and @acronym TNEF maps.

      @param deleteAll if true, delete the map memory as well.
    */
    void clear(bool deleteAll = false);

private:
    //@cond PRIVATE
    KTNEFPropertySetPrivate *const d;
    //@endcond

    Q_DISABLE_COPY(KTNEFPropertySet)
};

}
