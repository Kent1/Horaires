/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef HEURISTICS_H_
#define HEURISTICS_H_

#define NOT_SCHEDULED UINT8_MAX
#define NO_SAT        UINT8_MAX


/**
 * This function returns a pointer to the next exam to schedule,
 * following the saturation degree first and the largest
 * enrollment in case of tie-break.
 *
 * @param  exams An array of exams.
 * @param  exams_size The size of the exams array.
 * @param  max_timeslot Maximum available timeslots.
 * @return The next exam to schedule following our heuristics.
 */
exam *get_first_exam(array_exams *exams, uint8_t max_timeslot);

/**
 * This function returns an array regrouping the saturation
 * degree for each given exams. The degree of already scheduled exams
 * is set to 255, considered as a special value to not consider.
 *
 * @param  exams An array of exams.
 * @param  exams_size The size of the exams array.
 * @param  max_timeslot Maximum available timeslots.
 * @return An array regrouping saturation degree for the given parameter.
 */
uint8_t *get_exams_saturation_degree(array_exams *exams, uint8_t max_timeslot);
/**
 * Computes all available timeslots, knowing all those which
 * have already been scheduled and the possibilities of that exam.
 *
 * @param exam_ The exam to schedule.
 * @param exams An array of exams.
 * @param exams_size The size of the exams array.
 * @param max_timeslot Maximum timeslot
 * @return A bool array of real available timeslots.
 */
bool *set_possible_timeslot(exam *exam_, array_exams *exams, uint8_t max_timeslot);

/**
 * Try to set a correct timeslot to all exams and then assign them a room.
 * Correct means that any students/teachers doesn't have two exams scheduled
 * in the same period. Same thing for the room, it must be a room compatible
 * for the exam (with the correct room_type) and with enough capacity.
 *
 * @param  exams An array of exams.
 * @param  exams_size The size of the exams array.
 * @param  rooms An 3-dimensional array of rooms. The first given indice is
 *               the faculty of the room. The second indice is the type of
 *               the room.
 * @param  rooms_size An two dimensions array filled with the number of rooms
 *                    given its faculty and its type.
 * @param  faculty_size The number of different faculties.
 * @param  max_timeslot Maximum available timeslots.
 * @return true if a correct assignement is found, false otherwise.
 *         The algorithm also assign a timeslot and a room to all exams if
 *         true is returned.
 */
bool color_graph_backtrack(array_exams *exams, room ***rooms, uint16_t **rooms_size, uint8_t faculty_size, uint8_t max_timeslot);

/**
 * This function assigns rooms to exams (exams must be scheduled).
 * For each exam, we consider a compatible room (i.e. a room with
 * the right type), and we assign the room with the minimum but
 * sufficient capacity. rooms must be sorted by type and capacity.
 *
 * @param exams An array of scheduled exams.
 * @param exams_size the size of exams array.
 * @param  rooms An 3-dimensional array of rooms. The first given indice is
 *               the faculty of the room. The second indice is the type of
 *               the room.
 * @param  rooms_size An two dimensions array filled with the number of rooms
 *                    given its faculty and its type.
 * @param  faculty_size The number of different faculties.
 * @param  max_timeslot Maximum available timeslots.
 * @return true if the algorithm was able to find a correct assignement,
 *         false otherwise. If true is return, rooms are assigned to all exams.
 */
bool room_assign(array_exams *exams, room ***rooms, uint16_t **room_size, uint8_t faculty_size, uint8_t max_timeslot);

#endif /*HEURISTICS_H_*/
