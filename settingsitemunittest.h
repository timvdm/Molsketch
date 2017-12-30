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

#include "signalcounter.h"

#include <cxxtest/TestSuite.h>
#include <settingsfacade.h>
#include <settingsitem.h>

using namespace Molsketch;

const QString DOUBLE_KEY("TestDoubleKey");
const qreal DOUBLE_VALUE = 3.75;
const QVariant DOUBLE_VARIANT(DOUBLE_VALUE);
const QString DOUBLE_AS_STRING("3.75");
const qreal ALTERNATE_DOUBLE = 4.5;
const QVariant ALTERNATE_DOUBLE_VARIANT(4.5);
const QString ALTERNATE_DOUBLE_AS_STRING("4.5");

const QString BOOL_KEY("TestBoolKey");
const bool BOOL_VALUE = false;
const QVariant BOOL_VARIANT(false);
const QString BOOL_AS_STRING("False");
const bool ALTERNATE_BOOL = true;
const QVariant ALTERNATE_BOOL_VARIANT(true);
const QString ALTERNATE_BOOL_AS_STRING("True");

const QString COLOR_KEY("TestColorKey");
const QColor COLOR_VALUE(Qt::red);
const QVariant COLOR_VARIANT(COLOR_VALUE);
const QString COLOR_AS_STRING("Af////8AAAAAAAA=");
const QColor ALTERNATE_COLOR(Qt::blue);
const QVariant ALTERNATE_COLOR_VARIANT(ALTERNATE_COLOR);
const QString ALTERNATE_COLOR_AS_STRING("Af//AAAAAP//AAA=");

const QString FONT_KEY("TestFontKey");
const QFont FONT_VALUE("Times", 10, QFont::Bold);
const QVariant FONT_VARIANT(FONT_VALUE);
const QString FONT_AS_STRING("AAAACgBUAGkAbQBlAHP/////QCQAAAAAAAD/////BQABAEsQAGQBAAAAAAAAAAAAAA==");
const QFont ALTERNATE_FONT("Helvetica", 15, QFont::Cursive);
const QVariant ALTERNATE_FONT_VARIANT(ALTERNATE_FONT);
const QString ALTERNATE_FONT_AS_STRING("AAAAEgBIAGUAbAB2AGUAdABpAGMAYf////9ALgAAAAAAAP////8FAAEABhAAZAEAAAAAAAAAAAAA");


class SettingsItemUnitTest : public CxxTest::TestSuite {
  SettingsFacade *facade;
  QRealSignalCounter *qrealSignalCounter;
  BoolSignalCounter *boolSignalCounter;
  ColorSignalCounter *colorSignalCounter;
  FontSignalCounter *fontSignalCounter;

  template<typename T>
  void performDoubleSetterTest(const T& newValue) {
    DoubleSettingsItem doubleSettingsItem(DOUBLE_KEY, facade);
    QObject::connect(&doubleSettingsItem, SIGNAL(updated(qreal)), qrealSignalCounter, SLOT(record(qreal)));

    doubleSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(DOUBLE_KEY), ALTERNATE_DOUBLE_VARIANT);
    qrealSignalCounter->assertPayloads({ALTERNATE_DOUBLE});
  }

  template<typename T>
  void performBoolSetterTest(const T& newValue) {
    BoolSettingsItem boolSettingsItem(BOOL_KEY, facade);
    QObject::connect(&boolSettingsItem, SIGNAL(updated(bool)), boolSignalCounter, SLOT(record(bool)));

    boolSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(BOOL_KEY), ALTERNATE_BOOL_VARIANT);
    boolSignalCounter->assertPayloads({ALTERNATE_BOOL});
  }

  template<typename T>
  void performColorSetterTest(const T& newValue) {
    ColorSettingsItem colorSettingsItem(COLOR_KEY, facade);
    QObject::connect(&colorSettingsItem, SIGNAL(updated(QColor)), colorSignalCounter, SLOT(record(QColor)));

    colorSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(COLOR_KEY), ALTERNATE_COLOR_VARIANT);
    colorSignalCounter->assertPayloads({ALTERNATE_COLOR});
  }

  template<typename T>
  void performFontSetterTest(const T& newValue) {
    FontSettingsItem fontSettingsItem(FONT_KEY, facade);
    QObject::connect(&fontSettingsItem, SIGNAL(updated(QFont)), fontSignalCounter, SLOT(record(QFont)));

    fontSettingsItem.set(newValue);

    TS_ASSERT_EQUALS(facade->value(FONT_KEY), ALTERNATE_FONT_VARIANT);
    fontSignalCounter->assertPayloads({ALTERNATE_FONT});
  }

