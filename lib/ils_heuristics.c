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
    float fitness = 0;
    for(int i = 0; i < exams->size; i++) {
        fitness += local_fitness(exams, i);
    }
    return fitness;
}

float local_fitness(array_exams *exams, uint16_t index) {
    exam *exam = exams->data[index];
    uint16_t conflicts = 0;
    uint16_t distance = 0;
    for(int i = 0; i < exams->size; i++) {
        /* Skip if i is index */
        if(i == index)
            continue;
        if(exam->conflicts[i]) {
            int16_t dist = exam->timeslot - exams->data[i]->timeslot;
            if(dist < 0)
                dist *= -1;
            distance += dist;
            conflicts++;
        }
    }
    return distance*-1/conflicts;
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