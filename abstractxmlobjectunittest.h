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
#include "utilities.h"
#include <QXmlStreamReader>
#include <abstractxmlobject.h>
#include "mocks.h"

using namespace Molsketch;

const QString objectName("TestObject");
typedef std::function<void()> callback;

class AbstractXmlObjectForTesting : public abstractXmlObject {
public:
  QString xmlName() const override {
    return objectName;
  }

  typedef abstractXmlObject super;
  SUPERMOCK(QList<const abstractXmlObject*>, children, , )
  SUPERMOCK(abstractXmlObject*, produceChild, const QString& name COMMA const QString& type, name COMMA type)
  VOIDSUPERMOCK(readAttributes, const QXmlStreamAttributes& attr, attr)
  SUPERMOCK(QXmlStreamAttributes, xmlAttributes, , )
  SUPERMOCK(QStringList, textItemAttributes, , )
  VOIDSUPERMOCK(afterReadFinalization, , )
};

class AbstractXmlObjectUnitTest : public CxxTest::TestSuite {
  AbstractXmlObjectForTesting* testObject;

public:
  void setUp() {
    testObject = new AbstractXmlObjectForTesting;
  }

  void tearDown() {
    delete testObject;
  }

  void testDefaultImplementations() {
    TS_ASSERT_EQUALS(testObject->produceChild(objectName, objectName), nullptr);
    QS_ASSERT_EQUALS(testObject->children(), QList<const abstractXmlObject*>());
    TS_ASSERT_EQUALS(testObject->xmlAttributes(), QXmlStreamAttributes());
    QS_ASSERT_EQUALS(testObject->textItemAttributes(), QStringList());
  }

  void testReadElementWithoutContent() {
    QXmlStreamReader reader("<testElement testattribute=\"testvalue\" "
                            "otherattribute=\"othervalue\">"
                            "</testElement>");
    reader.readNext();
    reader.readNext();
    testObject->produceChildCallback = [](QString a, QString b) {
      TSM_ASSERT("produceChild() should not be called if there are no children", false);
      return new AbstractXmlObjectForTesting; };
    testObject->childrenCallback = []() {
      TSM_ASSERT("children() should not be called if there are no children", false);
      return QList<const abstractXmlObject*>(); };
    testObject->textItemAttributesCallback;
    testObject->xmlAttributesCallback = [] () {
      TSM_ASSERT("xmlAtrributes() should not be called during read", false);
      return QXmlStreamAttributes(); };
    QXmlStreamAttributes attributes;
    int attributesRead = 0;
    int finalizationPerformed = 0;
    testObject->readAttributesCallback = [&](const QXmlStreamAttributes& attribs){
      attributes = attribs;
      ++attributesRead;
      TSM_ASSERT("Finalization should happen after reading attributes", !finalizationPerformed); };
    testObject->afterReadFinalizationCallback = [&] (){
      TSM_ASSERT("Reading attributes should precede finalization", attributesRead);
      ++finalizationPerformed; };

    testObject->readXml(reader);

    TSM_ASSERT_EQUALS("Arguments should be read exactly once", attributesRead, 1);
    TSM_ASSERT_EQUALS("Finalization should occur exaclty once", finalizationPerformed, 1);

    TS_ASSERT_EQUALS(attributes.size(), 2);
    TS_ASSERT(attributes.contains(QXmlStreamAttribute("testattribute", "testvalue")));
    TS_ASSERT(attributes.contains(QXmlStreamAttribute("otherattribute", "othervalue")));
  }

  // Test: readAttributes called with attributes
  // Test: assert that stream is read after readXml (at end or at next element)
  // Test: afterReadFinalization called after readAttributes (and produceChild, if any)
  // Test: child read at correct positiion
};
