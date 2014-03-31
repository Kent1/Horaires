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
    room ** **data;
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
    uint16_t  room_id;

    uint8_t   timeslot;

    bool *availabilities;

    bool *conflicts;

    uint16_t *deps;
    uint8_t  deps_size;
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
 * @param max_timeslot Maximum timeslot available.
 * @return A struct room allocated and initialized.
 */
room *init_room(uint16_t id, room_type type, uint16_t capacity,
                uint8_t faculty, uint8_t max_timeslot);

/**
 * Clone a room and all its specifications.
 *
 * @param room Room to be cloned.
 * @param max_timeslot Maximum timeslot available.
 *
 * @return A pointer of the cloned room.
 */
room *clone_room(room *room, uint8_t max_timeslot);

/**
 * Clone an array of rooms (the rooms are also cloned).
 *
 * @param rooms An array of room (struct array_rooms).
 * @param max_timeslot Maximum timeslot available.
 *
 * @return A pointer of the cloned array.
 */
array_rooms *clone_array_rooms(array_rooms *rooms, uint8_t max_timeslot);

/**
 * Initialize an array of rooms (struct array_rooms).
 *
 * @param rooms_size Number of rooms.
 * @param rooms Array of pointer of each room's pointer.
 *
 * @return A pointer of the struct array_rooms initialized.
 */
array_rooms *init_array_rooms(uint16_t rooms_size, room **rooms);

/**
 * Following our 3-dim model containing rooms by faculty and type, we need
 * to know the limit of each array of rooms. This provides the limits
 * for each rooms categorized by faculty (1st dimension) and type (2nd
 * dimension).
 *
 * @param faculty_size Number of faculties.
 * @param max_room_type Maximum number of room type.
 * @param rooms An array of room (struct array_rooms).
 * @return Limits for the 3-dimensional array containing categorized rooms.
 */
size_t **get_rooms_sizes(uint8_t faculty_size, uint8_t max_room_type, array_rooms *rooms);

/**
 * Sorts the rooms in a 3-dim array, categorized by faculty and type.
 * The 1st dimension is for the faculty, the 2nd dimension for the type and
 * the 3rd contains an array of rooms verifying this faculty and type.
 *
 * @param faculty_size Number of faculties.
 * @param max_room_type Maximum number of room type.
 * @param rooms An array of rooms (struct array_rooms).
 * @param rooms_limits Limits of the different arrays of room (3rd dimension).
 * @return A sorted 3-dim array containing the rooms (struct matrix_rooms).
 */
matrix_rooms *get_rooms_matrix(uint8_t faculty_size, uint8_t max_room_type, array_rooms *rooms,
                               size_t **rooms_limits);

/**
 * Clone a struct matrix_rooms and its specifications.
 *
 * @param rooms Struct matrix_rooms to be cloned.
 * @param max_timeslot Maximum timeslot available.
 * @param faculty_size Number of faculties.
 * @param max_room_type Maximum different type of room.
 *
 * @return A pointer of the cloned matrix_rooms.
 */
matrix_rooms *clone_matrix_rooms(matrix_rooms *rooms, uint8_t max_timeslot,
                                 uint8_t faculty_size, uint8_t max_room_type);

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
 */
void free_rooms(array_rooms *rooms);

/**
 * Frees the pointer of struct matrix_rooms. This function frees also the rooms
 * (using the free_room function).
 *
 * @param rooms Struct matrix_rooms to be freed.
 * @param faculty_size Number of faculties.
 * @param max_room_type Maximum different type of room.
 */
void free_matrix_rooms(matrix_rooms *rooms, uint8_t faculty_size,
                       uint8_t max_room_type);

/**
 * This function initializes an array of examinations (struct array_exams).
 *
 * @param  exams_size Number of exminations (size of the exams array).
 * @param  exams Array of pointer of each pointer of examination.
 *
 * @return A pointer of the array exam initialized (struct array_exams).
 */
array_exams *init_array_exams(uint16_t exams_size, exam **exams);

/**
 * This function initalizes a struct exam with the specifications
 * given in parameters.
 *
 * @param exam_id ID of the examination.
 * @param faculty ID of the faculty offering the examination.
 * @param teacher_id ID (reference number) of the professor giving the
 *                 examination.
 * @param students Array of students enrolled in the examination.
 * @param enrollment Number of students enrolled in the examination.
 * @param type Type of the room needed for the examination.
 * @param availabilities Array of boolean specifying the availabilities
 *                       of the professor (true for available).
 * @param max_timeslot Maximum timeslot available
 *                    (size of the availabilities array).
 * @param exams_size Number of examinations to schedule
 *                   (size of the conflict array).
 * @param deps Array of dependencies (prerequisite) for this examination.
 *             The dependencies is specified by the examination's ID.
 * @param deps_size Number of dependencies (size of the deps array).
 */
exam *init_exam(uint16_t exam_id, uint8_t faculty, uint32_t teacher_id,
                uint32_t *students, uint16_t enrollment, room_type type,
                bool *availabilities, uint8_t max_timeslot, uint16_t exams_size,
                uint16_t *deps, uint8_t deps_size);

/**
 * This function clones a struct exam and its specifications.
 *
 * @param  exam_ Examination to be cloned (struct exam).
 * @param  exams_size Number of exams this session.
 * @param max_timeslot Maximum timeslot available.
 *
 * @return A pointer of the examination (struct exam) cloned.
 */
exam *clone_exam(exam *exam_, uint16_t exams_size, uint8_t max_timeslot);

/**
 * The function clones a struct array_exams and its specifications.
 *
 * @param  exams Array of exams (struct array_exams) to be cloned.
 * @param max_timeslot Maximum timeslot available.
 *
 * @return A pointer of the array of exams (struct array_exams) cloned.
 */
array_exams *clone_array_exams(array_exams *exams, uint8_t max_timeslot);

/**
 * Frees all the memory used by a room r.
 *
 * @param e Pointer to the room to free.
 */
void free_exam(exam *e);

/**
 * Frees each exam of the given array, and the array itself.
 *
 * @param exams Array of exams (struct array_exams).
 */
void free_exams(array_exams *exams);

#endif /*STRUCTS_H_*/
