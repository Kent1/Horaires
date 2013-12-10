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
#include "structs/exam.h"
#include "structs/room.h"
#include "heuristics.h"

/**
 * This function returns a pointer to the next exam to schedule,
 * following the saturation degree first and the largest
 * enrollment in case of tie-break.
 *
 * @param exams An array of exams.
 * @return The next exam to schedule following our heuristics.
 */
exam *get_first_exam(exam *exams, uint16_t size, uint8_t max_timeslot) {
    // Initialization
    uint16_t i;
    uint8_t best = 255;
    exam *first = NULL;

    // Compute all saturation degrees for the given exams
    uint8_t *sat_degree = get_exams_saturation_degree(exams, size, max_timeslot);

    // Find the exam with the max saturation degree and resolve
    // tie-break with largest enrollment
    for (i = 0; i < size; i++) {
        // Exam 'i' already scheduled & so we don't care about sat_degree
        if (sat_degree[i] == NO_SAT)
            continue;

        if (sat_degree[i] < best) {
            best  = sat_degree[i];
            first = &exams[i];
        } else if (sat_degree[i] == best &&
                   exams[i].enrollment > first->enrollment) {
            best  = sat_degree[i];
            first = &exams[i];
        }
    }

    free(sat_degree);

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
uint8_t *get_exams_saturation_degree(exam *exams, uint16_t size,
                                     uint8_t max_timeslot) {
    uint16_t i, j;
    uint8_t *sat_degree = calloc(size, sizeof(uint8_t));

    for (i = 0; i < size; i++) {
        // already scheduled
        if (exams[i].timeslot != NOT_SCHEDULED) {
            sat_degree[i] = NO_SAT;
        } else { // not scheduled, compute all conflicts
            sat_degree[i] = max_timeslot;

            for (j = 0; j < max_timeslot; j++) {
                if (exams[i].availabilities[j] == false)
                    sat_degree[i]--;
            }

            for (j = 0; j < size; j++) {
                // A conflict represents an edge between i and j,
                // then if j has a timeslot => j is scheduled
                if (exams[i].conflicts[j] && exams[j].timeslot != 0 &&
                        exams[i].availabilities[exams[j].timeslot] == true)
                    sat_degree[i]--;
            }
        }
    }

    return sat_degree;
}

/**
 * Computes all available timeslots, knowing all those who
 * have already been scheduled and the possibilities of that exam.
 *
 * @param exam_ The exam to schedule
 * @param exams Array of all exams
 * @param size Size of array of exams
 * @param max_timeslot Maximum timeslot
 * @return An array of boolean with real available timeslots.
 */
bool *set_possible_timeslot(exam *exam_, exam *exams, uint16_t size,
                            uint8_t min_timeslot, uint8_t max_timeslot) {
    uint16_t i;
    bool *timeslots_available = calloc(max_timeslot, sizeof(bool));

    for (i = 0; i < max_timeslot; i++)
        timeslots_available[i] = exam_->availabilities[i];

    // Store all the timeslots used by neighbors (i.e. vertex in conflict)
    for (i = 0; i < size; i++) {
        if (exam_->conflicts[i] && exams[i].timeslot != NOT_SCHEDULED)
            // If i is a neighbor and i is scheduled
            timeslots_available[exams[i].timeslot] = false;
    }

    return timeslots_available;
}

/**
 * Try to set a correct timeslot to all exams. Correct means
 * that any students/teachers have two exams scheduled in
 * the same period.
 *
 * @param exams Array of all exams
 * @param size Size of the array of exams
 * @param max_timeslot Maximum timeslot
 * @return True if a correct assignement is found, false otherwise
 */
bool color_graph_backtrack(exam *exams, uint16_t size, uint8_t max_timeslot) {
    exam *exam_ = get_first_exam(exams, size, max_timeslot);

    if (exam_ == NULL)
        return true;

    uint8_t min_timeslot = 0, i = 0;
    bool success         = false;
    bool backtrack       = false;

    bool *timeslots_available = set_possible_timeslot(exam_, exams, size,
                                min_timeslot, max_timeslot);

    do {
        // Iterate the array to search which is the minimal timeslot
        // available and set it to the exam
        for (i = min_timeslot; i < max_timeslot; i++) {
            if (timeslots_available[i] == true) {
                exam_->timeslot = i;
                break;
            }
        }

        // If i == max_timeslot,
        // there is no more available timeslots => need backtrack
        backtrack = (i == max_timeslot);

        if (!backtrack) {
            success = color_graph_backtrack(exams, size, max_timeslot);

            if (!success) {
                min_timeslot = exam_->timeslot + 1;
            }
        }
    } while (!backtrack && !success);

    if (backtrack)
        exam_->timeslot = NOT_SCHEDULED; // reset the timeslot

    free(timeslots_available);

    return success;
}

/**
 * Assign rooms to exams (exams must be scheduled). For each exam, we consider
 * a compatible room (i.e. a room with the right type), and we assign the room
 * with the minimum but sufficient capacity. rooms must be sorted by type and
 * capacity.
 *
 *
 * @param exams An array filled with scheduled exams
 * @param exams_size the size of exams array
 * @param rooms An array of rooms. This array is sorted by type and for each
 *               type, the rooms are sorted by capacity
 * @param room_type_indices An array filled with start indices of each type
 *                          in the rooms array
 * @param max_timeslot The maximum number of timeslot available
 * @return true if the algorithm was able to find a correct assignement,
 *         false otherwise
 */
bool room_assign(exam *exams, uint16_t exams_size, room *rooms,
                 uint16_t *room_type_indices, uint8_t max_timeslot) {
    uint16_t i, j;

    for (i = 0; i < exams_size; i++) {
        exam exam_ = exams[i];
        j = exam_.room_type ? room_type_indices[exam_.room_type] : 0;

        for (; j < room_type_indices[exam_.room_type]; j++) {
            room room_ = rooms[j];

            if (room_.assignation[exam_.timeslot] == -1 &&
                    room_.capacity >= exam_.enrollment) {
                room_.assignation[exam_.timeslot] = exam_.exam_id;
                exam_.room_id = room_.room_id;
                break;
            }
        }

        if (exam_.room_id == -1) {

            for (j = 0; j < exams_size; j++)
                exams[i].room_id = -1;

            for (j = 0; j < room_type_indices[MAX_ROOM_TYPE]; j++) {
                for (i = 0; i < max_timeslot; i++)
                    rooms[j].assignation[i] == -1;
            }

            return false;
        }
    }

    return true;
}
