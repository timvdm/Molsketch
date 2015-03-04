#include "arrowtypewidget.h"
#include "arrow.h"

namespace Molsketch {

#define TIPGEN(TYPE, IMAGESTRING) << typeIconPair(TYPE, QPixmap::fromImage(QImage(":/images/" + QString(IMAGESTRING) + ".png")))

  arrowTypeWidget::arrowTypeWidget(QWidget *parent)
    : ItemTypeWidget(parent)
  {
    setButtons(QList<typeIconPair>()
               TIPGEN(Arrow::NoArrow, "line") // --
               TIPGEN(Arrow::UpperForward | Arrow::LowerForward, "arrow") // ->
               TIPGEN(Arrow::UpperForward | Arrow::LowerForward | Arrow::UpperBackward | Arrow::LowerBackward, "resonanceArrow") // <->
               TIPGEN(Arrow::UpperForward, "hookUp") // -P
               TIPGEN(Arrow::LowerForward, "hookDown") // -b
               TIPGEN(Arrow::UpperForward | Arrow::UpperBackward, "eqHook") //
               TIPGEN(Arrow::UpperForward | Arrow::LowerBackward, "eqMirrorHook")
               TIPGEN(Arrow::LowerForward | Arrow::UpperBackward, "eqMirrorHookb")
               );
  }

} // namespace Molsketch

