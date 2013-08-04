#include <iostream>
/* -- */
#include <QVector>
/* -- */
#include "core/map_data/tiledata.hpp"
#include "core/map_data/modelworld.h"
/* -- */
#include "core/Perso.hpp"
/* -- */
#include "scene/graphicsobject.hpp"
/* -- */
#include "computemoves.hpp"

#define INFINITY 100000
#define TILE_SIZE 48

#define NB_STEP_BY_MV 2.

std::vector < std::vector<MoveToCase *> > ComputeMoves::_current_moves;

ComputeMoves::ComputeMoves()
{
}

MoveAction *ComputeMoves::create_moves(const QPointF &begin_pos, const QPointF &end_pos)
{
    int end_x = end_pos.x()/TILE_SIZE;
    int end_y = end_pos.y()/TILE_SIZE;
    int begin_x = begin_pos.x()/TILE_SIZE;
    int begin_y = begin_pos.y()/TILE_SIZE;

    MoveAction *mv_action =  new MoveAction();
    QList <Move *> *moves = new QList<Move*>();
    mv_action->set_moves(moves);
    // Go horizontally first then vertically
    const int mv_step = TILE_SIZE/NB_STEP_BY_MV;

    double current_case_x = end_x;
    double current_case_y = end_y;

    while(current_case_x != begin_x || current_case_y != begin_y) {
        MoveToCase *current_case = _current_moves[current_case_x][current_case_y];
        MoveToCase *previous_case = _current_moves[current_case_x][current_case_y]->prev;

        double previous_case_x = previous_case->x;
        double previous_case_y = previous_case->y;

        if(current_case->x < previous_case->x) { // Means previous case is the right one
            for(int i = 1 ; i < NB_STEP_BY_MV+1 ; i ++) {
                QPointF begin(previous_case_x*TILE_SIZE, previous_case_y*TILE_SIZE);
                QPointF end((current_case_x*TILE_SIZE+mv_step*i), current_case_y*TILE_SIZE);
                Move *mv = new Move;
                mv->pos_init = begin;
                mv->pos_final = end;
                mv->sens = LEFT;
                moves->push_front(mv);
                previous_case_x += 1/NB_STEP_BY_MV;
            }
        }
        else if(current_case->x > previous_case->x) { // Means previous case is the left one
            for(int i = 1 ; i < NB_STEP_BY_MV+1 ; i ++) {
                QPointF begin(previous_case_x*TILE_SIZE, previous_case_y*TILE_SIZE);
                QPointF end((current_case_x*TILE_SIZE-mv_step*i), current_case_y*TILE_SIZE);
                Move *mv = new Move;
                mv->pos_init = begin;
                mv->pos_final = end;
                mv->sens = RIGHT;
                moves->push_front(mv);
                previous_case_x -= 1/NB_STEP_BY_MV;
            }
        }
        else if(current_case->y < previous_case->y) { // Means previous case is the top one
            for(int i = 1 ; i < NB_STEP_BY_MV+1 ; i ++) {
                QPointF begin(previous_case_x*TILE_SIZE, previous_case_y*TILE_SIZE);
                QPointF end(current_case_x*TILE_SIZE, (current_case_y*TILE_SIZE+mv_step*i));
                Move *mv = new Move;
                mv->pos_init = begin;
                mv->pos_final = end;
                mv->sens = TOP;
                moves->push_front(mv);
                previous_case_y += 1/NB_STEP_BY_MV;
            }
        }
        else if(current_case->y > previous_case->y) { // Means previous case is the bottom one
            for(int i = 1 ; i < NB_STEP_BY_MV+1 ; i ++) {
                QPointF begin(previous_case_x*TILE_SIZE, previous_case_y*TILE_SIZE);
                QPointF end(current_case_x*TILE_SIZE, (current_case_y*TILE_SIZE-mv_step*i));
                Move *mv = new Move;
                mv->pos_init = begin;
                mv->pos_final = end;
                mv->sens = BOTTOM;
                moves->push_front(mv);

                previous_case_y -= 1/NB_STEP_BY_MV;
            }
        }

        current_case_x = previous_case->x;
        current_case_y = previous_case->y;
    }

    return mv_action;
}

