#ifndef MOLSKETCH_DRAWACTION_H
#define MOLSKETCH_DRAWACTION_H

#include "genericaction.h"

namespace Molsketch {

        class drawAction : public genericAction
	{
          Q_OBJECT
	public:
                explicit drawAction(MolScene* scene);
		~drawAction();

                void mousePressEvent(QGraphicsSceneMouseEvent *event);
                void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
                void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
                void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); // TODO
                void leaveSceneEvent(QEvent *event) ;

        private:
                class privateData;
                privateData *d;
        private slots:
                void refreshIcon();
                void toggleVisibility(bool visible);
	};

} // namespace Molsketch

#endif // MOLSKETCH_DRAWACTION_H
