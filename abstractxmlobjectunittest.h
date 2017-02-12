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
#include "utilities.h"

using namespace Molsketch;

const QString objectName("TestObject");
typedef std::function<void()> callback;

class AbstractXmlObjectForTesting : public abstractXmlObject {
public:
  QString xmlName() const override {
    return objectName;
  }

  typedef abstractXmlObject super;
  SUPER_MOCK(QList<const abstractXmlObject*>, children, , )
  SUPER_MOCK(abstractXmlObject*, produceChild, const QString& name COMMA const QString& type, name COMMA type)
  VOID_SUPER_MOCK(readAttributes, const QXmlStreamAttributes& attr, attr)
  SUPER_MOCK(QXmlStreamAttributes, xmlAttributes, , )
  SUPER_MOCK(QStringList, textItemAttributes, , )
  VOID_SUPER_MOCK(afterReadFinalization, , )
};

class AbstractXmlObjectUnitTest : public CxxTest::TestSuite {
  AbstractXmlObjectForTesting* testObject;
  QXmlStreamReader *reader;

  void prepareReader(const QString& input) {
    reader = new QXmlStreamReader(input);
    reader->readNext();
    reader->readNext();
  }

  static std::function<abstractXmlObject *(QString, QString)> noChildrenToProduce() {
    return [](QString a, QString b) { Q_UNUSED(a) Q_UNUSED(b)
      TSM_ASSERT("produceChild() should not be called if there are no children", false);
      return new AbstractXmlObjectForTesting; };
  }

  static std::function<QList<const abstractXmlObject*>() > shouldNotCallChildren() {
    return [] {
      TSM_ASSERT("children() should not be called during read", false);
      return QList<const abstractXmlObject*>(); };
  }

  static std::function<QXmlStreamAttributes()> shouldNotCallAttributes() {
    return [] () {
      TSM_ASSERT("xmlAtrributes() should not be called during read", false);
      return QXmlStreamAttributes(); };
  }

  struct ReadingTestSetup {
    AbstractXmlObjectForTesting *object;
    QXmlStreamAttributes readAttributes;
    int timesAttributesWereRead;
    int timesFinalized;

    ReadingTestSetup(AbstractXmlObjectForTesting* object)
      : object(object),
        timesAttributesWereRead(0),
        timesFinalized(0) {
      prepareDefaultScenario();
    }

    void prepareDefaultScenario() {
      object->produceChildCallback = noChildrenToProduce();
      object->childrenCallback = shouldNotCallChildren();
      object->xmlAttributesCallback = shouldNotCallAttributes();
      object->readAttributesCallback = [&](const QXmlStreamAttributes& attribs){
        readAttributes = attribs;
        ++timesAttributesWereRead;
        TSM_ASSERT("Finalization should happen after reading attributes", !timesFinalized); };
      object->afterReadFinalizationCallback = [&] (){
        TSM_ASSERT("Reading attributes should precede finalization", timesAttributesWereRead);
        ++timesFinalized; };
    }

    void assertTestResult(QList<QPair<QString, QString> > expectedAttributes = {}) {
      TSM_ASSERT_EQUALS("Arguments should be read exactly once", timesAttributesWereRead, 1);
      TSM_ASSERT_EQUALS("Finalization should occur exaclty once", timesFinalized, 1);

      TS_ASSERT_EQUALS(readAttributes.size(), expectedAttributes.size());
      for (auto attribute : expectedAttributes)
        TSM_ASSERT(("Did not contain " + attribute.first +"/" + attribute.second).toStdString(), readAttributes.contains(QXmlStreamAttribute(attribute.first, attribute.second)));
    }

  };


public:
  void setUp() {
    testObject = new AbstractXmlObjectForTesting;
    reader = 0;
  }

  void tearDown() {
    if (reader) {
      TS_ASSERT(reader->isEndElement())
      QS_ASSERT_EQUALS(reader->name(), "testElement")
    }
    delete testObject;
    delete reader;
  }

  void testDefaultImplementations() {
    TS_ASSERT_EQUALS(testObject->produceChild(objectName, objectName), nullptr);
    QS_ASSERT_EQUALS(testObject->children(), QList<const abstractXmlObject*>());
    TS_ASSERT_EQUALS(testObject->xmlAttributes(), QXmlStreamAttributes());
    QS_ASSERT_EQUALS(testObject->textItemAttributes(), QStringList());
  }

  void testReadBareElement() {
    ReadingTestSetup setup(testObject);
    prepareReader("<testElement></testElement>");
    testObject->readXml(*reader);
    setup.assertTestResult();
  }

  void testReadElementWithAttributes() {
    ReadingTestSetup setup(testObject);

    prepareReader("<testElement testattribute=\"testvalue\" "
                  "otherattribute=\"othervalue\">"
                  "</testElement>");
    testObject->readXml(*reader);

    setup.assertTestResult({{"testattribute", "testvalue"}, {"otherattribute", "othervalue"}});
  }

  void testReadingChild() {
    AbstractXmlObjectForTesting *child = new AbstractXmlObjectForTesting();
    ReadingTestSetup setup(testObject), childSetup(child);

    int timesChildProduced = 0;
    testObject->produceChildCallback = [&](const QString& name, const QString& type) {
      QS_ASSERT_EQUALS(name, "childObject");
      QS_ASSERT_EQUALS(type, "childType");
      TSM_ASSERT("Should produce children before finalization", !setup.timesFinalized);
      TSM_ASSERT_EQUALS("Should read attributes before reading childern", setup.timesAttributesWereRead, 1);
      ++timesChildProduced;
      return child;
    };

    prepareReader("<testElement>"
                  "  <childObject type=\"childType\">"
                  "  </childObject>"
                  "</testElement>");
    testObject->readXml(*reader);

    setup.assertTestResult();
    childSetup.assertTestResult({{"type", "childType"}});
  }

};
