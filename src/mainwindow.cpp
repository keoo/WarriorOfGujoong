#include <QList>
#include <QMessageBox>
/* -- */
#include "core/Perso.hpp"
#include "core/player.hpp"
#include "core/leveldata.hpp"
/* -- */
#include "modelworld.h"
#include "scene/graphictile.hpp"
#include "scene/graphicsscene.hpp"
/* -- */
#include "persostatistics.hpp"
/* -- */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow), _scene(new GraphicsScene())
{
    ui->setupUi(this);

    ui->main_view->setMouseTracking(true);

    ui->main_view->setScene(_scene);

    setMinimumSize(800, 600);

    /*
 *_stats_view = new PersoStatistics(parent);

    _stats_view->setVisible(false);
    */

    //connect(_scene, SIGNAL(signal_perso_mouse_hovered(/*Perso **/)), _stats_view, SLOT(slot_show_view(/*Perso **/)));
    //connect(_scene, SIGNAL(signal_perso_mouse_quit_hovered()), _stats_view, SLOT(slot_hide_view()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
}

void MainWindow::on_action_load_game_triggered() {

    qDebug("Load a game");
    ModelWorld *mw = new ModelWorld("World.xml");
    try {

        QList<Perso *> objects;
        Perso *obj = new Perso();
        obj->set_name("/tmp/WarriorOfGujoong-tiles/persos/chun");
        obj->set_position(Position(1, 4, 0));
        obj->set_mobility(6);
        obj->slot_reset_has_moved();
        objects.push_back(obj);
        obj = new Perso();
        obj->set_name("/tmp/WarriorOfGujoong-tiles/persos/kyle");
        obj->set_position(Position(2, 4, 0));
        obj->set_mobility(4);
        obj->slot_reset_has_moved();
        objects.push_back(obj);
        obj = new Perso();
        obj->set_name("/tmp/WarriorOfGujoong-tiles/persos/ryan");
        obj->set_position(Position(3, 4, 0));
        obj->set_mobility(5);
        obj->slot_reset_has_moved();
        objects.push_back(obj);

        Player *p1 = new Player;
        p1->set_persos(objects);

        objects.clear();
        obj = new Perso();
        obj->set_name("/tmp/WarriorOfGujoong-tiles/persos/unknown");
        obj->set_position(Position(17, 4, 0));
        obj->slot_reset_has_moved();
        obj->set_mobility(8);
        objects.push_back(obj);
        Player *p2 = new Player;
        p2->set_persos(objects);

        QList <Player *> players;
        players.push_back(p1);
        players.push_back(p2);

        LevelData *mapData = new LevelData(players, "1");


        const std::map<QString, QSharedPointer<ModelArea> > &model_area = mw->get_modelarea_map();
        const QString &world_name = mapData->get_map_id();

        if(model_area.find(world_name) == model_area.end()) {
            throw ("map " + world_name + " not found");
        }
        else {
            mapData->set_model_area(model_area.at(world_name));
        }

        _scene->create_world(mapData);
    }
    catch (const QString &e) {
        QMessageBox::critical(this, "Critical error occured", e);
    }
    delete mw;
}
