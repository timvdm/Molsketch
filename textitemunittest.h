/***************************************************************************
 *   Copyright (C) 2017 by Hendrik Vennekate                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cxxtest/TestSuite.h>
#include <textitem.h>
#include "utilities.h"

using namespace Molsketch;

class TextItemUnitTest : public CxxTest::TestSuite {
  TextItem *textItem;

private:
  QString writeOut() {
    QString output;
    QXmlStreamWriter writer(&output);
    textItem->writeXml(writer);
    return output;
  }

public:
  void setUp() {
    textItem = new TextItem;
  }

  void tearDown() {
    delete textItem;
  }

  void testWritingXmlEmpty() {
    QS_ASSERT_EQUALS("<textItem><![CDATA[]]></textItem>", writeOut());
  }

  void testWritingXmlWithContent() {
    QString htmlText = QStringLiteral("Some text");
    textItem->setHtml(htmlText);

    QString output = writeOut();
    QS_ASSERT_EQUALS("<textItem>", output.left(10));
    QS_ASSERT_EQUALS("</textItem>", output.right(11));
    TS_ASSERT(output.contains(htmlText));
  }

  void testReadingXmlEmpty() {
    QString input = QStringLiteral("<textItem><![CDATA[]]></textItem>");
    QXmlStreamReader reader(input);
    reader.readNextStartElement();
    textItem->readXml(reader);
    QS_ASSERT_EQUALS("", textItem->toPlainText());
  }

  void testReadingXmlWithContent() {
    QString input = QStringLiteral("<textItem><![CDATA[<p>Test Text</p>]]></textItem>");
    QXmlStreamReader reader(input);
    reader.readNextStartElement();
    textItem->readXml(reader);
    TS_ASSERT(textItem->toHtml().contains("Test Text"));
    QS_ASSERT_EQUALS("Test Text", textItem->toPlainText());
  }

  // TODO can be read by MolScene
  // TODO can be written by MolScene
};
