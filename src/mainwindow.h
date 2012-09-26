#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GraphicsScene;
class PersoStatistics;

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
    // Scene where the game takes place. The view used is ui->main_view
    GraphicsScene *_scene;

    // Show view where statistics on perso are printed
    PersoStatistics *_stats_view;

protected slots:
    void on_action_load_game_triggered();

};

#endif // MAINWINDOW_H
