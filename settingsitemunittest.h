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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "signalcounter.h"

#include <cxxtest/TestSuite.h>
#include <settingsfacade.h>
#include <settingsitem.h>

using namespace Molsketch;

const QString DOUBLE_KEY("TestDoubleKey");
const qreal DOUBLE_VALUE = 3.75;
const QVariant DOUBLE_VARIANT(DOUBLE_VALUE);
const QString DOUBLE_AS_STRING("3.75");
const QString DOUBLE_XML("<" + DOUBLE_KEY + " value=\"" + DOUBLE_AS_STRING + "\"/>");
const qreal ALTERNATE_DOUBLE = 4.5;
const QVariant ALTERNATE_DOUBLE_VARIANT(4.5);
const QString ALTERNATE_DOUBLE_AS_STRING("4.5");

const QString BOOL_KEY("TestBoolKey");
const bool BOOL_VALUE = false;
const QVariant BOOL_VARIANT(false);
const QString BOOL_AS_STRING("false");
const QString BOOL_XML("<" + BOOL_KEY + " value=\"" + BOOL_AS_STRING + "\"/>");
const bool ALTERNATE_BOOL = true;
const QVariant ALTERNATE_BOOL_VARIANT(true);
const QString ALTERNATE_BOOL_AS_STRING("True");

const QString COLOR_KEY("TestColorKey");
const QColor COLOR_VALUE(Qt::red);
const QVariant COLOR_VARIANT(COLOR_VALUE);
const QString COLOR_AS_STRING("Af////8AAAAAAAA=");
const QString COLOR_XML("<" + COLOR_KEY + " value=\"" + COLOR_AS_STRING + "\"/>");
const QColor ALTERNATE_COLOR(Qt::blue);
const QVariant ALTERNATE_COLOR_VARIANT(ALTERNATE_COLOR);
const QString ALTERNATE_COLOR_AS_STRING("Af//AAAAAP//AAA=");

const QString FONT_KEY("TestFontKey");
const QFont FONT_VALUE("Times", 10, QFont::Bold);
const QVariant FONT_VARIANT(FONT_VALUE);
const QString FONT_AS_STRING("AAAACgBUAGkAbQBlAHP/////QCQAAAAAAAD/////BQABAEsQAGQBAAAAAAAAAAAAAA==");
const QString FONT_XML("<" + FONT_KEY + " value=\"" + FONT_AS_STRING + "\"/>");
const QFont ALTERNATE_FONT("Helvetica", 15, QFont::Cursive);
const QVariant ALTERNATE_FONT_VARIANT(ALTERNATE_FONT);
const QString ALTERNATE_FONT_AS_STRING("AAAAEgBIAGUAbAB2AGUAdABpAGMAYf////9ALgAAAAAAAP////8FAAEABhAAZAEAAAAAAAAAAAAA");

const QString STRING_LIST_KEY("TestStringListKey");
const QStringList STRING_LIST_VALUE{"testString1", "testString2"};
const QVariant STRING_LIST_VARIANT(STRING_LIST_VALUE);
const QString STRING_LIST_AS_STRING("AAAAAgAAABYAdABlAHMAdABTAHQAcgBpAG4AZwAxAAAAFgB0AGUAcwB0AFMAdAByAGkAbgBnADI=");
const QString STRING_LIST_XML("<" + STRING_LIST_KEY + " value=\"" + STRING_LIST_AS_STRING + "\"/>");
const QStringList ALTERNATE_STRING_LIST{"alternate1", "Alternate2"};
const QVariant ALTERNATE_STRING_LIST_VARIANT(ALTERNATE_STRING_LIST);
const QString ALTERNATE_STRING_LIST_AS_STRING("AAAAAgAAABQAYQBsAHQAZQByAG4AYQB0AGUAMQAAABQAQQBsAHQAZQByAG4AYQB0AGUAMg==");

class SettingsItemUnitTest : public CxxTest::TestSuite {
  SettingsFacade *facade, *emptyFacade;
  QRealSignalCounter *qrealSignalCounter;
  BoolSignalCounter *boolSignalCounter;
  ColorSignalCounter *colorSignalCounter;
  FontSignalCounter *fontSignalCounter;
  StringListSignalCounter *stringListSignalCounter;

