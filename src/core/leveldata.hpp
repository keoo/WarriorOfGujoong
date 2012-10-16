#ifndef LEVELDATA_HPP
#define LEVELDATA_HPP

#include <QObject>
#include <QSharedPointer>

class Player;
class ModelArea;

class LevelData : public QObject
{
    Q_OBJECT

public:
    LevelData(const QList<Player *> &players, const QString &map_area_id);

    ~LevelData();

    const QString &get_map_id() const;
    const QList<Player *> &get_players() const;

    int get_current_player();

    void set_model_area(const QSharedPointer<ModelArea> &model);
    QSharedPointer<ModelArea> &get_model_area();

private:
    QList<Player *>  _players;
    QString _map_id;
    int _current_player;

    QSharedPointer<ModelArea> _model_area;

public slots:
    void set_next_player();
};

#endif // LEVELDATA_HPP
