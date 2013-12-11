/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef HEURISTICS_H_
#define HEURISTICS_H_

#define NOT_SCHEDULED UINT8_MAX
#define NO_SAT        UINT8_MAX

exam *get_first_exam(exam *, uint16_t, uint8_t);

uint8_t *get_exams_saturation_degree(exam *, uint16_t, uint8_t);

bool *set_possible_timeslot(exam *, exam *, uint16_t, uint8_t, uint8_t);

bool color_graph_backtrack(exam *, uint16_t, room ***, uint16_t **, uint8_t, uint8_t);

bool room_assign(exam *, uint16_t, room ***, uint16_t **, uint8_t, uint8_t);

#endif /*HEURISTICS_H_*/
