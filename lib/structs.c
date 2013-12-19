/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 11/12/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "structs.h"

/* Functions associated with the type room */

room *init_room(uint16_t id, room_type type, uint16_t capacity,
                uint8_t faculty, uint8_t max_timeslot) {
    room *new_room = malloc(sizeof(room));

    // Initializes non-changing values
    new_room->room_id     = id;
    new_room->type        = type;
    new_room->capacity    = capacity;
    new_room->faculty     = faculty;
    /* Allocates an array such that for each input (corresponding to a timeslot),
       the constant NOT_ASSIGNED at initial time.*/
    new_room->assignation = malloc(max_timeslot * sizeof(uint16_t));

    for (int i = 0; i < max_timeslot; i++)
        new_room->assignation[i] = NOT_ASSIGNED;

    return new_room;
}

array_rooms *init_array_rooms(uint16_t rooms_size, room **rooms) {
    array_rooms *array = malloc(sizeof(array_rooms));
    array->data = malloc(rooms_size * sizeof(room *));
    array->size = rooms_size;

    for(uint16_t i = 0; i < rooms_size; i++)
        array->data[i] = rooms[i];

    return array;
}

size_t **get_rooms_sizes(uint8_t faculty_size, array_rooms *rooms) {

    // Allocation of the 2-dim array
    size_t **rooms_limits = malloc(faculty_size * sizeof(size_t *));

    for(uint8_t f = 0; f < faculty_size; f++)
        rooms_limits[f] = calloc(MAX_ROOM_TYPE, sizeof(size_t));

    // Filling the limits in this 2-dim array
    for(uint8_t f = 0; f < rooms->size; f++)
        rooms_limits[rooms->data[f]->faculty][rooms->data[f]->type]++;

    return rooms_limits;
}

matrix_rooms *get_rooms_matrix(uint8_t faculty_size, array_rooms *rooms, size_t **rooms_limits) {
    // Allocation of the struct matrix
    matrix_rooms *matrix = malloc(sizeof(matrix_rooms));
    // Allocation of an array of counters for each array of rooms (3rd dimension)
    size_t **counters  = malloc(faculty_size * sizeof(size_t *));
    // rooms_matrix initializes the 3-dim array
    room ****rooms_3d = malloc(faculty_size * sizeof(room**));

    // Allocates/Initializes the other dimensions
    for (uint8_t i = 0; i < faculty_size; i++) {
        counters[i] = calloc(MAX_ROOM_TYPE, sizeof(size_t));

        rooms_3d[i] = malloc(MAX_ROOM_TYPE * sizeof(room*));
        for (uint8_t j = 0; j < MAX_ROOM_TYPE; j++)
            rooms_3d[i][j] = malloc(rooms_limits[i][j] * sizeof(room));
    }

    // Fills the 3-dim array using the counters
    for (uint8_t i = 0; i < rooms->size; i++) {
        uint16_t index = counters[rooms->data[i]->faculty][rooms->data[i]->type]++;
        rooms_3d[rooms->data[i]->faculty][rooms->data[i]->type][index] = rooms->data[i];
    }

    // Frees counters
    for (uint8_t i = 0; i < faculty_size; i++)
        free(counters[i]);
    free(counters);
    // Frees the array_rooms and its array only, not the datas
    free(rooms->data);
    free(rooms);

    matrix->data = rooms_3d;
    matrix->size = rooms_limits;

    return matrix;
}

void free_room(room *r) {
    free(r->assignation);
    free(r);
}

void free_rooms(array_rooms *rooms) {
    for (int i = 0; i < rooms->size; i++)
        free_room(rooms->data[i]);

    free(rooms);
}

/* Functions associated with the type exam */

array_exams *init_array_exams(uint16_t exams_size, exam **exams) {
    array_exams *array = malloc(sizeof(array_exams));
    array->data = malloc(exams_size * sizeof(exam *));
    array->size = exams_size;

    for(uint16_t i = 0; i < exams_size; i++)
        array->data[i] = exams[i];

    return array;
}

exam *init_exam(uint16_t exam_id, uint8_t faculty, uint32_t teacher_id,
                uint32_t *students, uint16_t enrollment, room_type type,
                bool *availabilities, uint8_t max_timeslot, uint16_t exams_size,
                uint16_t *deps) {

    exam *new_exam = malloc(sizeof(exam));
    new_exam->exam_id = exam_id;
    new_exam->faculty = faculty;
    new_exam->teacher_id = teacher_id;
    new_exam->enrollment = enrollment;
    new_exam->room_type  = type;

    // Default values for the rest
    new_exam->conflicts      = malloc(exams_size * sizeof(bool));
    new_exam->room_id        = NOT_ASSIGNED;
    new_exam->timeslot       = NOT_SCHEDULED;
    new_exam->deps           = deps;

    // Fills arrays with a proper copy
    new_exam->students = malloc(new_exam->enrollment * sizeof(uint32_t));

    for (uint16_t i = 0; i < new_exam->enrollment; i++) {
        new_exam->students[i] = students[i];
    }

    new_exam->availabilities = malloc(max_timeslot * sizeof(bool));

    for (uint8_t i = 0; i < max_timeslot; i++) {
        new_exam->availabilities[i] = availabilities[i];
    }

    return new_exam;
}

void free_exam(exam *e) {
    // Cannot free a pointer non-initialized, 'cause still not handled
    //free(e->deps);
    free(e->students);
    free(e->availabilities);
    free(e->conflicts);
    free(e);
}

void free_exams(array_exams *exams) {
    for (int i = 0; i < exams->size; i++)
        free_exam(exams->data[i]);

    free(exams);
}
