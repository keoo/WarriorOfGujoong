#ifndef MODELAREA_H
#define MODELAREA_H

#include <vector>
#include <map>
#include <QString>
#include <QSharedPointer>

class QDomElement;
class QPixmap;

class ModelArea
{
public:
    ModelArea( QDomElement & elt , const std::map < QString, QSharedPointer<QPixmap> > & tiles_map );

    const int get_width () const;
    const int get_height () const;
    const std::vector < std::vector<QSharedPointer<QPixmap> > > & get_tiles_grid() const ;


protected:
    std::vector < std::vector<QSharedPointer<QPixmap> > > tiles_grid;
};

#endif // MODELAREA_H
