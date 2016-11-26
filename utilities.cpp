#include "utilities.h"

void mouseMoveEvent(QWidget *widget, Qt::MouseButton button, Qt::KeyboardModifiers stateKey, QPoint pos, int delay) {
    QTEST_ASSERT(widget);

    if (pos.isNull())
        pos = widget->rect().center();

    if (delay > 0)
        QTest::qWait(delay);

    QTEST_ASSERT(stateKey == 0 || stateKey & Qt::KeyboardModifierMask);

    stateKey &= static_cast<unsigned int>(Qt::KeyboardModifierMask);

    QMouseEvent me(QEvent::MouseMove, pos, widget->mapToGlobal(pos), button, button, stateKey);
    QSpontaneKeyEvent::setSpontaneous(&me);
    if (!qApp->notify(widget, &me)) {
        QString warning = QString::fromLatin1("Mouse event \"MouseMove\" not accepted by receiving widget");
        QTest::qWarn(warning.toLatin1().data());
    }
}
