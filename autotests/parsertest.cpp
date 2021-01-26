/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2015 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsertest.h"

#include "config-ktnef-tests.h"

#include <ktnef/ktnefattach.h>
#include <ktnef/ktnefmessage.h>
#include <ktnef/ktnefparser.h>

#include <QString>
#include <QTest>

using namespace KTnef;

QTEST_GUILESS_MAIN(ParserTest)

void ParserTest::testSingleAttachment()
{
    KTNEFParser parser;
    QVERIFY(parser.openFile(QString(QLatin1String(TESTSOURCEDIR) + QLatin1String("one-file.tnef"))) == true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QVERIFY(atts.count() == 1);

    KTNEFAttach *att = atts.first();
    QVERIFY(att != nullptr);
    QVERIFY(att->size() == 244);
    QVERIFY(att->name() == QLatin1String("AUTHORS"));
}

void ParserTest::testTwoAttachments()
{
    KTNEFParser parser;
    QVERIFY(parser.openFile(QString(QLatin1String(TESTSOURCEDIR) + QLatin1String("two-files.tnef"))) == true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QVERIFY(atts.count() == 2);

    KTNEFAttach *att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QVERIFY(att->size() == 244);
    QVERIFY(att->name() == QLatin1String("AUTHORS"));

    att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QVERIFY(att->size() == 893);
    QVERIFY(att->name() == QLatin1String("README"));
}

void ParserTest::testMAPIAttachments()
{
    KTNEFParser parser;
    QVERIFY(parser.openFile(QString(QLatin1String(TESTSOURCEDIR) + QLatin1String("mapi_attach_data_obj.tnef"))) == true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QVERIFY(atts.count() == 3);

    KTNEFAttach *att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QVERIFY(att->size() == 61952);
    QVERIFY(att->name() == QLatin1String("VIA_Nytt_1402.doc"));

    att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QVERIFY(att->size() == 213688);
    QVERIFY(att->name() == QLatin1String("VIA_Nytt_1402.pdf"));

    att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QVERIFY(att->size() == 68920);
    QVERIFY(att->name() == QLatin1String("VIA_Nytt_14021.htm"));
}

void ParserTest::testUmlautAttachmentFilenames()
{
    KTNEFParser parser;
    QVERIFY(parser.openFile(QString(QLatin1String(TESTSOURCEDIR) + QLatin1String("umlaut-filename.tnef"))) == true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QVERIFY(atts.count() == 1);

    KTNEFAttach *att = atts.first();
    QCOMPARE(att->fileName(), QString::fromUtf8("döcument.pdf"));
}
