/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 07/02/2014
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef ILS_HEURISTICS_H_
#define ILS_HEURISTICS_H_

#include "graph_heuristics.h"
#include "room_assign.h"
#include <time.h>

/**
 * This function tries to return a better solution than the given
 * (feasible) one. Receiving a simple feasible schedule, the function
 * searches to make a better solution using an objective function to
 * maximize representing the respect of the soft constraints.
 *
 * The stop condition can be of 3 types : total time, number of iteration
 * without getting a better solution and a threshold to reach.
 *
 * @param exams An array of exams with a feasible schedule(struct array_exams).
 * @return A feasible schedule not worse than the given one.
 */
void iterative_local_search(array_exams **exams, matrix_rooms **rooms,
                                    uint8_t max_timeslot, uint16_t faculty_size,
                                    uint16_t max_room_type);

float fitness(array_exams *exams, exam **worst, float *exam_fitness);

float fitness_bis(array_exams *exams);

float local_fitness(array_exams *exams, uint16_t index);

void
perturbation(array_exams **current_best, exam *worst,
             uint8_t max_timeslot, matrix_rooms **current_rbest,
             uint16_t faculty_size, uint16_t max_room_type);

bool check_conflict(array_exams *candidate, uint16_t exam_id, uint8_t timeslot);
bool kempe_chains(array_exams *candidate, uint16_t exam_id, uint8_t swap_slot,
                  uint8_t *swaps);
uint8_t* swap_initialization(size_t size);
void swap_timeslots(array_exams *candidate, uint8_t *swaps);

bool acceptance_criterion(array_exams *candidate, float best_score,
                          float candidate_score);

bool termination_condition(array_exams *best, float best_score, float threshold,
                      time_t start, time_t max_time,
                      uint32_t counter, uint32_t max_counter);

#endif /*ILS_HEURISTICS_H_*/
