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
iterative_local_search(array_exams *exams, uint8_t max_timeslot) {
    // array_exams *best, *candidate;
    // float best_score, candidate_score;

    // best = exams;
    // best_score = fitness(best);

    // do {
    //     candidate = perturbation(best);
    //     candidate_score = fitness(candidate);

    //     if(acceptance_criterion(candidate, best_score, candidate_score)) {
    //         free_exams(best);

    //         best = candidate;
    //         best_score = candidate_score;
    //     } else {
    //         free_exams(candidate);
    //     }
    // } while(termination_condition(best, best_score));

    // return best;
    return 0;
}

float
fitness(array_exams *exams, exam **worst, float *exam_fitness,
        float min_threshold_fitness) {
    float fitness = 0;
    float worst_fitness = FLT_MAX;

    for (uint16_t i = 0; i < exams->size; i++) {
        float l_fitness = local_fitness(exams, i);
        fitness += l_fitness;

        /* Warning : ignore exam with exactly same fitness */
        if (l_fitness < worst_fitness && l_fitness > min_threshold_fitness) {
            worst_fitness = l_fitness;
            *worst = exams->data[i];
        }
    }

    *exam_fitness = worst_fitness;

    return fitness;
}

float
fitness_bis(array_exams *exams) {
    float fitness = 0;

    for (uint16_t i = 0; i < exams->size; i++) {
        float l_fitness = local_fitness(exams, i);
        fitness += l_fitness;
    }

    return fitness;
}

float
local_fitness(array_exams *exams, uint16_t index) {
    exam *exam = exams->data[index];
    uint16_t conflicts = 0;
    uint16_t distance = 0;

    for (uint16_t i = 0; i < exams->size; i++) {
        /* Skip if i is index */
        if (i == index)
            continue;

        if (exam->conflicts[i]) {
            int16_t dist = exam->timeslot - exams->data[i]->timeslot;

            if (dist < 0)
                dist *= -1;

            distance += dist;
            conflicts++;
        }
    }

    if (conflicts)
        return 1.0 * distance / conflicts;
    else
        return 0;
}

array_exams *
perturbation(array_exams *current, uint16_t id_worst,
             float initial_fitness, uint8_t max_timeslot) {
    array_exams *best_candidate = NULL;
    float best_candidate_score = FLT_MIN;

    /* For each timeslot, search a better solution by spreading the exam with the next
       worst fitness known, test by deplacing to each timeslot available and check that
       the result remains feasible. */
    for (uint8_t i = 0; i < max_timeslot; i++) {
        float candidate_score = 0;
        array_exams *candidate = clone_array_exams(current, max_timeslot);

        /* Check if there exists a conflict with exams scheduled
         in the timeslot i. If not, just move, otherwhise use
         Kempe Chains algorithm. */
        if (check_conflict(candidate, id_worst, i)) { // KC
            uint8_t *swaps = calloc(candidate->size, sizeof(uint8_t));
            kempe_chains(candidate, id_worst, i, swaps);

            swap_timeslots(candidate, swaps);

            // Reset rooms for both timeslots i and old_timeslot
        } else { // No conflict, just move
            candidate->data[id_worst]->timeslot = i;
            // Desassignation room
        }

        candidate_score = fitness_bis(candidate);

        // Saves best permutation
        if (best_candidate_score < candidate_score) {
            free_exams(best_candidate);

            best_candidate = candidate;
            best_candidate_score = candidate_score;
        } else {
            free_exams(candidate);
        }
    }

    return best_candidate;
}

bool
check_conflict(array_exams *candidate, uint16_t exam_id,
               uint8_t timeslot) {
    for (uint16_t i = 0; i < candidate->size; i++) {
        if (candidate->data[exam_id]->conflicts[i] &&
                candidate->data[i]->timeslot == timeslot)
            return true;
    }

    return false;
}

void
kempe_chains(array_exams *candidate, uint16_t exam_id, uint8_t swap_slot,
             uint8_t *swaps) {
    swaps[exam_id] = swap_slot;

    for (uint16_t i = 0; i < candidate->size; i++) {
        if (candidate->data[exam_id]->conflicts[i]
                && candidate->data[i]->timeslot == swap_slot
                && swaps[i] == 0) {
            kempe_chains(candidate, i, candidate->data[exam_id]->timeslot, swaps);
        }
    }
}

void
swap_timeslots(array_exams *candidate, uint8_t *swaps) {
    for (uint16_t i = 0; i < candidate->size; i++) {
        if (swaps[i] != 0)
            candidate->data[i]->timeslot = swaps[i];
    }
}

bool
acceptance_criterion(array_exams *candidate, float best_score,
                     float candidate_score) {
    // ToDo
    return false;
}

bool
termination_condition(array_exams *best, float best_score) {
    // ToDo
    return true;
}
