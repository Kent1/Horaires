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
#include "heuristics.h"

/**
 * This function returns a pointer to the next exam to schedule,
 * following the saturation degree first and the largest
 * enrollment in case of tie-break.
 *
 * @param exams An array of exams.
 * @return The next exam to schedule following our heuristics.
 */
exam *get_first_exam(exam *exams, uint16_t exams_size, uint8_t max_timeslot) {
    // Initialization
    uint8_t best = NO_SAT;
    exam *first = NULL;

    // Compute all saturation degrees for the given array of exams
    uint8_t *sat_degree = get_exams_saturation_degree(exams, exams_size,
                          max_timeslot);

    // Find the exam with the lowest saturation degree and resolve
    // tie-break with largest enrollment
    for (uint16_t i = 0; i < exams_size; i++) {
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
uint8_t *get_exams_saturation_degree(exam *exams, uint16_t exams_size,
                                     uint8_t max_timeslot) {
    uint8_t *sat_degree = malloc(exams_size * sizeof(uint8_t));

    for (uint16_t i = 0; i < exams_size; i++) {
        if (exams[i].timeslot != NOT_SCHEDULED) { // already scheduled
            sat_degree[i] = NO_SAT;
        } else { // not scheduled, compute saturation degree
            // Initially saturation degree is maximum
            sat_degree[i] = max_timeslot;

            // Then substract a degree for each timeslot unavailable
            for (uint16_t j = 0; j < max_timeslot; j++) {
                if (exams[i].availabilities[j] == false)
                    sat_degree[i]--;
            }

            /* Finally, substract a degree for each timeslot available,
               but already used by a conflicting exam, i.e. an edge exists
               between the two exam => something in common(teacher or students)
               => cannot be set on the same timeslot. */
            for (uint16_t j = 0; j < exams_size; j++) {
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
bool *set_possible_timeslot(exam *exam_, exam *exams, uint16_t exams_size,
                            uint8_t min_timeslot, uint8_t max_timeslot) {
    bool *timeslots_available = malloc(max_timeslot * sizeof(bool));

    // Initially, possible availabilities are the ones provided with the exam
    for (uint16_t i = 0; i < max_timeslot; i++)
        timeslots_available[i] = exam_->availabilities[i];

    /* For each exam in conflict (i.e. that has a vertex with exam_) and
       which is scheduled on a certain timeslot, this timeslot is substracted
       from the timeslots available. */
    for (uint16_t i = 0; i < exams_size; i++) {
        if (exam_->conflicts[i] && exams[i].timeslot != NOT_SCHEDULED)
            timeslots_available[exams[i].timeslot] = false;
    }

    return timeslots_available;
}

/**
 * Try to set a correct timeslot to all exams and then assign them a room.
 * Correct means that any students/teachers have two exams scheduled in
 * the same period. Same thing for the room, it must be a room compatible
 * for the exam (with the correct room_type) and with enough capacity
 *
 * @param  exams Array of all exams
 * @param  exams_size Size of the array of exams
 * @param  rooms An array of rooms. This array is sorted by type and for each
 *               type, the rooms are sorted by capacity
 * @param  room_indices An two dimensions array filled with the number of rooms
 *                      given his faculty and type
 * @param  max_timeslot Maximum available timeslots
 * @return true if a correct assignement is found, false otherwise
 */
bool color_graph_backtrack(exam *exams, uint16_t exams_size, room ***rooms,
                           uint16_t **rooms_limits, uint8_t faculty_size, uint8_t max_timeslot) {
    // Pick up the next exam to schedule
    exam *exam_ = get_first_exam(exams, exams_size, max_timeslot);

    // A solution has been found, then compute a room assignement
    if (exam_ == NULL)
        return room_assign(exams, exams_size, rooms, rooms_limits, faculty_size,
                           max_timeslot);

    // Initializes some variables for the process part
    uint8_t min_timeslot = 0, i = 0;
    bool success         = false;
    bool backtrack       = false;

    // Get an array of timeslots usable for this exam
    bool *timeslots_available = set_possible_timeslot(exam_, exams, exams_size,
                                min_timeslot, max_timeslot);

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
            success = color_graph_backtrack(exams, exams_size, rooms,
                                     rooms_limits, faculty_size, max_timeslot);

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
 * @param  room_indices An two dimensions array filled with the number of rooms
 *                      given his faculty and type
 * @param max_timeslot The maximum number of timeslot available
 * @return true if the algorithm was able to find a correct assignement,
 *         false otherwise
 */
bool room_assign(exam *exams, uint16_t exams_size, room ***rooms,
                 uint16_t **rooms_limits, uint8_t faculty_size, uint8_t max_timeslot) {

    /* For each exam, having is own faculty and room_type, we'll select a room
       not assigned, corresponding with these parameters. If an exam has no
       room after the research, then the room assignation failed and another
       shedule has to be found => launch backtrack for the scheduling. */
    for (uint16_t i = 0; i < exams_size; i++) {
        exam *exam_ = &exams[i];

        // Research a unassigned room for the exam exam_
        for (uint16_t j = 0; j < rooms_limits[exam_->faculty][exam_->room_type]; j++) {
            room *room_ = &rooms[exam_->faculty][exam_->room_type][j];

            /* If a room, corresponding in faculty & type, is unassigned and
               can contain the number of students, then the room is assigned.*/
            if (room_->assignation[exam_->timeslot] == NOT_ASSIGNED &&
                    room_->capacity >= exam_->enrollment) {
                room_->assignation[exam_->timeslot] = exam_->exam_id;
                exam_->room_id = room_->room_id;
                break;
            }
        }

        /* If true, the reseach has failed and all values are reset before
           launching the backtrack for the schedule. */
        if (exam_->room_id == NOT_ASSIGNED) {
            for (uint16_t j = 0; j < exams_size; j++)
                exams[i].room_id = NOT_ASSIGNED;

            for (i = 0; i < faculty_size; i++)
                for (uint16_t j = 0; j < MAX_ROOM_TYPE; j++)
                    for (uint16_t k = 0; k < rooms_limits[i][j]; k++)
                        for (uint16_t l = 0; l < max_timeslot; l++)
                            rooms[i][j][k].assignation[l] = NOT_ASSIGNED;

            return false;
        }
    }

    // Only if a room assignement has been found
    return true;
}
