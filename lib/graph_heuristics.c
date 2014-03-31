/*
 * This file is part of UMons - horaires project
 *
 * Heuristics are use to assign to each exam a timeslot.
 * To do this, we transform the problem to a graph coloring problem,
 * each color representing a timeslot.
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "util.h"
#include "structs.h"
#include "graph_heuristics.h"
#include "room_assign.h"


exam *get_first_exam(array_exams *exams, uint8_t max_timeslot) {
    // Initialization
    uint8_t best = NO_SAT;
    exam *first = NULL;

    // Compute all saturation degrees for the given array of exams
    uint8_t *sat_degree = get_exams_saturation_degree(exams, max_timeslot);

    // Find the exam with the lowest saturation degree and resolve
    // tie-break with largest enrollment
    for (uint16_t i = 0; i < exams->size; i++) {
        // Exam 'i' already scheduled & so we don't care about sat_degree
        if (sat_degree[i] == NO_SAT)
            continue;

        if (sat_degree[i] < best) {
            best  = sat_degree[i];
            first = exams->data[i];
        } else if (sat_degree[i] == best &&
                   exams->data[i]->enrollment > first->enrollment) {
            best  = sat_degree[i];
            first = exams->data[i];
        }
    }

    free(sat_degree);

    return first;
}


uint8_t *get_exams_saturation_degree(array_exams *exams, uint8_t max_timeslot) {
    uint8_t *sat_degree = malloc(exams->size * sizeof(uint8_t));

    for (uint16_t i = 0; i < exams->size; i++) {
        if (exams->data[i]->timeslot != NOT_SCHEDULED) { // already scheduled
            sat_degree[i] = NO_SAT;
            continue;
        }

        bool compute_sat = true;

        for (uint8_t j = 0; j < exams->data[i]->deps_size; j++) {
            uint16_t index = exams->data[i]->deps[j];

            if (exams->data[index]->timeslot == NOT_SCHEDULED) {
                sat_degree[i] = NO_SAT;
                compute_sat = false;
                break;
            }
        }

        // If not scheduled; compute saturation degree or
        // If all deps scheduled; compute saturation degree.
        if (compute_sat) {
            // Initially saturation degree is maximum
            sat_degree[i] = max_timeslot;
            // Min_timeslot for the i-th exam
            uint8_t min_timeslot = compute_min_timeslot(exams->data[i], exams);

            /* Then substract a degree for each timeslot unavailable or for each
               timeslot available but strictly less than the min_timeslot (equals
               to a timeslot unavailable). */
            for (uint16_t j = 0; j < max_timeslot; j++) {
                if (exams->data[i]->availabilities[j] == false ||
                        (exams->data[i]->availabilities[j] && j < min_timeslot))
                    sat_degree[i]--;
            }

            /* Finally, substract a degree for each timeslot available,
               but already used by a conflicting exam, i.e. an edge exists
               between the two exam => something in common(teacher or students)
               => cannot be set on the same timeslot. */
            for (uint16_t j = 0; j < exams->size; j++) {
                if (exams->data[i]->conflicts[j] &&
                        exams->data[j]->timeslot != NOT_SCHEDULED &&
                        exams->data[j]->timeslot >= min_timeslot &&
                        exams->data[i]->availabilities[exams->data[j]->timeslot])
                    sat_degree[i]--;
            }
        }
    }

    return sat_degree;
}


bool *set_possible_timeslot(exam *exam_, array_exams *exams,
                            uint8_t max_timeslot) {
    bool *timeslots_available = malloc(max_timeslot * sizeof(bool));

    // Initially, possible availabilities are the ones provided with the exam
    for (uint16_t i = 0; i < max_timeslot; i++)
        timeslots_available[i] = exam_->availabilities[i];

    /* For each exam in conflict (i.e. that has a vertex with exam_) and
       which is scheduled on a certain timeslot, this timeslot is substracted
       from the timeslots available. */
    for (uint16_t i = 0; i < exams->size; i++) {
        if (exam_->conflicts[i] && exams->data[i]->timeslot != NOT_SCHEDULED)
            timeslots_available[exams->data[i]->timeslot] = false;
    }

    return timeslots_available;
}


uint8_t compute_min_timeslot(exam *exam_, array_exams *exams) {
    uint8_t min_timeslot = 0;

    for (uint8_t i = 0; i < exam_->deps_size; i++) {
        min_timeslot = MAX(min_timeslot, exams->data[exam_->deps[i]]->timeslot + 1);
    }

    return min_timeslot;
}


bool color_graph_backtrack(array_exams *exams, matrix_rooms *rooms,
                           uint8_t faculty_size, uint8_t max_room_type,
                           uint8_t max_timeslot) {
    // Pick up the next exam to schedule
    exam *exam_ = get_first_exam(exams, max_timeslot);

    // A solution has been found, then compute a room assignement
    if (exam_ == NULL)
        return room_assign(exams, rooms, faculty_size, max_room_type, max_timeslot);

    // Initializes some variables for the process part
    uint8_t i = 0;
    uint8_t min_timeslot = compute_min_timeslot(exam_, exams);
    bool success         = false;
    bool backtrack       = false;

    // Get an array of timeslots usable for this exam
    bool *timeslots_available = set_possible_timeslot(exam_, exams, max_timeslot);

    // Process part
    do {
        /* Iterate the available timeslots to search the next minimal timeslot
           available and set it to the exam. */
        for (i = min_timeslot; i < max_timeslot; i++) {
            if (timeslots_available[i] == true) {
                exam_->timeslot = i;
                break;
            }
        }

        /* If i == max_timeslot, there is no more available timeslot
           => need backtrack. */
        backtrack = (i == max_timeslot);

        if (!backtrack) { // No backtrack needed, process the next schedule
            success = color_graph_backtrack(exams, rooms, faculty_size, max_room_type, max_timeslot);

            if (!success) { // failed, must pick the next timeslot available
                min_timeslot = exam_->timeslot + 1;
            }
        }
    } while (!backtrack && !success);

    if (backtrack) // Reset timeslot in case of backtrack
        exam_->timeslot = NOT_SCHEDULED;

    free(timeslots_available);

    return success;
}
