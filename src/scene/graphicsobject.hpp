#ifndef GRAPHICSOBJECT_HPP
#define GRAPHICSOBJECT_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QTimer>

#include "computemoves.hpp"

#include "core/WGObject.hpp"

class GraphicsObject : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    GraphicsObject(WGObject *obj, const QPixmap &pixmap);
    ~GraphicsObject();

    enum { Type = UserType + 4 };

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    void move_object_to(const QPointF &new_pos);

    bool has_moved() const;

    void set_selected(bool select);

    MoveAction *_actions;

private:
    WGObject *_obj;

    QGraphicsPixmapItem *_pixmap;

    // has move
    QGraphicsEllipseItem *_status;

    QGraphicsRectItem *_selected_item_box;

    bool _has_move;

    std::string _current_image_name;

    QTimer _move_timer;

public slots:
    void updateAnimation();

    /**
     * If has_moved == true, the perso has moved so we
     * put a red circle to the perso,
     * else we put a green one (meaning he can move)
     * @brief slot_perso_has_move
     * @param has_moved
     */
    void slot_perso_has_move(bool has_moved);

};

#endif // GRAPHICSOBJECT_HPP
