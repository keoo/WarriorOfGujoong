#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GraphicsScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    /**
     * Scene where the game takes place. The view used is ui->main_view
     */
    GraphicsScene *_scene;

protected slots:
    void on_action_load_game_triggered();

};

#endif // MAINWINDOW_H