void ComputeMoves::release_moves(MoveAction *mv_action)
{
    if(mv_action != NULL) {
        foreach (Move *mv, *(mv_action->get_moves())) {
            delete mv;
        }
        delete mv_action->get_moves();
        delete mv_action;
    }

    for(unsigned int i = 0 ; i < _current_moves.size() ; i ++) {
        for (unsigned int j = 0 ; j < _current_moves[i].size() ; j ++) {
            delete _current_moves[i][j];
            _current_moves[i][j] = NULL;
        }
        _current_moves[i].clear();
    }
    _current_moves.clear();
}

void ComputeMoves::compute_visibility(QSharedPointer<ModelArea> &model, const GraphicsObject *perso, const QVector<GraphicsObject *> &persos)
{
    std::vector < std::vector<QSharedPointer<TileData> > > &area = model.data()->get_tiles_grid();
    const int map_width = area.size();
    const int map_height = area[0].size();

    // Clean before computing
    release_moves(NULL);

    for(int i = 0 ; i < map_width ; ++ i) {
        std::vector <MoveToCase *> dataLine;
        for(int j = 0 ; j < map_height ; ++ j) {
            MoveToCase *tmpTile = new MoveToCase;
            dataLine.push_back(tmpTile);
            tmpTile->x = i;
            tmpTile->y = j;
            tmpTile->mob = INFINITY;
            tmpTile->prev = NULL;
        }
        _current_moves.push_back(dataLine);
    }

    const int mobility = perso->get_object()->get_mobility();
    _current_moves[perso->get_object()->get_position().getX()][perso->get_object()->get_position().getY()]->mob = mobility;

    for (int nb = 0 ; nb < mobility+1 ; ++ nb) {
        // Do mobility+1 time the algo
        for (int i = 0 ; i < map_width ; i ++) {
            for (int j = 0 ; j < map_height ; j ++) {

                // If we can move from the case, we compute if it is better than the current
                if(_current_moves[i][j]->mob > 0) {
                    if(i-1 >= 0) { // Watch if can go to left case
                        if(area[i-1][j].data()->is_walkable() && _current_moves[i-1][j]->mob-1 > _current_moves[i][j]->mob) {
                            _current_moves[i-1][j]->mob = _current_moves[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            _current_moves[i-1][j]->prev = _current_moves[i][j];
                        }
                    }
                    if(i+1 < _current_moves.size()) { // Watch if can go to  right case
                        if(area[i+1][j].data()->is_walkable() && _current_moves[i+1][j]->mob-1 > _current_moves[i][j]->mob) {
                            _current_moves[i+1][j]->mob = _current_moves[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            _current_moves[i+1][j]->prev = _current_moves[i][j];
                        }
                    }
                    if(j-1 >= 0) { // Watch if can go to  top case
                        if(area[i][j-1].data()->is_walkable() && _current_moves[i][j-1]->mob-1 > _current_moves[i][j]->mob) {
                            _current_moves[i][j-1]->mob = _current_moves[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            _current_moves[i][j-1]->prev = _current_moves[i][j];
                        }
                    }
                    if(j+1 < _current_moves[i].size()) { // Watch if go to from right case
                        if(area[i][j+1].data()->is_walkable() && _current_moves[i][j+1]->mob-1 > _current_moves[i][j]->mob) {
                            _current_moves[i][j+1]->mob = _current_moves[i][j]->mob - 1; // - case mobilité si on fait des cases avec mobilité différente de 1
                            _current_moves[i][j+1]->prev = _current_moves[i][j];
                        }
                    }
                }
            }
        }
    }

    // We can not move on other persos, so we remove these cases
    foreach(GraphicsObject *obj, persos) {
        Perso *all_perso = obj->get_object();
        _current_moves[all_perso->get_position().getX()][all_perso->get_position().getY()]->mob = INFINITY;
    }

    // We can stay on this position
    _current_moves[perso->get_object()->get_position().getX()][perso->get_object()->get_position().getY()]->mob = 0;

    // Do it on cases but then do it on model with signals send to graphic tile
    for (int ik = 0 ; ik < map_width ; ik ++) {
        for (int jk = 0 ; jk < map_height ; jk ++) {
            // Set visibility on cases
            area[ik][jk].data()->set_walkable_for_action(_current_moves[ik][jk]->mob >= 0 && _current_moves[ik][jk]->mob != INFINITY);
        }
    }
}

