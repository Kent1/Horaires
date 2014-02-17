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

void
perturbation(array_exams *current, uint16_t id_worst,
             float initial_fitness, uint8_t max_timeslot,
             matrix_rooms *rooms, uint16_t faculty_size,
             uint16_t max_room_type) {
    array_exams *best_candidate = NULL;
    float best_candidate_score = FLT_MIN;

    /* For each timeslot, search a better solution by spreading the exam with the next
       worst fitness known, test by deplacing to each timeslot available and check that
       the result remains feasible. */
    for (uint8_t i = 0; i < max_timeslot; i++) {
        uint8_t timeslot_before = current->data[id_worst]->timeslot;
        uint8_t timeslot_after  = i;

        // If the new timeslot is not possible
        if(!check_preds(current, id_worst, timeslot_after))
            continue;

        float candidate_score = 0;
        array_exams *candidate = clone_array_exams(current, max_timeslot);
        matrix_rooms *rcandidate = clone_matrix_rooms(rooms, max_timeslot, faculty_size, max_room_type);

        /* Check if there exists a conflict with exams scheduled
         in the timeslot i. If not, just move, otherwhise use
         Kempe Chains algorithm. */
        if (check_conflict(candidate, id_worst, timeslot_after)) { // KC
            uint8_t *swaps = calloc(candidate->size, sizeof(uint8_t));
            kempe_chains(candidate, id_worst, timeslot_after, swaps);

            swap_timeslots(candidate, swaps);

            //Reset rcandidate for both timeslots i and old_timeslot
            reset_room_by_timeslot(candidate, rcandidate, timeslot_before);
            reset_room_by_timeslot(candidate, rcandidate, timeslot_after);
            // Launch assign room on both timeslot
            bool status_reassign = assign_by_timeslot(candidate, rcandidate, timeslot_before);
            if(!status_reassign) {
                free_exams(candidate);
                free_matrix_rooms(rcandidate, faculty_size, max_room_type);
                continue;
            }

            status_reassign = assign_by_timeslot(candidate, rcandidate, timeslot_after);
            if(!status_reassign) {
                free_exams(candidate);
                free_matrix_rooms(rcandidate, faculty_size, max_room_type);
                continue;
            }

        } else { // No conflict, just move
            exam *worst = candidate->data[id_worst];
            worst->timeslot = timeslot_after;

            for(uint16_t j = 0; j < rcandidate->size[worst->faculty][worst->room_type]; j++) {
                if(rcandidate->data[worst->faculty][worst->room_type][j]->room_id == worst->room_id) {
                    rcandidate->data[worst->faculty][worst->room_type][j]->assignation[timeslot_before] = NOT_ASSIGNED;

                    if(rcandidate->data[worst->faculty][worst->room_type][j]->assignation[timeslot_after] == NOT_ASSIGNED)
                        rcandidate->data[worst->faculty][worst->room_type][j]->assignation[timeslot_after] = worst->exam_id;
                }
            }

            // Desassignation room if not ok
            bool must_reassign = !is_valid(candidate, rcandidate, timeslot_before, timeslot_after);

            if(must_reassign) {
                reset_room_by_timeslot(candidate, rcandidate, timeslot_after);
                bool status_reassign = assign_by_timeslot(candidate, rcandidate, timeslot_after);
                if(!status_reassign) {
                    free_exams(candidate);
                    free_matrix_rooms(rcandidate, faculty_size, max_room_type);
                    continue;
                }
            }
        }

        candidate_score = fitness_bis(candidate);

        // Saves best permutation
        if (best_candidate_score < candidate_score) {
            free_exams(best_candidate);
            free_matrix_rooms(rooms, faculty_size, max_room_type);

            best_candidate = candidate;
            best_candidate_score = candidate_score;
            rooms = rcandidate;
        } else {
            free_exams(candidate);
            free_matrix_rooms(rcandidate, faculty_size, max_room_type);
        }
    }
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

bool
check_preds(array_exams *candidate, uint16_t exam_id, uint8_t timeslot) {
    uint8_t min_timeslot = compute_min_timeslot(candidate->data[exam_id], candidate);

    return min_timeslot < timeslot;
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
