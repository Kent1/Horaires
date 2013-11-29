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

#include "heuristics.h"

/**
 * This function returns a pointer to the next exam to schedule,
 * following the saturation degree first and the largest
 * enrollment in case of tie-break.
 *
 * @param exams An array of exams.
 * @return The next exam to schedule following our heuristics.
 */
exam* get_first_exam(exam* exams, uint16_t size, uint8_t max_timeslot) {
    // Initialization
    uint16_t i;
    uint16_t best = 256;
    exam* first = NULL;

    // Compute all saturation degrees for the given exams
    uint8_t* sat_degree = get_exams_saturation_degree(exams, size, max_timeslot);

    // Find the exam with the max saturation degree and resolve
    // tie-break with largest enrollment
    for (i = 1; i < size; i++) {
        // Exam 'i' already scheduled
        if(sat_degree[i] == 0)
            continue;

        if(sat_degree[i] < best) {
            best  = sat_degree[i];
            first = &exams[i];
        } else if(sat_degree[i] == best &&
                    exams[i].enrollment > first->enrollment) {
            best  = sat_degree[i];
            first = &exams[i];
        }
    }

    return first;
}

/**
 * This function returns an array regrouping the saturation
 * degree for each given exams. The degree of already scheduled exams
 * is set to 255, considered as a special value to not consider.
 *
 * @param exams An array of exams.
 * @return An array regrouping saturation degree for the given parameter.
 */
uint8_t* get_exams_saturation_degree(exam* exams, uint16_t size, uint8_t max_timeslot) {
    uint16_t i, j;
    uint8_t* sat_degree = calloc(size, sizeof(uint8_t));

    for (i = 0; i < size; i++) {
        // already scheduled
        if(exams[i].timeslot != 0) {
            sat_degree[i] = 0;
        } else { // not scheduled, compute all conflicts
            sat_degree[i] = max_timeslot;

            for (j = 0; j < max_timeslot; j++) {
                if(exams[i].availabilities[j] == false)
                    sat_degree[i]--;
            }

            for (j = 0; j < size; j++) {
                // A conflict represents an edge between i and j,
                // then if j has a timeslot => j is scheduled
                if(exams[i].conflicts[j] && exams[j].timeslot != 0 && exams[i].availabilities[exams[j].timeslot] == true)
                    sat_degree[i]--;
            }
        }
    }

    return sat_degree;
}

/**
 * Try to set the minimal available timeslot
 *
 * @param exam_ The exam to schedule
 * @param exams Array of all exams
 * @param size Size of array of exams
 * @param max_timeslot Maximum timeslot
 * @return true if timeslot is assigned, false otherwise
 */
bool set_possible_timeslot(exam* exam_, exam* exams, uint16_t size,
                           uint8_t min_timeslot, uint8_t max_timeslot) {
    uint16_t i;
    bool timeslot_available[max_timeslot];

    for (i = 0; i < max_timeslot; i++)
        timeslot_available[i] = exam_->availabilities[i];

    // Store all the timeslots used by neighbors (i.e. vertex in conflict)
    for (i = 0; i < size; i++) {
        if (exam_->conflicts[i] && exams[i].timeslot != 0)
            // If i is a neighbor and i is scheduled
            timeslot_available[exams[i].timeslot - 1] = false;
    }

    // Iterate the array to search which is the minimal timeslot available
    // and set it to the exam
    for (i = min_timeslot; i <= max_timeslot; i++) {
        if (timeslot_available[i] == true) {
            exam_->timeslot = i;
            return true;
        }
    }

    return false;
}

/**
 * Try to set a correct timeslot to all exams. Correct means
 * that any students have two exams scheduled in the same period.
 *
 * @param exams Array of all exams
 * @param size Size of the array of exams
 * @param max_timeslot Maximum timeslot
 * @return True if a correct assignement is found, false otherwise
 */
bool color_graph_backtrack(exam* exams, uint16_t size, uint8_t max_timeslot) {
    exam* exam_ = get_first_exam(exams, size, max_timeslot);
    if (exam_ == NULL)
        return true;

    uint8_t min_timeslot = 1;
    bool success         = false;
    bool backtrack       = false;

    do {
        backtrack = !set_possible_timeslot(exam_, exams, size,
                                           min_timeslot, max_timeslot);
        if (!backtrack) {
            success = color_graph_backtrack(exams, size, max_timeslot);
            if (!success) {
                min_timeslot = exam_->timeslot+1;
            }
        }
    } while (!backtrack && !success);

    if (backtrack)
        exam_->timeslot = 0;

    return success;
}
