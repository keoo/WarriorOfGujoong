#ifndef COMPUTEMOVES_HPP
#define COMPUTEMOVES_HPP

#include <QPointF>
#include <QList>

#include "modelarea.h"
#include "util/direction.hpp"

class GraphicsObject;

struct Move {
    QPointF pos_init;
    QPointF pos_final;
    Direction sens;
};

class MoveAction {
public:
    MoveAction() : _current_move(0){}

    void set_moves(QList<Move *> *moves) { _moves = moves; }
    QList<Move *> *get_moves() { return _moves; }
    qreal &get_current_move() { return _current_move; }
private:
    QList <Move *> *_moves;
    qreal _current_move;
};

class ComputeMoves
{
private:
    ComputeMoves();

public:
    static MoveAction *create_moves(const QPointF &pos_init, const QPointF &pos_fin);

    static void release_moves(MoveAction *mv_action);

    static void compute_visibility(QVector<QVector<QSharedPointer<GraphicTile> > > &map, const GraphicsObject *perso, const QVector<GraphicsObject *> &persos);
    // FIXME Do it from model data !
    //    static void compute_visibility(QSharedPointer<ModelArea> &map, const GraphicsObject *perso, const QVector<GraphicsObject *> &persos);

};

#endif // COMPUTEMOVES_HPP
