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
 * This function tries to return a better solution than the given (feasible)
 * one. Receiving a simple feasible schedule, the function searches to make
 * a better solution using an objective function to maximize representing
 * the respect of the soft constraints.
 *
 * The stop condition can be of 3 types : total time, number of iteration
 * without getting a better solution and a threshold to reach.
 *
 * @param exams An array_exams struct with a feasible schedule.
 * @param matrix_rooms A matrix_rooms struct with feasible room assignment.
 * @param max_timeslot Maximum available timeslots.
 * @param faculty_size The number of different faculties.
 * @param max_room_type Maximum room type available.
 *
 * @return A feasible schedule not worse than the given one.
 */
void iterative_local_search(array_exams **exams, matrix_rooms **rooms,
                            uint8_t max_timeslot, uint16_t faculty_size,
                            uint16_t max_room_type);

/**
 * This function is used to compute the score of the given schedule and to
 * return the exam with the worst score. The provided variables allow to
 * consider only the worst exam from a certain threshold.
 *
 * The function returns the exam with the worst score by saving its pointer
 * into the variable worst. The score is also saved in the exam_fitness.
 *
 * @param exams An array_exams struct with a feasible schedule.
 * @param worst A exam pointer to save the exam with the worst score of the
 *              given schedule.
 * @param exam_fitness A threshold for the worst exam research and to save
 *                     the new worst score.
 *
 * @return The total fitness score for the provided schedule.
 */
float fitness(array_exams *exams, exam **worst, float *exam_fitness);

/**
 * This function is used to compute the score of the given schedule. Unlike the
 * previous function, it does not save any information about the worst exam and
 * only do the fitness computation.
 *
 * @param exams An array_exams struct with a feasible schedule.
 * @return The total fitness score for the provided schedule.
 */
float fitness_bis(array_exams *exams);

/**
 * This function computes the fitness score of a specific exam and returns it.
 * It uses a evaluation function that can be modified. The actual function
 * computes the distance between the considered exam and the other exams that
 * are relation with.
 *
 * @param exams An array_exams struct with a feasible schedule.
 * @param index The index that specifies for which exam compute the fitness.
 *
 * @return The fitness score of the specified exam.
 */
float local_fitness(array_exams *exams, uint16_t index);

/**
 * This function computes a perturbation for the given exam providing the best
 * improvement, if possible. The result is returned by the pointer given as
 * parameters. One or more exam may be moved and in that case, the schedule
 * updates all its informations and the room assignation too.
 *
 * @param current_best An array_exams struct to provide and save the best
 *                     schedule of the perturbation.
 * @param worst The exam to move to improve the global fitness of the schedule.
 * @param max_timeslot Maximum available timeslots.
 * @param current_rbest The matrix_rooms struct room assignment for the
 *                      corresponding array_exams pointed by current_best.
 * @param faculty_size The number of different faculties.
 * @param max_room_type Maximum room type available.
 */
void perturbation(array_exams **current_best, exam *worst,
                  uint8_t max_timeslot, matrix_rooms **current_rbest,
                  uint16_t faculty_size, uint16_t max_room_type);

/**
 * This function checks if there exists a conflict in a candidate schedule,
 * for a specific exam in a certain timeslot.
 *
 * @param candidate An array_exams struct with a candidate schedule.
 * @param exam_id The id in the table contening the exam to check
 *                for conflicts.
 * @param timeslot The timeslot for which the verification is done.
 *
 * @return True in case of conflicts between some exams and the specified one
 * for the provided timeslot, False otherwise.
 */
bool check_conflict(array_exams *candidate, uint16_t exam_id, uint8_t timeslot);

/**
 * This function fills the array swaps such that the i-th entry contains a
 * timeslot that the i-th exam must be swapped with. This creates two sets,
 * the kempe chains, and all exams in both sets will have their timeslot
 * swapped.
 *
 * @param candidate An array_exams struct with a candidate schedule.
 * @param exam_id The id in the table contening the exam to check
 *                for conflicts.
 * @param swap_slot The timeslot value to update the exams in the other set.
 * @param swaps An array containing the swap informations.
 *
 * @return An array of timeslot to swap/update exams timeslot if necessary.
 */
bool kempe_chains(array_exams *candidate, uint16_t exam_id, uint8_t swap_slot,
                  uint8_t *swaps);

/**
 * This function allocates and initializes an array of uint8_t to give to
 * the kempe_chains function.
 *
 * @param size The size of the array to allocate.
 *
 * @return An array of the specified size initialized.
 */
uint8_t *swap_initialization(size_t size);

/**
 * This function updates the timeslot of some exams in the given schedule.
 * For each entry of the schedule, the timeslot of the i-th exam is updated
 * with the i-th entry of the array swaps,
 *
 * @param candidate An array_exams struct with a candidate schedule.
 * @param swaps The array of new timeslots to update the schedule with.
 */
void swap_timeslots(array_exams *candidate, uint8_t *swaps);

/**
 * This function answers if the candidate schedule must be accepted to replace
 * the current best schedule in the iterative_local_search. The function is
 * pretty simple but can be modified to use more information provided in
 * parameters.
 *
 * @param candidate An array_exams struct with a candidate schedule.
 * @param best_score The fitness score of the best current schedule.
 * @param candidate_score The fitness score of the candidate schedule.
 *
 * @return True if the candidate schedule can replace the actual best
 * schedule, False otherwise.
 */
bool acceptance_criterion(array_exams *candidate, float best_score,
                          float candidate_score);

/**
 * This function evaluates if the iterative_local_search can continue and may be
 * improve its result, or if enough attempts have been made and the search can
 * stop. Multiple conditions are evaluate as a specified threshold, maximum time
 * elapsed since start of iterative local search or a certain amount of
 * candidate non improving the fitness.
 *
 * @param best An array_exals struct with the best current schedule.
 * @param best_score The fitness score of the best current schedule.
 * @param threshold A specific value that the best_score must reach,
 *                  if 0 then threshold condition is not considered.
 * @param start The time in seconds when the iterative_local_search begins.
 * @param max_time The maximum number of seconds that limits
 *                 the iterative_local_search work.
 * @param counter The number of iteration the iterative_local_search
 *                has done, without improving the best score.
 * @param max_counter Maximum number of iteration the iterative_local_search
 *                can do, without improving its best score before stop.
 *
 * @return True if the iterative_local_search must stop, False otherwise.
 */
bool termination_condition(array_exams *best, float best_score, float threshold,
                           time_t start, time_t max_time,
                           uint32_t counter, uint32_t max_counter);

#endif /*ILS_HEURISTICS_H_*/
