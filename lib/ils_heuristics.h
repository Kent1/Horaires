/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 07/02/2014
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef ILS_HEURISTICS_H_
#define ILS_HEURISTICS_H_

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
array_exams* iterative_local_search(array_exams *exams);

float fitness(array_exams *exams);

array_exams* perturbation(array_exams *best);

bool acceptance_criterion(array_exams *candidate, float best_score, float candidate_score);

bool termination_condition(array_exams *best, float best_score);

#endif /*ILS_HEURISTICS_H_*/
