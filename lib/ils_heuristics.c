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

#define PAYOFF_SAMEDAY -20
#define PAYOFF_NEXTDAY -10

// Use parameters to return values (exams and rooms)
uint8_t
iterative_local_search(array_exams **exams, matrix_rooms **rooms,
                       uint8_t max_timeslot, uint16_t faculty_size,
                       uint8_t max_room_type) {
    // Declare variables
    exam *worst;
    array_exams *best_exams, *candidate;
    matrix_rooms *best_rooms, *rcandidate;
    float best_score, candidate_score, worst_score = -FLT_MAX;
    time_t start = time(NULL);
    time_t max_time = 60; // in seconds
    float threshold = 0;
    uint32_t counter = 0, max_counter = 100000;
    uint8_t final_condition = 0;

    // Initializes variables
    best_exams = *exams;
    best_rooms = *rooms;
    best_score = fitness_bis(best_exams);

    do {
        candidate = best_exams;
        rcandidate = best_rooms;

        fitness(best_exams, &worst, &worst_score);

        if (!worst) { // worst is NULL, no new worst to process
            *exams = best_exams;
            *rooms = best_rooms;
            return 4;
        }

        perturbation(&candidate, worst, max_timeslot, &rcandidate, faculty_size,
                     max_room_type);

        candidate_score = fitness_bis(candidate);

        if (acceptance_criterion(candidate, best_score, candidate_score)) {
            free_exams(best_exams);
            free_matrix_rooms(best_rooms, faculty_size, max_room_type);

            best_exams = candidate;
            best_score = candidate_score;
            best_rooms = rcandidate;

            worst_score = -FLT_MAX;
            counter = 0;
        } else {
            free_exams(candidate);
            free_matrix_rooms(rcandidate, faculty_size, max_room_type);
            counter++;
        }

    } while (!(final_condition = termination_condition(best_exams, best_score,
                                 threshold, start, max_time, counter, max_counter)));

    // Set pointer to the best
    *exams = best_exams;
    *rooms = best_rooms;

    return final_condition;
}

float
fitness(array_exams *exams, exam **worst, float *exam_fitness) {
    float fitness = 0;
    float min_threshold_fitness = *exam_fitness;
    float worst_fitness = FLT_MAX;
    *worst = NULL;

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
    uint64_t total_payoff = 0;

    for (uint16_t i = 0; i < exams->size; i++) {
        /* Skip if i is index */
        if (i == index)
            continue;

        if (exam->conflicts[i]) {
            int16_t dist = exam->timeslot - exams->data[i]->timeslot;

            if (dist < 0)
                dist *= -1;

            uint64_t payoff = 0;

            if (dist < 48)
                payoff = (uint64_t)(pow(2, (48 - dist)));

            if (dist == 1) {
                if ((exam->timeslot < exams->data[i]->timeslot &&
                        (exam->timeslot % 10) == 9) ||
                        (exams->data[i]->timeslot < exam->timeslot &&
                         (exams->data[i]->timeslot % 10) == 9))
                    payoff = 0;
                else if (((exam->timeslot % 2) == 0 &&
                          exam->timeslot < exams->data[i]->timeslot) ||
                         ((exam->timeslot % 2) == 1 &&
                          exams->data[i]->timeslot < exam->timeslot))
                    payoff = payoff;
                else
                    payoff = payoff / 2;
            }

            total_payoff += payoff;
            conflicts++;
        }
    }

    if (conflicts)
        return -1.0 * total_payoff / conflicts;
    else
        return 0;
}