  template<typename T>
  void performDoubleSetterTest(const T& newValue, const T& oldValue) {
    DoubleSettingsItem doubleSettingsItem(DOUBLE_KEY, facade);
    QObject::connect(&doubleSettingsItem, SIGNAL(updated(qreal)), qrealSignalCounter, SLOT(record(qreal)));
    qrealSignalCounter->callback =
        [&] (const qreal&) { qrealSignalCounter->callback = nullptr; doubleSettingsItem.set(newValue); };

    doubleSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(DOUBLE_KEY), ALTERNATE_DOUBLE_VARIANT);
    qrealSignalCounter->assertPayloads({ALTERNATE_DOUBLE});

    doubleSettingsItem.set(oldValue);

    TS_ASSERT_EQUALS(facade->value(DOUBLE_KEY), DOUBLE_VARIANT);
    qrealSignalCounter->assertPayloads({ALTERNATE_DOUBLE, DOUBLE_VALUE});
  }

  template<typename T>
  void performBoolSetterTest(const T& newValue, const T& oldValue) {
    BoolSettingsItem boolSettingsItem(BOOL_KEY, facade);
    QObject::connect(&boolSettingsItem, SIGNAL(updated(bool)), boolSignalCounter, SLOT(record(bool)));
    boolSignalCounter->callback = // TODO is this lambda deleted?
        [&] (const bool&) { boolSignalCounter->callback = nullptr; boolSettingsItem.set(newValue); };

    boolSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(BOOL_KEY), ALTERNATE_BOOL_VARIANT);
    boolSignalCounter->assertPayloads({ALTERNATE_BOOL});

    boolSettingsItem.set(oldValue);

    TS_ASSERT_EQUALS(facade->value(BOOL_KEY), BOOL_VARIANT);
    boolSignalCounter->assertPayloads({ALTERNATE_BOOL, BOOL_VALUE});
  }

  template<typename T>
  void performColorSetterTest(const T& newValue, const T& oldValue) {
    ColorSettingsItem colorSettingsItem(COLOR_KEY, facade);
    QObject::connect(&colorSettingsItem, SIGNAL(updated(QColor)), colorSignalCounter, SLOT(record(QColor)));
    colorSignalCounter->callback =
        [&] (const QColor&) { colorSignalCounter->callback = nullptr; colorSettingsItem.set(newValue); };

    colorSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(COLOR_KEY), ALTERNATE_COLOR_VARIANT);
    colorSignalCounter->assertPayloads({ALTERNATE_COLOR});

    colorSettingsItem.set(oldValue);

    TS_ASSERT_EQUALS(facade->value(COLOR_KEY), COLOR_VARIANT);
    colorSignalCounter->assertPayloads({ALTERNATE_COLOR, COLOR_VALUE});
  }

  template<typename T>
  void performFontSetterTest(const T& newValue, const T& oldValue) {
    FontSettingsItem fontSettingsItem(FONT_KEY, facade);
    QObject::connect(&fontSettingsItem, SIGNAL(updated(QFont)), fontSignalCounter, SLOT(record(QFont)));
    fontSignalCounter->callback =
        [&] (const QFont&) { fontSignalCounter->callback = nullptr; fontSettingsItem.set(newValue); };

    fontSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(FONT_KEY), ALTERNATE_FONT_VARIANT);
    fontSignalCounter->assertPayloads({ALTERNATE_FONT});

    fontSettingsItem.set(oldValue);

    TS_ASSERT_EQUALS(facade->value(FONT_KEY), FONT_VARIANT);
    fontSignalCounter->assertPayloads({ALTERNATE_FONT, FONT_VALUE});
  }

  template<typename T>
  void performStringListSetterTest(const T& newValue, const T& oldValue) {
    StringListSettingsItem stringListSettingsItem(STRING_LIST_KEY, facade);
    QObject::connect(&stringListSettingsItem, SIGNAL(updated(QStringList)), stringListSignalCounter, SLOT(record(QStringList)));
    stringListSignalCounter->callback = [&] (const QStringList&) { stringListSignalCounter->callback = nullptr; stringListSettingsItem.set(newValue); }; // TODO why?

    stringListSettingsItem.set(newValue);

    QS_ASSERT_EQUALS(facade->value(STRING_LIST_KEY), ALTERNATE_STRING_LIST_VARIANT);
    stringListSignalCounter->assertPayloads({ALTERNATE_STRING_LIST});

    stringListSettingsItem.set(oldValue);

    QS_ASSERT_EQUALS(facade->value(STRING_LIST_KEY), STRING_LIST_VARIANT);
    stringListSignalCounter->assertPayloads({ALTERNATE_STRING_LIST, STRING_LIST_VALUE});
  }

  void assertWritingXml(const SettingsItem &item, const QString &expectedXml) {
    QString actualXml;
    QXmlStreamWriter writer(&actualXml);
    item.writeXml(writer);
    QS_ASSERT_EQUALS(actualXml, expectedXml);
  }

  template<typename ITEM, typename T>
  void assertReadingXml(ITEM &item, const QString &xml, const T &expected) {
    QXmlStreamReader reader(xml);
    reader.readNextStartElement();
    item.readXml(reader);
    QS_ASSERT_EQUALS(item.get(), expected);
  }

