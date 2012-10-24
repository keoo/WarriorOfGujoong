#ifndef COMPUTEMOVES_HPP
#define COMPUTEMOVES_HPP

#include <QPointF>
#include <QList>

#include "core/map_data/modelarea.h"
#include "util/direction.hpp"

class GraphicsObject;

struct Move {
    QPointF pos_init;
    QPointF pos_final;
    Direction sens;
};

struct TileMM {
    int x;
    int y;
    int mob;
    TileMM *prev;
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

    static std::vector < std::vector<TileMM *> > _current_moves;

public:
    static MoveAction *create_moves(const QPointF &begin_pos, const QPointF &end_pos);

    static void release_moves(MoveAction *mv_action);

    static void compute_visibility(QSharedPointer<ModelArea> &model, const GraphicsObject *perso, const QVector<GraphicsObject *> &persos);
};

#endif // COMPUTEMOVES_HPP
