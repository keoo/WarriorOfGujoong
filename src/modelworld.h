#ifndef MODELWORLD_H
#define MODELWORLD_H

#include <map>
#include <QSharedPointer>
#include <QString>

class QString;
class ModelArea;
class QPixmap;

class ModelWorld
{
public:
    ModelWorld(const QString & fileSource);

    const std::map < QString, QSharedPointer<QPixmap> > & get_tiles_map() const;

    const std::map < QString, QSharedPointer<ModelArea> > & get_modelarea_map() const;

protected:

    std::map < QString, QSharedPointer<QPixmap> > _tiles_map;
    std::map < QString, QSharedPointer<ModelArea> > _modelarea_map;
};

#endif // MODELWORLD_H
