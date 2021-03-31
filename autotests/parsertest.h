/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ParserTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testSingleAttachment();
    void testTwoAttachments();
    void testMAPIAttachments();
    void testUmlautAttachmentFilenames();
};

