/*
    SPDX-FileCopyrightText: 2007 Volker Krause <vkrause@kde.org>
    SPDX-FileCopyrightText: 2015 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "parsertest.h"
using namespace Qt::Literals::StringLiterals;

#include "config-ktnef-tests.h"

#include "ktnef/ktnefattach.h"
#include "ktnef/ktnefmessage.h"
#include "ktnef/ktnefparser.h"

#include <QString>
#include <QTest>

using namespace KTnef;

QTEST_GUILESS_MAIN(ParserTest)

void ParserTest::testSingleAttachment()
{
    KTNEFParser parser;
    QCOMPARE(parser.openFile(QString(QLatin1StringView(TESTSOURCEDIR) + "one-file.tnef"_L1)), true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QCOMPARE(atts.count(), 1);

    KTNEFAttach *att = atts.first();
    QVERIFY(att != nullptr);
    QCOMPARE(att->size(), 244);
    QCOMPARE(att->name(), "AUTHORS"_L1);
}

void ParserTest::testTwoAttachments()
{
    KTNEFParser parser;
    QCOMPARE(parser.openFile(QString(QLatin1StringView(TESTSOURCEDIR) + "two-files.tnef"_L1)), true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QCOMPARE(atts.count(), 2);

    KTNEFAttach *att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QCOMPARE(att->size(), 244);
    QCOMPARE(att->name(), "AUTHORS"_L1);

    att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QCOMPARE(att->size(), 893);
    QCOMPARE(att->name(), "README"_L1);
}

void ParserTest::testMAPIAttachments()
{
    KTNEFParser parser;
    QCOMPARE(parser.openFile(QString(QLatin1StringView(TESTSOURCEDIR) + "mapi_attach_data_obj.tnef"_L1)), true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QCOMPARE(atts.count(), 3);

    KTNEFAttach *att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QCOMPARE(att->size(), 61952);
    QCOMPARE(att->name(), "VIA_Nytt_1402.doc"_L1);

    att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QCOMPARE(att->size(), 213688);
    QCOMPARE(att->name(), "VIA_Nytt_1402.pdf"_L1);

    att = atts.takeFirst();
    QVERIFY(att != nullptr);
    QCOMPARE(att->size(), 68920);
    QCOMPARE(att->name(), "VIA_Nytt_14021.htm"_L1);
}

void ParserTest::testUmlautAttachmentFilenames()
{
    KTNEFParser parser;
    QCOMPARE(parser.openFile(QString(QLatin1StringView(TESTSOURCEDIR) + "umlaut-filename.tnef"_L1)), true);

    KTNEFMessage *msg = parser.message();
    QVERIFY(msg != nullptr);

    QList<KTNEFAttach *> atts = msg->attachmentList();
    QCOMPARE(atts.count(), 1);

    KTNEFAttach *att = atts.first();
    QCOMPARE(att->fileName(), QString::fromUtf8("döcument.pdf"));
}

#include "moc_parsertest.cpp"
