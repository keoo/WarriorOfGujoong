#include "graphicsscene.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow), _scene(new GraphicsScene())
{
    ui->setupUi(this);

    ui->main_view->setScene(_scene);

    setMinimumSize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
}

void MainWindow::on_action_load_game_triggered() {
    // Map *map = MapHandler::loadMap("default");
    // m_scene->setCurrentMap(map);
}