public:
  void setUp() {
    facade = SettingsFacade::transientSettings();
    facade->setValue(DOUBLE_KEY, DOUBLE_VALUE);
    facade->setValue(BOOL_KEY, BOOL_VALUE);
    facade->setValue(COLOR_KEY, COLOR_VALUE);
    facade->setValue(FONT_KEY, FONT_VALUE);
    facade->setValue(STRING_LIST_KEY, STRING_LIST_VALUE);
    emptyFacade = SettingsFacade::transientSettings(facade);
    emptyFacade->setValue(BOOL_KEY, ALTERNATE_BOOL);

    qrealSignalCounter = new QRealSignalCounter(facade);
    boolSignalCounter = new BoolSignalCounter(facade);
    colorSignalCounter = new ColorSignalCounter(facade);
    fontSignalCounter = new FontSignalCounter(facade);
    stringListSignalCounter = new StringListSignalCounter(facade);
  }

  void tearDown() {
    delete facade;
  }

  void testObtainingDoubleValue() {
    TS_ASSERT_EQUALS(DoubleSettingsItem(DOUBLE_KEY, facade).get(), DOUBLE_VALUE);
    QS_ASSERT_EQUALS(DoubleSettingsItem(DOUBLE_KEY, facade).getVariant(), DOUBLE_VARIANT);
    QS_ASSERT_EQUALS(DoubleSettingsItem(DOUBLE_KEY, facade).serialize(), DOUBLE_AS_STRING);
  }

  void testSettingDoubleValue() {
    performDoubleSetterTest(ALTERNATE_DOUBLE, DOUBLE_VALUE);
  }

  void testSettingDoubleString() {
    performDoubleSetterTest(ALTERNATE_DOUBLE_AS_STRING, DOUBLE_AS_STRING);
  }

  void testSettingDoubleVariant() {
    performDoubleSetterTest(ALTERNATE_DOUBLE_VARIANT, DOUBLE_VARIANT);
  }

  void testWritingDoubleToXml() {
    assertWritingXml(DoubleSettingsItem(DOUBLE_KEY, facade), DOUBLE_XML);
  }

  void testReadingDoubleFromXml() {
    DoubleSettingsItem item(DOUBLE_KEY, emptyFacade);
    assertReadingXml(item, DOUBLE_XML, DOUBLE_VALUE);
  }

  void testObtainingBoolValue() {
    TS_ASSERT_EQUALS(BoolSettingsItem(BOOL_KEY, facade).get(), BOOL_VALUE);
    QS_ASSERT_EQUALS(BoolSettingsItem(BOOL_KEY, facade).getVariant(), BOOL_VARIANT);
    QS_ASSERT_EQUALS(BoolSettingsItem(BOOL_KEY, facade).serialize(), BOOL_AS_STRING);
  }

  void testSettingBoolValue() {
    performBoolSetterTest(ALTERNATE_BOOL, BOOL_VALUE);
  }

  void testSettingBoolString() {
    performBoolSetterTest(ALTERNATE_BOOL_AS_STRING, BOOL_AS_STRING);
  }

  void testSettingBoolVariant() {
    performBoolSetterTest(ALTERNATE_BOOL_VARIANT, BOOL_VARIANT);
  }

  void testWritingBoolToXml() {
    assertWritingXml(BoolSettingsItem(BOOL_KEY, facade), BOOL_XML);
  }

  void testReadingBoolFromXml() {
    BoolSettingsItem item(BOOL_KEY, emptyFacade);
    assertReadingXml(item, BOOL_XML, BOOL_VALUE);
  }

  void testObtainingColorValue() {
    TS_ASSERT_EQUALS(ColorSettingsItem(COLOR_KEY, facade).get(), COLOR_VALUE);
    QS_ASSERT_EQUALS(ColorSettingsItem(COLOR_KEY, facade).getVariant(), COLOR_VARIANT);
    QS_ASSERT_EQUALS(ColorSettingsItem(COLOR_KEY, facade).serialize(), COLOR_AS_STRING);
  }

  void testSettingColorValue() {
    performColorSetterTest(ALTERNATE_COLOR, COLOR_VALUE);
  }

  void testSettingColorString() {
    performColorSetterTest(ALTERNATE_COLOR_AS_STRING, COLOR_AS_STRING);
  }

  void testSettingColorVariant() {
    performColorSetterTest(ALTERNATE_COLOR_VARIANT, COLOR_VARIANT);
  }

  void testWritingColorToXml() {
    assertWritingXml(ColorSettingsItem(COLOR_KEY, facade), COLOR_XML);
  }

  void testReadingColorFromXml() {
    ColorSettingsItem item(COLOR_KEY, emptyFacade);
    assertReadingXml(item, COLOR_XML, COLOR_VALUE);
  }

  void testObtainingFontValue() {
    QS_ASSERT_EQUALS(FontSettingsItem(FONT_KEY, facade).get(), FONT_VALUE);
    QS_ASSERT_EQUALS(FontSettingsItem(FONT_KEY, facade).getVariant(), FONT_VARIANT);
    QS_ASSERT_EQUALS(FontSettingsItem(FONT_KEY, facade).serialize(), FONT_AS_STRING);
  }

  void testSettingFontValue() {
    performFontSetterTest(ALTERNATE_FONT, FONT_VALUE);
  }

  void testSettingFontString() {
    performFontSetterTest(ALTERNATE_FONT_AS_STRING, FONT_AS_STRING);
  }

  void testSettingFontVariant() {
    performFontSetterTest(ALTERNATE_FONT_VARIANT, FONT_VARIANT);
  }

  void testWritingFontToXml() {
    assertWritingXml(FontSettingsItem(FONT_KEY, facade), FONT_XML);
  }

  void testReadingFontFromXml() {
    FontSettingsItem item(FONT_KEY, emptyFacade);
    assertReadingXml(item, FONT_XML, FONT_VALUE);
  }

  void testObtainingStringListValue() {
    QS_ASSERT_EQUALS(StringListSettingsItem(STRING_LIST_KEY, facade).get(), STRING_LIST_VALUE);
    QS_ASSERT_EQUALS(StringListSettingsItem(STRING_LIST_KEY, facade).getVariant(), STRING_LIST_VARIANT);
    QS_ASSERT_EQUALS(StringListSettingsItem(STRING_LIST_KEY, facade).serialize(), STRING_LIST_AS_STRING);
  }

  void testSettingStringListValue() {
    performStringListSetterTest(ALTERNATE_STRING_LIST, STRING_LIST_VALUE);
  }

  void testSettingStringListString() {
    performStringListSetterTest(ALTERNATE_STRING_LIST_AS_STRING, STRING_LIST_AS_STRING);
  }

  void testSettingStringListVariant() {
    performStringListSetterTest(ALTERNATE_STRING_LIST_VARIANT, STRING_LIST_VARIANT);
  }

  void testWritingStringListToXml() {
    assertWritingXml(StringListSettingsItem(STRING_LIST_KEY, facade), STRING_LIST_XML);
  }

  void testReadingStringListFromXml() {
    StringListSettingsItem item(STRING_LIST_KEY, emptyFacade);
    assertReadingXml(item, STRING_LIST_XML, STRING_LIST_VALUE);
  }
};
