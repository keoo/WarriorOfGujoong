#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GraphicsScene;
class PersoStatistics;
class Player;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    // Scene where the game takes place. The view used is ui->main_view
    GraphicsScene *_scene;

    // Show view where statistics on perso are printed
    PersoStatistics *_stats_view;

    // Load players depending on the fact that it is a new game or a continued game
    // (persos could not be default ones (lvl up, equipment changed, deads...))
    void temporary_load_human_player(QList<Player *> &players);

    void load_map(const QString &world_name);

protected slots:
    void on_action_new_game_triggered();
    void on_action_load_game_triggered();
};

#endif // MAINWINDOW_H
