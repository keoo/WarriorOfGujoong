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
#include "scene/graphictile.hpp"
#include "scene/graphicsscene.hpp"
/* -- */
#include "persostatistics.hpp"
/* -- */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow), _scene(NULL)
{
    ui->setupUi(this);

    ui->main_view->setMouseTracking(true);

    setMinimumSize(800, 600);

    _stats_view = new PersoStatistics(parent);

    ui->horizontalLayout->addWidget(_stats_view);
    _stats_view->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    std::cout << "resize window " << event->size().width() << " " << event->size().height() << std::endl;
    _stats_view->setGeometry(0, 0, ui->main_view->size().width()/3, ui->main_view->size().height());
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
    delete _scene;
    _scene = new GraphicsScene();
    ui->main_view->setScene(_scene);

    //connect(_scene, SIGNAL(signal_perso_mouse_hovered(Perso *)), _stats_view, SLOT(slot_show_view(Perso *)));
    //connect(_scene, SIGNAL(signal_perso_mouse_quit_hovered()), _stats_view, SLOT(slot_hide_view()));

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

        _scene->create_world(mapData);
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
    obj->set_mobility(6);
    persos.push_back(obj);
    obj = new Perso("kyle", 0);
    obj->set_position(Position(2, 4, 0));
    obj->set_HP(15);
    obj->set_mobility(4);
    persos.push_back(obj);
    obj = new Perso("ryan", 0);
    obj->set_position(Position(3, 4, 0));
    obj->set_HP(20);
    obj->set_mobility(5);
    persos.push_back(obj);

    Player *p1 = new Player(0);
    p1->set_persos(persos);

    players.push_back(p1);
}
