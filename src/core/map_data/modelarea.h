#ifndef MODELAREA_H
#define MODELAREA_H

#include <vector>
#include <map>
#include <QString>
#include <QSharedPointer>

class QDomElement;
class TileData;

class ModelArea
{
public:
    ModelArea( QDomElement & elt , std::map < QString, QSharedPointer<TileData> > & tiles_map );

    int get_width () const;
    int get_height () const;
    std::vector < std::vector<QSharedPointer<TileData> > > & get_tiles_grid();


protected:
    std::vector < std::vector<QSharedPointer<TileData> > > tiles_grid;
};

#endif // MODELAREA_H
