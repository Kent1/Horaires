/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 11/12/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include "util.h"

/* Constants */
#define MAX_ROOM_TYPE 3
#define NOT_ASSIGNED UINT16_MAX
#define NOT_SCHEDULED UINT8_MAX

/**
 * @enum room_type
 *
 * enumeration type for the different room types.
 */
typedef enum {
    classroom,
    lab,
    computer_room,
} room_type;


/**
 * @struct room
 * @brief This structure represents a room.
 *
 * @var room::room_id
 * Id of the room.
 *
 * @var room::type
 * Type of the room (Amphitheater, lab, ...). @see room_type.
 *
 * @var room::faculty
 * Faculty which owns the room.
 *
 * @var room::capacity
 * Number of seats in the room.
 *
 * @var room::assignation
 * Array representing the reservation of the room.
 * Contains for each timeslot the assigned exam_id.
 */
typedef struct {
    uint16_t  room_id;
    room_type type;
    uint8_t   faculty;
    uint16_t  capacity;
    uint16_t  *assignation;
} room;

/**
 * @struct array_rooms
 * @brief This structure represents an array of rooms.
 *
 * @var array_rooms::data
 * Pointer on the room datas.
 *
 * @var array_rooms::size
 * Size of the array pointed by @see data.
 */
typedef struct {
    room **data;
    size_t size;
} array_rooms;

/**
 * @struct matrix_rooms
 * @brief This structure represents 3-d matrix of rooms.
 *
 * @var matrix_rooms::data
 * Pointer on the matrix of room datas.
 *
 * @var matrix_rooms::size
 * Sizes of the matrix pointed by @see data.
 */
typedef struct {
    room ****data;
    size_t **size;
} matrix_rooms;

/**
 * @struct exam
 * @brief This structure represents an exam.
 *
 * @var exam::exam_id
 * Id of the exam.
 *
 * @var exam::faculty
 * Exams are associated in general to a faculty.
 *
 * @var exam::teacher_id
 * ID of the teacher responsible of the exam.
 *
 * @var exam::students
 * List of students which have to take the exam.
 *
 * @var exam::enrollment
 * Number of students, also sizeof students.
 *
 * @var exam::room_type
 * Needed room_type for this exam.
 *
 * @var exam::room_id
 * Assigned room id to this exam.
 *
 * @var exam::timeslot
 * Assigned timeslot.
 *
 * @var exam::availabilities
 * Bool array, describing a list of all timeslots in which the i-th index
 * is true if the exam can be sheduled at the i-th timeslot, false otherwhise.
 *
 * @var exam::conflicts
 * Bool array, describing a list of adjacencies for conflicting exams in
 * which true means there is a conflict with the i-th exam, false otherwhise.
 *
 * @var exam::deps
 * List of prerequisites.
 *
 * @var exam::deps_size
 * Size of list of prerequisites.
 */
typedef struct {
    uint16_t  exam_id;

    uint8_t   faculty;

    uint32_t  teacher_id;

    uint32_t  *students;
    uint16_t  enrollment;

    room_type room_type;
    uint16_t room_id;

    uint8_t timeslot;

    bool *availabilities;

    bool *conflicts;
    uint16_t *deps;
    uint8_t deps_size;
} exam;

/**
 * @struct array_exams
 * @brief This structure represents an array of exams.
 *
 * @var array_exams::data
 * Pointer on the exam datas.
 *
 * @var array_exams::size
 * Size of the array pointed by @see data.
 */
typedef struct {
    exam **data;
    size_t size;
} array_exams;

/**
 * Makes allocation and initialization of a new room with the specified
 * parameters. Sets all the entries of the assignation array to UINT16_MAX,
 * means the room is not assigned for each timeslot.
 *
 * @param id The room ID.
 * @param type The room type.
 * @param capacity The capacity of the room.
 * @param faculty The faculty in charge of the room.
 * @return A struct room allocated and initialized.
 */
room *init_room(uint16_t id, room_type type, uint16_t capacity,
                uint8_t faculty, uint8_t max_timeslot);

array_rooms *init_array_rooms(uint16_t rooms_size, room **rooms);

/**
 * Following our 3-dim model containing rooms by faculty and type, we need
 * to know the limit of each array of rooms. This provides the limits
 * for each rooms categorized by faculty (1st dimension) and type (2nd
 * dimension).
 *
 * @param faculty_size Number of faculties.
 * @param rooms An array of room (struct array_rooms).
 * @return Limits for the 3-dimensional array containing categorized rooms.
 */
size_t **get_rooms_sizes(uint8_t faculty_size, array_rooms *rooms);

/**
 * Sorts the rooms in a 3-dim array, categorized by faculty and type.
 * The 1st dimension is for the faculty, the 2nd dimension for the type and
 * the 3rd contains an array of rooms verifying this faculty and type.
 *
 * @param faculty_size Number of faculties.
 * @param rooms An array of room (struct array_rooms).
 * @param rooms_limits Limits of the different arrays of room (3rd dimension).
 * @return A sorted 3-dim array containing the rooms (struct matrix_rooms).
 */
matrix_rooms *get_rooms_matrix(uint8_t faculty_size, array_rooms *rooms, size_t **rooms_limits);

/**
 * Frees all the memory used by a room r.
 *
 * @param r Pointer to the room to free.
 */
void free_room(room *r);

/**
 * Frees each room of the given array, and the array itself.
 *
 * @param rooms Array of rooms (struct array_rooms).
 * @param rooms_size Number of rooms, also size of rooms.
 */
void free_rooms(array_rooms *rooms);

/**
 * Makes the allocation and initialization of an exam. The parameters are
 * length variable, to have this function dynamic. It takes, in this order,
 * the 'exam id', the 'teacher id', the 'number of enrollment' followed by
 * the specified number of enrollment, the 'number of availabilities'
 * followed by the specified number of availabilities and the 'number of
 * exams'.
 *
 * @param id The exam id.
 * @return A struct exam allocated and initialized.
 */

array_exams *init_array_exams(uint16_t exams_size, exam **exams);

exam *init_exam(uint16_t exam_id, uint8_t faculty, uint32_t teacher_id,
                uint32_t *students, uint16_t enrollment, room_type type,
                bool *availabilities, uint8_t max_timeslot, uint16_t exams_size,
                uint16_t *deps, uint8_t deps_size);

/**
 * Frees all the memory used by a room r.
 *
 * @param r Pointer to the room to free.
 */
void free_exam(exam *e);

/**
 * Frees each exam of the given array, and the array itself.
 *
 * @param exams Array of exams (struct array_exams).
 * @param exams_size Number of exams, also size of exams.
 */
void free_exams(array_exams *exams);

#endif /*STRUCTS_H_*/
