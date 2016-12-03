#ifndef UTILITIES_H
#define UTILITIES_H

#include <QDebug>
#include <QString>
#include <cxxtest/TestSuite.h>
#include <QTest>

class QTableView;

#define QS_ASSERT_EQUALS(VAL1,VAL2) {QString __comparison; QDebug __out(&__comparison); __out << VAL1; __comparison += "!= "; __out << VAL2; TSM_ASSERT_EQUALS(__comparison.toStdString().data(), VAL1, VAL2)}

void mouseMoveEvent(QWidget *widget, Qt::MouseButton button, Qt::KeyboardModifiers stateKey, QPoint pos, int delay=-1);

void enterDataIntoCell(QTableView *table, const QString& data, int row, int column);

#endif // UTILITIES_H
