#ifndef GRAPHICSOBJECT_HPP
#define GRAPHICSOBJECT_HPP

#include <QGraphicsItemGroup>
#include <QTimer>

#include "util/direction.hpp"

class MoveAction;
class Perso;

/**
 * Contains the list of images for one direction
 * And the current image of the animation.
 */
struct ImageAnimation {
    int _current_img;
    QList<QGraphicsPixmapItem *> _items;
};


class GraphicsObject : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    GraphicsObject(Perso *obj);
    ~GraphicsObject();

    enum { Type = UserType + 4 };

    int type() const {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    void move_object_to(const QPointF &new_pos);

    bool has_moved() const;

    void set_selected(bool select);

    Perso *get_object() const;

private:
    // The perso data for this graphic object
    Perso *_perso;

    // For each direction, the corresponding images
    QMap<Direction, QSharedPointer<ImageAnimation> > _pixmaps;

    // The current pixmap displayed
    QGraphicsPixmapItem *_current_pixmap;

    // Corresponds to the icon for the status : red means the perso has moved else it is green
    QGraphicsEllipseItem *_status;

    // Box telling that the perso is selected
    QGraphicsRectItem *_selected_item_box;

    // Timer for the animation
    QTimer _move_timer;

    // Moves to do during the animation
    MoveAction *_actions;

public slots:
    // Slot call by the timer to update the animation
    void updateAnimation();

    /**
     * If has_moved == true, the perso has moved so we
     * put a red circle to the perso,
     * else we put a green one (meaning he can move)
     * @brief slot_perso_has_move
     * @param has_moved
     */
    void slot_perso_has_move(bool has_moved);

signals:
    // Signal throw when the animation is finished
    void signal_finish_moved(bool);

};

#endif // GRAPHICSOBJECT_HPP
