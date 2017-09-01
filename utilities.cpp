#include "utilities.h"

#include <QLineEdit>
#include <QTableView>

void mouseMoveEvent(QWidget *widget, Qt::MouseButton button, Qt::KeyboardModifiers stateKey, QPoint pos, int delay) {
    QTEST_ASSERT(widget);

    if (pos.isNull())
        pos = widget->rect().center();

    if (delay > 0)
        QTest::qWait(delay);

    QTEST_ASSERT(stateKey == 0 || stateKey & Qt::KeyboardModifierMask);

    stateKey &= static_cast<unsigned int>(Qt::KeyboardModifierMask);

    QMouseEvent me(QEvent::MouseMove, pos, widget->mapToGlobal(pos), button, Qt::NoButton, stateKey);
    QSpontaneKeyEvent::setSpontaneous(&me);
    if (!qApp->notify(widget, &me)) {
        QString warning = QString::fromLatin1("Mouse event \"MouseMove\" not accepted by receiving widget");
        QTest::qWarn(warning.toLatin1().data());
    }
}


void enterDataIntoCell(QTableView *table, const QString& data, int row, int column) {
  QPoint position(table->columnViewportPosition(column), table->rowViewportPosition(row));
  QTest::mouseClick(table->viewport(), Qt::LeftButton, Qt::NoModifier, position);
  QTest::mouseDClick(table->viewport(), Qt::LeftButton, Qt::NoModifier, position);
  QWidget* editor = table->viewport()->focusWidget();
  TS_ASSERT(editor);
  if (editor) {
    QTest::keyClicks(editor, data);
    QTest::keyClick(editor, Qt::Key_Tab); // TODO find out how to use Key_Return here
//      editor = table->viewport()->focusWidget(); // TODO
//      TS_ASSERT(!editor);
  }
}

void enterTextIntoInputWidget(QLineEdit *editor, const QString& text, int position) {
  QTest::mouseClick(editor, Qt::LeftButton);
  QTest::keyClick(editor, Qt::Key_Home);
  for (int i = 0 ; i < position ; ++i)
    QTest::keyClick(editor, Qt::Key_Right);
  QTest::keyClicks(editor, text);
}



void assertTrue(bool input, QString message) {
  TSM_ASSERT(message.toStdString().data(), input);
  if (!input) throw nullptr;
}

void clickCheckBox(QCheckBox *checkBox) {
  QTest::mouseClick((QWidget*) checkBox, Qt::LeftButton, Qt::NoModifier, QPoint(3,3)); // offset for QCheckBox
}
