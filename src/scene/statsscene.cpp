#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QGraphicsGridLayout>
#include <QKeyEvent>
/* -- */
#include "core/Perso.hpp"
#include "core/player.hpp"
/* -- */
#include "statsscene.hpp"

StatsScene::StatsScene()
{
    setBackgroundBrush(QBrush(Qt::cyan));
}

void StatsScene::set_players(QList<Player *> &players)
{
    _players = players;
}

void StatsScene::refresh_stats()
{
    clear();
    _layouts.clear();

    qDebug("Refresh stats");
    foreach(Player *player, _players) {
        _layouts.push_back(createPlayerLayout(player));
    }
    int i = 0;
    foreach(QGraphicsGridLayout *layout, _layouts) {
        QGraphicsWidget *form = new QGraphicsWidget;
        form->setLayout(layout);
        addItem(form);
        form->setPos(i, 0);
        i += 200;
    }
}

void StatsScene::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
    case Qt::Key_S:
         emit signal_hide_stats();
    }
}

QGraphicsGridLayout *StatsScene::createPlayerLayout(Player *player)
{
    QGraphicsGridLayout *layout = new QGraphicsGridLayout();

    int counter = 0;
    foreach(Perso *perso, player->get_persos()) {
        QGraphicsProxyWidget *proxyLabel = new QGraphicsProxyWidget();

        QLabel *label = new QLabel();
        label->setText(QString::fromStdString(perso->get_name()) + QString(" lvl %1 HP : %2 / %3").arg(perso->get_level()).arg(perso->get_HP()).arg(perso->get_max_HP()));

        proxyLabel->setWidget(label);
        layout->addItem(proxyLabel, counter ++, 0);
    }

    return layout;
}
