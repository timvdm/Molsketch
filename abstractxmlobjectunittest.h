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
#include <abstractxmlobject.h>

using namespace Molsketch;

const QString objectName("TestObject");
typedef std::function<void()> callback;

class AbstractXmlObjectForTesting : public abstractXmlObject {
public:
  QString xmlName() const override {
    return objectName;
  }

  void testDefaultImplementations() {
    TSM_ASSERT_EQUALS("Default implementation of produceChild() should return null pointer", abstractXmlObject::produceChild(objectName, objectName), nullptr);
    QS_ASSERT_EQUALS(abstractXmlObject::children(), QList<const abstractXmlObject*>());
    TS_ASSERT_EQUALS(abstractXmlObject::xmlAttributes(), QXmlStreamAttributes());
    QS_ASSERT_EQUALS(abstractXmlObject::textItemAttributes(), QStringList());
  }

  // abstractXmlObject interface
public:
  AbstractXmlObjectForTesting() :
    produceChildCallback([]{})
  {}
  // TODO write some MOCK macro (static meta info?)
  callback produceChildCallback;
  abstractXmlObject *produceChild(const QString &name, const QString &type) override {
    produceChildCallback;
  }
  callback readAttributesCallback;
  void readAttributes(const QXmlStreamAttributes &attributes) override {
  }
  QList<const abstractXmlObject *> children() const override {
  }
  QXmlStreamAttributes xmlAttributes() const override {
  }
  QStringList textItemAttributes() const override {
  }
  void afterReadFinalization() override {
  }
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
    testObject->testDefaultImplementations();
  }

  // Test: readAttributes called with attributes
  // Test: assert that stream is read after readXml (at end or at next element)
  // Test: afterReadFinalization called after readAttributes (and produceChild, if any)
  // Test: child read at correct positiion
};
