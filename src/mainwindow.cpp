#include "scene/graphicsscene.hpp"
#include "modelworld.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include "persostatistics.hpp"

// TMP wait for keoo
#include "core/WGObject.hpp"
// End TMP

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow), _scene(new GraphicsScene())
{
    ui->setupUi(this);

    ui->main_view->setMouseTracking(true);

    ui->main_view->setScene(_scene);

    ui->main_view->scale(2, 2);

    setMinimumSize(800, 600);

    _stats_view = new PersoStatistics(parent);
    _stats_view->setVisible(false);

    connect(_scene, SIGNAL(signal_perso_mouse_hovered(/*Perso **/)), _stats_view, SLOT(slot_show_view(/*Perso **/)));
    connect(_scene, SIGNAL(signal_perso_mouse_quit_hovered()), _stats_view, SLOT(slot_hide_view()));
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
        _scene->create_world(mw, "1");

        // TMP wait for keoo
        QVector<WGObject *> objects;
        WGObject *obj = new WGObject();
        obj->setName("/tmp/WarriorOfGujoong-tiles/fighter.png");
        obj->setPosition(Position(5, 5, 0));
        objects.push_back(obj);
        obj = new WGObject();
        obj->setName("/tmp/WarriorOfGujoong-tiles/princess.png");
        obj->setPosition(Position(6, 5, 0));
        objects.push_back(obj);
        _scene->add_objects(objects);
        // End TMP
    }
    catch (const QString &e) {
        QMessageBox::critical(this, "Critical error occured", e);
    }
}
