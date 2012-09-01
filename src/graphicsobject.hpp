#ifndef GRAPHICSOBJECT_HPP
#define GRAPHICSOBJECT_HPP

#include <QGraphicsPixmapItem>

#include "core/WGObject.hpp"

class GraphicsObject : public QGraphicsPixmapItem
{
public:
    GraphicsObject(WGObject *obj, const QPixmap &pixmap);

    enum { Type = UserType + 4 };

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    void move_object_to(const QPointF &new_pos);

private:
    WGObject *_obj;
};

#endif // GRAPHICSOBJECT_HPP