void
perturbation(array_exams **current_best, exam *worst, uint8_t max_timeslot,
             matrix_rooms **current_rbest, uint16_t faculty_size,
             uint8_t max_room_type) {

    // Variables declaration & initialization
    array_exams  *current = *current_best;
    matrix_rooms *rooms   = *current_rbest;
    array_exams  *best_candidate  = clone_array_exams(current, max_timeslot);
    matrix_rooms *best_rcandidate = clone_matrix_rooms(rooms, max_timeslot,
                                    faculty_size, max_room_type);

    // Numeric variables
    uint16_t id_worst = 0;
    float    best_candidate_score = -FLT_MAX;

    for (uint8_t i = 0; i < current->size; i++) {
        if (current->data[i] == worst) {
            id_worst = i;
            break;
        }
    }

    uint8_t inf_timeslot = compute_inf_timeslot(current->data[id_worst], current);
    uint8_t sup_timeslot = compute_sup_timeslot(current->data[id_worst], current,
                           max_timeslot);

    /* For each timeslot, search a better solution by spreading the exam with
       the next worst fitness known, test by deplacing to each timeslot
       available and keep in mind that the result must remain feasible. */
    for (uint8_t i = inf_timeslot; i < sup_timeslot; i++) {
        // Variables (again)
        uint8_t timeslot_before = current->data[id_worst]->timeslot;
        uint8_t timeslot_after  = i;
        bool    op_status = !current->data[id_worst]->availabilities[timeslot_after]
                            || timeslot_before == timeslot_after;

        if (op_status)
            continue; // If the timeslot is not available, go to next timeslot

        float candidate_score    = 0;
        array_exams  *candidate  = clone_array_exams(current, max_timeslot);
        matrix_rooms *rcandidate = clone_matrix_rooms(rooms, max_timeslot,
                                   faculty_size, max_room_type);

        /* Check if there exists a conflict with exams scheduled in the timeslot
           i. If not, just move, otherwhise use Kempe Chains algorithm. */
        if (check_conflict(candidate, id_worst, timeslot_after)) {
            uint8_t *swaps = swap_initialization(candidate->size);
            op_status = kempe_chains(candidate, id_worst, timeslot_after, swaps);

            if (!op_status) {
                free_exams(candidate);
                free_matrix_rooms(rcandidate, faculty_size, max_room_type);
                continue;
            }

            swap_timeslots(candidate, swaps);
            free(swaps);

            // Reset rcandidate for both timeslots i and old_timeslot
            reset_room_by_timeslot(candidate, rcandidate, timeslot_before);
            reset_room_by_timeslot(candidate, rcandidate, timeslot_after);

            // Launch assign room on both timeslot
            op_status = assign_by_timeslot(candidate, rcandidate, timeslot_before);

            if (op_status) // The first reassign has to pass to do the second
                op_status = assign_by_timeslot(candidate, rcandidate, timeslot_after);

        } else {
            // Move the timeslot
            candidate->data[id_worst]->timeslot = timeslot_after;
            // Move the room if possible, otherwhise reassignation
            room **rooms_ = rcandidate->data[worst->faculty][worst->room_type];

            for (uint16_t j = 0; j < rcandidate->size[worst->faculty][worst->room_type];
                    j++) {
                if (rooms_[j]->room_id == worst->room_id) {
                    rooms_[j]->assignation[timeslot_before] = NOT_ASSIGNED;

                    if (rooms_[j]->assignation[timeslot_after] == NOT_ASSIGNED)
                        rooms_[j]->assignation[timeslot_after] = worst->exam_id;

                    break;
                }
            }

            // Room desassignation if not ok
            op_status = is_valid(candidate, rcandidate, timeslot_before, timeslot_after);

            if (!op_status) {
                reset_room_by_timeslot(candidate, rcandidate, timeslot_after);
                op_status = assign_by_timeslot(candidate, rcandidate, timeslot_after);
            }
        }

        // If one of the reassignation has failed,
        // then stop the current iteration
        if (!op_status) {
            free_exams(candidate);
            free_matrix_rooms(rcandidate, faculty_size, max_room_type);
            continue;
        }

        candidate_score = fitness_bis(candidate);

        // Saves best permutation
        if (best_candidate_score < candidate_score) {
            free_exams(best_candidate);
            free_matrix_rooms(best_rcandidate, faculty_size, max_room_type);

            best_candidate       = candidate;
            best_candidate_score = candidate_score;
            best_rcandidate      = rcandidate;
        } else {
            free_exams(candidate);
            free_matrix_rooms(rcandidate, faculty_size, max_room_type);
        }
    }

    // Set pointer to the best
    *current_best  = best_candidate;
    *current_rbest = best_rcandidate;
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
kempe_chains(array_exams *candidate, uint16_t exam_id, uint8_t swap_slot,
             uint8_t *swaps) { // exam_id ~ id in the table contening the exam

    // Variables initialization
    bool status      = true;
    uint8_t timeslot = candidate->data[exam_id]->timeslot;

    // Set swap for the current exam
    swaps[exam_id]   = swap_slot;

    // Verify the exam, referenced by exam_id, can be moved to swap_slot
    uint8_t inf_timeslot = compute_inf_timeslot(candidate->data[exam_id],
                                                candidate);
    uint8_t sup_timeslot = compute_sup_timeslot(candidate->data[exam_id],
                                                candidate, swap_slot);

    if (inf_timeslot > swap_slot || swap_slot > sup_timeslot)
        return false;

    /* For each exam, check if it is conflicting with the current exam.
       If it is the case, check if this exam is in the timeslot to swap,
       and not handled yet. Each exam that is going to swap must be
       available for the timeslot where it goes, otherwhise KC fails. */
    for (uint16_t i = 0; i < candidate->size; i++) {
        if (candidate->data[exam_id]->conflicts[i]
                && candidate->data[i]->timeslot == swap_slot
                && swaps[i] == NOT_SCHEDULED) {

            if (candidate->data[i]->availabilities[timeslot])
                status = kempe_chains(candidate, i,
                                      candidate->data[exam_id]->timeslot, swaps);

            if (!candidate->data[i]->availabilities[timeslot] || !status)
                return false;
        }
    }

    return true;
}

uint8_t *
swap_initialization(size_t size) {
    uint8_t *swaps = malloc(size * sizeof(uint8_t));

    for (uint8_t j = 0; j < size; j++)
        swaps[j] = NOT_SCHEDULED;

    return swaps;
}

void
swap_timeslots(array_exams *candidate, uint8_t *swaps) {
    for (uint16_t i = 0; i < candidate->size; i++) {
        if (swaps[i] != NOT_SCHEDULED)
            candidate->data[i]->timeslot = swaps[i];
    }
}

bool
acceptance_criterion(array_exams *candidate, float best_score,
                     float candidate_score) {
    return (candidate_score > best_score);
}

uint8_t
termination_condition(array_exams *best, float best_score, float threshold,
                      time_t start, time_t max_time,
                      uint32_t counter, uint32_t max_counter) {
    if (threshold > 0 && best_score >= threshold)
        return 1;

    if ((time(NULL) - max_time) > start)
        return 2;

    if (counter > max_counter)
        return 3;

    return 0;
}
