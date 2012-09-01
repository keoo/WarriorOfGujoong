#include "graphicsobject.hpp"

GraphicsObject::GraphicsObject(WGObject *obj, const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap), _obj(obj)
{
}

void GraphicsObject::move_object_to(const QPointF &new_pos)
{
    setPos(new_pos);
}
