#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class Perso;
class QGraphicsScene;
class Player;

namespace Ui {
class MainWindow;
}

enum SceneId
{
    LEVEL_MAP,
    FIGHT_SCENE,
    STATS_SCENE
};

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

    // All the scenes game. The view used is ui->main_view
    QMap<SceneId, QGraphicsScene *> _scenes;

    // Scene where the game takes place. The view used is ui->main_view
    QGraphicsScene *_current_scene;

    // Load players depending on the fact that it is a new game or a continued game
    // (persos could not be default ones (lvl up, equipment changed, deads...))
    void temporary_load_human_player(QList<Player *> &players);

    void load_map(const QString &world_name);

protected slots:
    void on_action_new_game_triggered();
    void on_action_load_game_triggered();

    void slot_begin_fight(Perso *yours, Perso *opponent);
    void slot_end_fight();

    void slot_show_stats();
    void slot_hide_stats();

    void slot_player_has_lost(Player *p);
};

#endif // MAINWINDOW_H
