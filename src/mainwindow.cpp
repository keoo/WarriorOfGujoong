#include "graphicsscene.hpp"
#include "modelworld.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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

    qDebug("Load a game");
    ModelWorld *mw = new ModelWorld("World.xml");
    try {
        _scene->create_world(mw, "1");
    }
    catch (const QString &e) {
        QMessageBox::critical(this, "Critical error occured", e);
    }
}
