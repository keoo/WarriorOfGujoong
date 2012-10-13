#include <iostream>
/* -- */
#include <QVector>
/* -- */
#include "modelworld.h"
/* -- */
#include "core/Perso.hpp"
/* -- */
#include "scene/graphicsobject.hpp"
#include "scene/graphictile.hpp"
/* -- */
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

struct TileMM {
    int x;
    int y;
    int mob;
    TileMM *prev;
};

#define INFINITY 100000

void ComputeMoves::compute_visibility(QVector<QVector<QSharedPointer<GraphicTile> > > &area, const GraphicsObject *perso, const QVector<GraphicsObject *> &persos)
{
    const int map_width = area.size();
    const int map_height = area[0].size();

    std::vector < std::vector<TileMM *> > data;
    for(int i = 0 ; i < map_width ; ++ i) {
        std::vector <TileMM *> dataLine;
        for(int j = 0 ; j < map_height ; ++ j) {
            TileMM *tmpTile = new TileMM;
            dataLine.push_back(tmpTile);
            tmpTile->x = i;
            tmpTile->y = j;
            tmpTile->mob = INFINITY;
            tmpTile->prev = NULL;
        }
        data.push_back(dataLine);
    }

    const int mobility = perso->get_object()->get_mobility();
    data[perso->get_object()->get_position().getX()][perso->get_object()->get_position().getY()]->mob = mobility;

    for (int nb = 0 ; nb < mobility+1 ; ++ nb) {
        // Do mobility+1 time the algo
        for (int i = 0 ; i < map_width ; i ++) {
            for (int j = 0 ; j < map_height ; j ++) {
                // If we can move from the case, we compute if it is better than the current
                if(data[i][j]->mob > 0) {
                    if(i-1 >= 0) { // Watch if can go to left case
                        if(1 /* FIXME can walk on case */ && data[i-1][j]->mob-1 > data[i][j]->mob) {
                            data[i-1][j]->mob = data[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            data[i-1][j]->prev = data[i][j];
                        }
                    }
                    if(i+1 < data.size()) { // Watch if can go to  right case
                        if(1 /* FIXME can walk on case */ && data[i+1][j]->mob-1 > data[i][j]->mob) {
                            data[i+1][j]->mob = data[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            data[i+1][j]->prev = data[i][j];
                        }
                    }
                    if(j-1 >= 0) { // Watch if can go to  top case
                        if(1 /* FIXME can walk on case */ && data[i][j-1]->mob-1 > data[i][j]->mob) {
                            data[i][j-1]->mob = data[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            data[i][j-1]->prev = data[i][j];
                        }
                    }
                    if(j+1 < data[i].size()) { // Watch if go to from right case
                        if(1 /* FIXME can walk on case */ && data[i][j+1]->mob-1 > data[i][j]->mob) {
                            data[i][j+1]->mob = data[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            data[i][j+1]->prev = data[i][j];
                        }
                    }
                }
            }
        }
    }

    // FIXME We can not move on other persos, so we have to remove these cases

    // Do it on cases but then do it on model with signals send to graphic tile
    for (int ik = 0 ; ik < map_width ; ik ++) {
        for (int jk = 0 ; jk < map_height ; jk ++) {
            // Set visibility on cases
            area[ik][jk].data()->set_walkable(data[ik][jk]->mob >= 0 && data[ik][jk]->mob != INFINITY);
        }
    }

    // TODO Store the data and do not forget to delete it !
}

