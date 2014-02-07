/*
 * This file is part of UMons - horaires project
 *
 * ILS Heuristics are used to improve a feasible schedule to respect
 * soft constraints. To do this, we use the Iterative Local Search heuristic
 * with a perturbation function.
 *
 *  Created on: 07/02/2014
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "util.h"
#include "structs.h"
#include "ils_heuristics.h"

array_exams *
iterative_local_search(array_exams *exams) {
	array_exams *best, *candidate;
	float best_score, candidate_score;

	best = exams;
	best_score = fitness(best);

	do {
		candidate = perturbation(best);
		candidate_score = fitness(candidate);

		if(! acceptance_criterion(candidate, best_score, candidate_score)) {
			free_exams(candidate);
			continue;
		}
		free_exams(best);

		best = candidate;
		best_score = candidate_score;
	} while(termination_condition(best, best_score));

	return best;
}

float fitness(array_exams *exams) {
	// ToDo
	return 0;
}

array_exams* perturbation(array_exams *best) {
	// ToDo
	return NULL;
}

bool acceptance_criterion(array_exams *candidate, float best_score, float candidate_score) {
	// ToDo
	return false;
}

bool termination_condition(array_exams *best, float best_score) {
	// ToDo
	return true;
}