#ifndef UTILITIES_H
#define UTILITIES_H

#include <QDebug>
#include <QString>
#include <cxxtest/TestSuite.h>
#include <QTest>

class QTableView;
class QLineEdit;
class QCheckBox;

#define QS_ASSERT_EQUALS(VAL1,VAL2) {QString __comparison; QDebug __out(&__comparison); __out << VAL1; __comparison += "!= "; __out << VAL2; TSM_ASSERT_EQUALS(__comparison.toStdString().data(), VAL1, VAL2)}

#define QSM_ASSERT_EQUALS(MESSAGE, VAL1, VAL2) TSM_ASSERT_EQUALS((MESSAGE).toStdString().data(), VAL1, VAL2)

#define QS_ASSERT_ON_POINTER(CAST_TYPE, POINTER, METHOD, VALUE) {\
  TSM_ASSERT(#POINTER " is null", POINTER);\
  if (POINTER) {\
    TSM_ASSERT(#POINTER " is not of type " #CAST_TYPE, dynamic_cast<CAST_TYPE*>(POINTER));\
    CAST_TYPE *p = dynamic_cast<CAST_TYPE*>(POINTER);\
    if (p) QS_ASSERT_EQUALS(p->METHOD, VALUE)\
  }\
  }

void mouseMoveEvent(QWidget *widget, Qt::MouseButton button, Qt::KeyboardModifiers stateKey, QPoint pos, int delay=-1);

void enterDataIntoCell(QTableView *table, const QString& data, int row, int column);

void enterTextIntoInputWidget(QLineEdit *editor, const QString& text, int position = 0);

void clickCheckBox(QCheckBox* checkBox);

template<class T>
class ForTesting {
public:
  static int instanceCounter;
  ForTesting<T>() { ++ instanceCounter; }
  ~ForTesting<T>() { -- instanceCounter; }
};

// Don't forget to add instanceCounter to instanceCounter.cpp
#define CLASS_FOR_TESTING_WITH_FUNCTIONS(CLASSNAME, CONTENT) \
  class CLASSNAME##ForTesting : public CLASSNAME, public ForTesting<CLASSNAME>{ CONTENT };

#define CLASS_FOR_TESTING(CLASSNAME) CLASS_FOR_TESTING_WITH_FUNCTIONS(CLASSNAME,)

template<class T>
T* assertNotNull(T* pointer, QString message = "Should not be null") {
  TSM_ASSERT_DIFFERS(message.toStdString().data(), pointer, nullptr);
  if (!pointer) throw nullptr;
  return pointer;
}
void assertTrue(bool input, QString message = "Assertion not fulfilled!");

#endif // UTILITIES_H
