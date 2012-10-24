#ifndef TILEDATA_HPP
#define TILEDATA_HPP

#include <QObject>
#include <QPixmap>

class TileData : public QObject
{
    Q_OBJECT

public:
    TileData(QPixmap *pixmap, bool walkable=true);
    TileData(const TileData &data);

    QPixmap &get_pixmap();
    bool is_walkable();

    void set_walkable_for_action(bool walk);
    bool is_walkable_for_action();

    const TileData &operator =(const TileData &data);

private:
    QPixmap *_pixmap;

    bool _is_walkable;

    bool _is_walkable_for_action;

signals:
    void signal_walkable_for_action(bool);

};

#endif // TILEDATA_HPP
