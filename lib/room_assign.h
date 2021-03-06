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
 * @param  max_room_type Maximum number of room type.
 * @param  max_timeslot Maximum available timeslots.
 * @return true if the algorithm was able to find a correct assignement,
 *         false otherwise. If true is return, rooms are assigned to all exams.
 */
bool room_assign(array_exams *exams, matrix_rooms *rooms, uint8_t faculty_size,
                 uint8_t max_room_type, uint8_t max_timeslot);

/**
 * This function tries to find a free room for the exam given.
 *
 * @param  exam_        The exam which a room must be find
 * @param  rooms        An 3-dimensional array of rooms. The first given indice
 *                      is the faculty of the room. The second indice is the
 *                      type of the room (struct matrix_rooms).
 * @return              True if a room was found, false otherwise.
 */
bool room_assign_single_exam(exam *exam_, matrix_rooms *rooms);

/**
 * This function sets to NOT_ASSIGNED all rooms and all exams given
 * in parameters.
 *
 * @param exams        An array of scheduled exams (struct array_exams).
 * @param rooms        An 3-dimensional array of rooms. The first given indice
 *                     is the faculty of the room. The second indice is the
 *                     type of the room (struct matrix_rooms).
 * @param faculty_size The number of different faculties.
 * @param max_room_type Maximum number of room type.
 * @param max_timeslot Maximum available timeslots.
 */
void reset_room_assigned(array_exams *exams, matrix_rooms *rooms,
                         uint8_t faculty_size, uint8_t max_room_type,
                         uint8_t max_timeslot);

/**
 * This function sets to NOT_ASSIGNED all romms and all exams which was assigned
 * by the given timeslot.
 *
 * @param exams        An array of scheduled exams (struct array_exams).
 * @param rooms        An 3-dimensional array of rooms. The first given indice
 *                     is the faculty of the room. The second indice is the
 *                     type of the room (struct matrix_rooms).
 * @param timeslot     The selected timeslot.
 */
void reset_room_by_timeslot(array_exams *exams, matrix_rooms *rooms,
                            uint8_t timeslot);

/**
 * This function tries to assign a room to every exam with the given timeslot.
 *
 * @param exams        An array of scheduled exams (struct array_exams).
 * @param rooms        An 3-dimensional array of rooms. The first given indice
 *                     is the faculty of the room. The second indice is the
 *                     type of the room (struct matrix_rooms).
 * @param timeslot     The selected timeslot.
 * @return             True if all room with the given timeslot gets
 *                     a room assigned.
 */
bool assign_by_timeslot(array_exams *exams, matrix_rooms *rooms,
                        uint8_t timeslot);

/**
 * This function checks if all exams in the selected timeslots have
 * no conflitcs.
 * If T1 or T2 have a conflict, the rooms of the conflicted timeslot are reset.
 * The function tries to reassign the rooms without conflict.
 *
 * @param exams        An array of scheduled exams (struct array_exams).
 * @param rooms        An 3-dimensional array of rooms. The first given indice
 *                     is the faculty of the room. The second indice is the
 *                     type of the room (struct matrix_rooms).
 * @param timeslot1    First timeslot to check.
 * @param timeslot2    Second timeslot to check.
 *
 * @return             True if there is no conflicts or rooms reassignation
 *                     leads to a solution.
 */
bool is_valid(array_exams *exams, matrix_rooms *rooms, uint8_t timeslot1,
              uint8_t timeslot2);

/**
 * This function checks if all exams in the same timeslot don't have
 * any conflict (i.e., students, professors, rooms).
 *
 * @param exams        An array of scheduled exams (struct array_exams).
 * @param rooms        An 3-dimensional array of rooms. The first given indice
 *                     is the faculty of the room. The second indice is the
 *                     type of the room (struct matrix_rooms).
 * @param timeslot     The selected timeslot.
 * @return             True if there is no conflicts.
 */
bool valid_assign_by_timeslot(array_exams *exams, matrix_rooms *rooms,
                              uint8_t timeslot);

#endif /*ROOM_ASSIGN_H_*/
