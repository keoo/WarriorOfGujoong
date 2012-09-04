#include "computemoves.hpp"

ComputeMoves::ComputeMoves()
{
}

MoveAction *ComputeMoves::create_moves(const QPointF &pos_init, const QPointF &pos_fin)
{
    MoveAction *mv_action =  new MoveAction();
    QList <Move *> *moves = new QList<Move*>();
    mv_action->set_moves(moves);
    // Go horizontally first then vertically
    const int mv_step = 32/2;
    int nb_move_left = (pos_init.x() - pos_fin.x())/mv_step;

    if(nb_move_left > 0) {
        for (int i = pos_init.x(); i > pos_fin.x(); i-=mv_step) {
            QPointF begin(i, pos_init.y());
            QPointF end((i-mv_step), pos_init.y());
            Move *mv = new Move;
            mv->pos_init = begin;
            mv->pos_final = end;
            mv->sens = LEFT;
            moves->push_back(mv);
        }
    }
    else if(nb_move_left < 0) {
        for (int i = pos_init.x(); i < pos_fin.x() ; i+=mv_step) {
            QPointF begin(i, pos_init.y());
            QPointF end((i+mv_step), pos_init.y());
            Move *mv = new Move;
            mv->pos_init = begin;
            mv->pos_final = end;
            mv->sens = RIGHT;
            moves->push_back(mv);
        }
    }

    int nb_move_top = (pos_init.y() - pos_fin.y())/mv_step;

    if(nb_move_top > 0) {
        for (int i = pos_init.y(); i > pos_fin.y(); i-=mv_step) {
            QPointF begin(pos_fin.x(), i);
            QPointF end(pos_fin.x(), (i-mv_step));
            Move *mv = new Move;
            mv->pos_init = begin;
            mv->pos_final = end;
            mv->sens = TOP;
            moves->push_back(mv);
        }
    }
    else if(nb_move_top < 0) {
        for (int i = pos_init.y(); i < pos_fin.y() ; i+=mv_step) {
            QPointF begin(pos_fin.x(), i);
            QPointF end(pos_fin.x(), (i+mv_step));
            Move *mv = new Move;
            mv->pos_init = begin;
            mv->pos_final = end;
            mv->sens = BOTTOM;
            moves->push_back(mv);
        }
    }


    return mv_action;

}

void ComputeMoves::release_moves(MoveAction *mv_action)
{
    foreach (Move *mv, *(mv_action->get_moves())) {
        delete mv;
    }
    delete mv_action->get_moves();
    delete mv_action;

}
