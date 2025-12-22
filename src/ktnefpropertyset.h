/*
    ktnefpropertyset.h

    SPDX-FileCopyrightText: 2002 Michael Goffioul <kdeprint@swing.be>

    This file is part of KTNEF, the KDE TNEF support library/program.

    SPDX-License-Identifier: LGPL-2.0-or-later
 */
/*!
 * @file
 * This file is part of the API for handling TNEF data and
 * defines the KTNEFPropertySet class.
 *
 * @author Michael Goffioul
 */

#pragma once

#include "ktnef_export.h"
#include <QMap>
#include <QVariant>
#include <memory>
class KTNEFPropertySetPrivate;
namespace KTnef
{
class KTNEFProperty;
}

namespace KTnef
{
/*!
 * \brief
 * Interface for setting @acronym MAPI properties and @acronym TNEF attributes.
 */
class KTNEF_EXPORT KTNEFPropertySet
{
public:
    /*!
      Constructor.
    */
    KTNEFPropertySet();

    /*!
      Destructor.
    */
    ~KTNEFPropertySet();

    /*!
      Adds a @acronym MAPI property.

      \a key is the property key.
      \a type is the property type.
      \a value is the property value.
      \a name is the property name.
      \a overwrite if true, then remove the property if it already exists.
    */
    void addProperty(int key, int type, const QVariant &value, const QVariant &name = QVariant(), bool overwrite = false);

    /*!
      Finds a property by \a key, returning a formatted value.

      \a key is the property key.
      \a fallback is the fallback formatted value to use if the \a key
      is not found.
      \a convertToUpper if true, then return the formatted value in all
      upper case characters.

      Returns a formatted value string.
    */
    [[nodiscard]] QString findProp(int key, const QString &fallback = QString(), bool convertToUpper = false) const;

    /*!
      Finds a property by \a name, returning a formatted value.

      \a name is the property name.
      \a fallback is the fallback formatted value to use if the \a name
      is not found.
      \a convertToUpper if true, then return the formatted value in all
      upper case characters.

      Returns a formatted value string.
    */
    [[nodiscard]] QString findNamedProp(const QString &name, const QString &fallback = QString(), bool convertToUpper = false) const;

    /*!
      Returns a #QMap of all (key,@acronym MAPI) properties
    */
    QMap<int, KTNEFProperty *> &properties();

    /*!
      Returns a #QMap of all (key,@acronym MAPI) properties
    */
    const QMap<int, KTNEFProperty *> &properties() const; // krazy:exclude=constref

    /*!
      Returns the property associated with the specified \a key.

      \a key is the property key.

      Returns the property.q
    */
    [[nodiscard]] QVariant property(int key) const;

    /*!
      Adds a @acronym TNEF attribute.

      \a key is the attribute key.
      \a type is the attribute type.
      \a value is the attribute value.
      \a overwrite if true, then remove the attribute if it already exists.
    */
    void addAttribute(int key, int type, const QVariant &value, bool overwrite = false);

    /*!
      Returns a #QMap of all (key,@acronym TNEF) attributes.
    */
    [[nodiscard]] QMap<int, KTNEFProperty *> &attributes();

    /*!
      Returns a #QMap of all (key,@acronym TNEF) attributes.
    */
    const QMap<int, KTNEFProperty *> &attributes() const; // krazy:exclude=constref

    /*!
      Returns the attribute associated with the specified \a key.

      \a key is the @acronym TNEF key.

      Returns the attribute associated with the key.
    */
    [[nodiscard]] QVariant attribute(int key) const;

    /*!
      Clears the @acronym MAPI and @acronym TNEF maps.

      \a deleteAll if true, delete the map memory as well.
    */
    void clear(bool deleteAll = false);

private:
    std::unique_ptr<KTNEFPropertySetPrivate> const d;

    Q_DISABLE_COPY(KTNEFPropertySet)
};

}