public:
  void setUp() {
    facade = SettingsFacade::transientSettings();
    facade->setValue(DOUBLE_KEY, DOUBLE_VALUE);
    facade->setValue(BOOL_KEY, BOOL_VALUE);
    facade->setValue(COLOR_KEY, COLOR_VALUE);
    facade->setValue(FONT_KEY, FONT_VALUE);

    qrealSignalCounter = new QRealSignalCounter(facade);
    boolSignalCounter = new BoolSignalCounter(facade);
    colorSignalCounter = new ColorSignalCounter(facade);
    fontSignalCounter = new FontSignalCounter(facade);
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
    performDoubleSetterTest(ALTERNATE_DOUBLE);
  }

  void testSettingDoubleString() {
    performDoubleSetterTest(ALTERNATE_DOUBLE_AS_STRING);
  }

  void testSettingDoubleVariant() {
    performDoubleSetterTest(ALTERNATE_DOUBLE_VARIANT);
  }

  void testObtainingBoolValue() {
    TS_ASSERT_EQUALS(BoolSettingsItem(BOOL_KEY, facade).get(), BOOL_VALUE);
    QS_ASSERT_EQUALS(BoolSettingsItem(BOOL_KEY, facade).getVariant(), BOOL_VARIANT);
    QS_ASSERT_EQUALS(BoolSettingsItem(BOOL_KEY, facade).serialize(), BOOL_AS_STRING);
  }

  void testSettingBoolValue() {
    performBoolSetterTest(ALTERNATE_BOOL);
  }

  void testSettingBoolString() {
    performBoolSetterTest(ALTERNATE_BOOL_AS_STRING);
  }

  void testSettingBoolVariant() {
    performBoolSetterTest(ALTERNATE_BOOL_VARIANT);
  }

  void testObtainingColorValue() {
    TS_ASSERT_EQUALS(ColorSettingsItem(COLOR_KEY, facade).get(), COLOR_VALUE);
    QS_ASSERT_EQUALS(ColorSettingsItem(COLOR_KEY, facade).getVariant(), COLOR_VARIANT);
    QS_ASSERT_EQUALS(ColorSettingsItem(COLOR_KEY, facade).serialize(), COLOR_AS_STRING);
  }

  void testSettingColorValue() {
    performColorSetterTest(ALTERNATE_COLOR);
  }

  void testSettingColorString() {
    performColorSetterTest(ALTERNATE_COLOR_AS_STRING);
  }

  void testSettingColorVariant() {
    performColorSetterTest(ALTERNATE_COLOR_VARIANT);
  }

  void testObtainingFontValue() {
    QS_ASSERT_EQUALS(FontSettingsItem(FONT_KEY, facade).get(), FONT_VALUE);
    QS_ASSERT_EQUALS(FontSettingsItem(FONT_KEY, facade).getVariant(), FONT_VARIANT);
    QS_ASSERT_EQUALS(FontSettingsItem(FONT_KEY, facade).serialize(), FONT_AS_STRING);
  }

  void testSettingFontValue() {
    performFontSetterTest(ALTERNATE_FONT);
  }

  void testSettingFontString() {
    performFontSetterTest(ALTERNATE_FONT_AS_STRING);
  }

  void testSettingFontVariant() {
    performFontSetterTest(ALTERNATE_FONT_VARIANT);
  }
};
