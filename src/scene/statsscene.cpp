
#include <QKeyEvent>
/* -- */
#include "statsscene.hpp"

StatsScene::StatsScene()
{
    setBackgroundBrush(QBrush(Qt::red));
}

void StatsScene::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
    case Qt::Key_S:
         emit signal_hide_stats();
    }
}
