/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 10/02/2014
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef ROOM_ASSIGN_H_
#define ROOM_ASSIGN_H_


/**
 * This function assigns rooms to exams (exams must be scheduled).
 * For each exam, we consider a compatible room (i.e. a room with
 * the right type), and we assign the room with the minimum but
 * sufficient capacity. rooms must be sorted by type and capacity.
 *
 * @param  exams An array of scheduled exams (struct array_exams).
 * @param  rooms An 3-dimensional array of rooms. The first given indice is
 *               the faculty of the room. The second indice is the type of
 *               the room (struct matrix_rooms).
 * @param  faculty_size The number of different faculties.
 * @param  max_timeslot Maximum available timeslots.
 * @return true if the algorithm was able to find a correct assignement,
 *         false otherwise. If true is return, rooms are assigned to all exams.
 */
bool room_assign(array_exams *exams, matrix_rooms *rooms, uint8_t faculty_size,
                 uint8_t max_timeslot);

/**
 * This function set to NO_ASSIGNED all rooms et all exams given in parameters.
 *
 * @param exams        An array of scheduled exams (struct array_exams).
 * @param rooms        An 3-dimensional array of rooms. The first given indice
 *                     is the faculty of the room. The second indice is the
 *                     type of the room (struct matrix_rooms).
 * @param faculty_size The number of different faculties.
 * @param max_timeslot Maximum available timeslots.
 */
void reset_room_assigned(array_exams *exams, matrix_rooms *rooms,
                         uint8_t faculty_size, uint8_t max_timeslot);

#endif /*ROOM_ASSIGN_H_*/
