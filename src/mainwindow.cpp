#include <iostream>
/* -- */
#include <QList>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QResizeEvent>
/* -- */
#include "core/Perso.hpp"
#include "core/player.hpp"
#include "core/leveldata.hpp"
/* -- */
#include "core/map_data/modelworld.h"
/* -- */
#include "scene/graphictile.hpp"
#include "scene/graphicsscene.hpp"
#include "scene/statsscene.hpp"
#include "scene/fightscene.hpp"
/* -- */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow), _current_scene(NULL)
{
    ui->setupUi(this);

    ui->main_view->setMouseTracking(true);

    setMinimumSize(800, 600);

    FightScene *fight_scene = new FightScene();
    _scenes[FIGHT_SCENE] = fight_scene;
    connect(fight_scene, SIGNAL(signal_end_fight()), this, SLOT(slot_end_fight()));

    StatsScene *stat_scene = new StatsScene();
    _scenes[STATS_SCENE] = stat_scene;
    connect(stat_scene, SIGNAL(signal_hide_stats()), this, SLOT(slot_hide_stats()));
}

MainWindow::~MainWindow()
{
    delete ui;
    foreach(QGraphicsScene *scene, _scenes)
        delete scene;

    _scenes.clear();
    _current_scene = NULL;
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    std::cout << "resize window " << event->size().width() << " " << event->size().height() << std::endl;
}

void MainWindow::on_action_new_game_triggered() {
    qDebug("New game");
    load_map("1");
}

void MainWindow::on_action_load_game_triggered() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Load map"),
                                         tr("map id:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        load_map(text);
    }
}

void MainWindow::load_map(const QString &world_name) {
    delete _scenes[LEVEL_MAP];

    _current_scene = new GraphicsScene();
    ui->main_view->setScene(_current_scene);

    _scenes[LEVEL_MAP] = _current_scene;

    connect((GraphicsScene*)_current_scene, SIGNAL(signal_begin_fight(Perso *, Perso *)), this, SLOT(slot_begin_fight(Perso *, Perso *)));
    connect((GraphicsScene*)_current_scene, SIGNAL(signal_show_stats()), this, SLOT(slot_show_stats()));

    ModelWorld *mw = new ModelWorld("World.xml");
    try {
        QList <Player *> players;
        // Load players
        temporary_load_human_player(players);

        LevelData *mapData = new LevelData(players, world_name);

        const std::map<QString, QSharedPointer<ModelArea> > &model_area = mw->get_modelarea_map();

        // If map not found, throw an error
        if(model_area.find(world_name) == model_area.end()) {
            throw ("map " + world_name + " not found");
        }
        else {
            mapData->set_model_area(model_area.at(world_name));
        }

        ((GraphicsScene*)_current_scene)->create_world(mapData);

        // Store players to show their stats
        StatsScene *stat_scene = dynamic_cast<StatsScene *>(_scenes[STATS_SCENE]);
        stat_scene->set_players(players);

        //
        foreach(Player *player, players) {
            connect(player, SIGNAL(signal_player_has_lost(Player*)), this, SLOT(slot_player_has_lost(Player *)));
        }
    }
    catch (const QString &e) {
        QMessageBox::critical(this, "Critical error occured", e);
    }
    delete mw;
}

void MainWindow::temporary_load_human_player(QList <Player *> &players) {
    QList<Perso *> persos;
    // Player 1
    Perso *obj = new Perso("chun", 0);
    obj->set_position(Position(1, 4, 0));
    obj->set_HP(10);
    obj->set_max_HP(15);
    obj->set_MP(12);
    obj->set_max_MP(14);
    obj->set_strength(5);
    obj->set_mobility(6);
    persos.push_back(obj);
    obj = new Perso("kyle", 0);
    obj->set_position(Position(2, 4, 0));
    obj->set_HP(15);
    obj->set_max_HP(25);
    obj->set_MP(22);
    obj->set_max_MP(24);
    obj->set_mobility(4);
    obj->set_strength(21);
    persos.push_back(obj);
    obj = new Perso("ryan", 0);
    obj->set_position(Position(3, 4, 0));
    obj->set_HP(20);
    obj->set_max_HP(20);
    obj->set_MP(10);
    obj->set_max_MP(30);
    obj->set_mobility(5);
    obj->set_strength(6);
    persos.push_back(obj);

    Player *p1 = new Player(0);
    p1->set_persos(persos);

    players.push_back(p1);
}

void MainWindow::slot_begin_fight(Perso *yours, Perso *opponent)
{
    qDebug("Switch to fight scene!");
    FightScene *scene = dynamic_cast<FightScene *>(_scenes[FIGHT_SCENE]);
    _current_scene = scene;
    ui->main_view->setScene(_current_scene);

    scene->begin_fight(yours, opponent);
}

void MainWindow::slot_end_fight()
{
    // Get back to level scene
    _current_scene = _scenes[LEVEL_MAP];
    ui->main_view->setScene(_current_scene);
}

void MainWindow::slot_show_stats()
{
    // Get back to level scene
    _current_scene = _scenes[STATS_SCENE];
    ui->main_view->setScene(_current_scene);
    StatsScene *stat_scene = dynamic_cast<StatsScene *>(_scenes[STATS_SCENE]);
    stat_scene->refresh_stats();
}

void MainWindow::slot_hide_stats()
{
    // Get back to level scene
    _current_scene = _scenes[LEVEL_MAP];
    ui->main_view->setScene(_current_scene);
}

void MainWindow::slot_player_has_lost(Player *p)
{
    if(p->get_id() == 0) {
        qDebug("You lost, TODO Display game over!");
    }
    else {
        qDebug("You won this level, Display town to buy weapons!");
    }
}
