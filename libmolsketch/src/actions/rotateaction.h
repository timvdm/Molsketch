#ifndef MOLSKETCH_ROTATEACTION_H
#define MOLSKETCH_ROTATEACTION_H

#include "genericaction.h"

namespace Molsketch {
  class graphicsItem ;

  class transformAction : public genericAction
  {
    Q_OBJECT
  public:
    explicit transformAction(MolScene* scene);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) ;

  protected:
    virtual QTransform generateTransform(const QLineF& originalLine, const QLineF& currentLine) = 0;
    virtual void snapLine(const QLineF &originalLine, QLineF& currentLine) = 0;
    virtual QString cursorLabel(const QLineF &originalLine, QLineF& currentLine);
  private:
    class privateData;
    privateData *d;
  };

  class rotateAction : public transformAction
  {
    Q_OBJECT
  public:
    explicit rotateAction(MolScene* scene) ;
  private:
    QTransform generateTransform(const QLineF &originalLine, const QLineF &currentLine) ;
    void snapLine(const QLineF &originalLine, QLineF &currentLine);
    QString cursorLabel(const QLineF &originalLine, QLineF &currentLine);
  };

  class translateAction : public transformAction
  {
    Q_OBJECT
  public:
    explicit translateAction(MolScene* scene) ;
  private:
    QTransform generateTransform(const QLineF &originalLine, const QLineF &currentLine) ;
    void snapLine(const QLineF &originalLine, QLineF &currentLine);
  };

} // namespace Molsketch

#endif // MOLSKETCH_ROTATEACTION_H
