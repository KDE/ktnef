/*
    ktnefproperty.h

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

#pragma once

#include "ktnef_export.h"
#include <QString>
#include <QVariant>
#include <memory>
class KTNEFPropertyPrivate;
namespace KTnef
{
/**
 * @brief
 * Interface for setting @acronym MAPI properties.
 */
class KTNEF_EXPORT KTNEFProperty
{
public:
    /**
     * The different @acronym MAPI types.
     */
    enum MAPIType {
        UInt16 = 0x0002, /**< 16-bit unsigned integer */
        ULong = 0x0003, /**< unsigned long integer */
        Float = 0x0004, /**< single precision floating point */
        Double = 0x0005, /**< double precision floating point */
        Boolean = 0x000B, /**< a boolean value */
        Object = 0x000D, /**< an object */
        Time = 0x0040, /**< a time value */
        String8 = 0x001E, /**< a string of 8 characters */
        UString = 0x001F, /**< a string of characters */
        Binary = 0x0102 /**< a binary value */
    };

    /**
     * Constructs a @acronym TNEF property.
     */
    KTNEFProperty();

    /**
     * Constructs a @acronym TNEF property initialized with specified settings.
     *
     * @param key_ is the property key.
     * @param type_ is the property type.
     * @param value_ is the property value.
     * @param name_ is the property name.
     */
    KTNEFProperty(int key_, int type_, const QVariant &value_, const QVariant &name_ = QVariant());

    /**
     * Constructs a @acronym TNEF property with settings from another property.
     *
     * @param p is a #KTNEFProperty.
     */
    KTNEFProperty(const KTNEFProperty &p);

    /**
     * Destroys the property.
     */
    ~KTNEFProperty();

    KTNEFProperty &operator=(const KTNEFProperty &other);

    /**
     * Returns the key string of the property.
     *
     * @return the key string.
     */
    [[nodiscard]] QString keyString() const;

    /**
     * Returns the value string of the property.
     *
     * @return the value string.
     */
    [[nodiscard]] QString valueString() const;

    /**
     * Creates a formatted string from the value of the property.
     *
     * @param v is the property value.
     * @param beautify if true uses a prettier format
     *
     * @return the formatted value string.
     */
    [[nodiscard]] static QString formatValue(const QVariant &v, bool beautify = true);

    /**
     * Returns the integer key of the property.
     *
     * @return the property key.
     */
    [[nodiscard]] int key() const;

    /**
     * Returns the integer type of the property.
     *
     * @return the property type.
     */
    [[nodiscard]] int type() const;

    /**
     * Returns the value of the property.
     *
     * @return the property value.
     */
    [[nodiscard]] QVariant value() const;

    /**
     * Returns the name of the property.
     *
     * @return the property name.
     */
    [[nodiscard]] QVariant name() const;

    /**
     * Determines if the property is a vector type.
     *
     * @returns true if the property is a vector type; otherwise false.
     */
    [[nodiscard]] bool isVector() const;

private:
    //@cond PRIVATE
    std::unique_ptr<KTNEFPropertyPrivate> const d;
    //@endcond
};

}
