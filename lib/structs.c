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
    new_room->room_id  = id;
    new_room->type     = type;
    new_room->capacity = capacity;
    new_room->faculty  = faculty;
    /* Allocates an array such that for each input (corresponding to a timeslot),
       the constant NOT_ASSIGNED at initial time.*/
    new_room->assignation = malloc(max_timeslot * sizeof(uint16_t));

    for (int i = 0; i < max_timeslot; i++)
        new_room->assignation[i] = NOT_ASSIGNED;

    return new_room;
}

room *clone_room(room *room_, uint8_t max_timeslot) {
    room *clone = malloc(sizeof(room));

    clone->room_id  = room_->room_id;
    clone->type     = room_->type;
    clone->capacity = room_->capacity;
    clone->faculty  = room_->faculty;
    /* Allocates an array such that for each input (corresponding to a timeslot),
       the constant NOT_ASSIGNED at initial time.*/
    clone->assignation = malloc(max_timeslot * sizeof(uint16_t));

    for (int i = 0; i < max_timeslot; i++)
        clone->assignation[i] = room_->assignation[i];

    return clone;
}

array_rooms *init_array_rooms(uint16_t rooms_size, room **rooms) {
    array_rooms *array = malloc(sizeof(array_rooms));
    array->data = malloc(rooms_size * sizeof(room *));
    array->size = rooms_size;

    for(uint16_t i = 0; i < rooms_size; i++)
        array->data[i] = rooms[i];

    return array;
}

array_rooms *clone_array_rooms(array_rooms *rooms, uint8_t max_timeslot) {
    array_rooms *clone = malloc(sizeof(array_rooms));
    clone->data = malloc(rooms->size * sizeof(room *));
    clone->size = rooms->size;

    for(uint16_t i = 0; i < rooms->size; i++)
        clone->data[i] = clone_room(rooms->data[i], max_timeslot);

    return clone;
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
    room ****rooms_3d = malloc(faculty_size * sizeof(room***));

    // Allocates/Initializes the other dimensions
    for (uint8_t i = 0; i < faculty_size; i++) {
        counters[i] = calloc(MAX_ROOM_TYPE, sizeof(size_t));

        rooms_3d[i] = malloc(MAX_ROOM_TYPE * sizeof(room**));
        for (uint8_t j = 0; j < MAX_ROOM_TYPE; j++)
            rooms_3d[i][j] = malloc(rooms_limits[i][j] * sizeof(room*));
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

matrix_rooms *clone_matrix_rooms(matrix_rooms *rooms, uint8_t max_timeslot, uint8_t faculty_size, uint8_t max_room_type) {
    // Allocation of the struct matrix
    matrix_rooms *clone = malloc(sizeof(matrix_rooms));
    size_t **clone_size = malloc(faculty_size * sizeof(size_t*));
    // rooms_matrix initializes the 3-dim array
    room ****rooms_3d = malloc(faculty_size * sizeof(room***));

    // Allocates/Initializes the other dimensions
    for (uint8_t i = 0; i < faculty_size; i++) {
        clone_size[i] = malloc(max_room_type * sizeof(size_t));

        rooms_3d[i] = malloc(max_room_type * sizeof(room**));

        for (uint8_t j = 0; j < max_room_type; j++) {
            clone_size[i][j] = rooms->size[i][j];
            rooms_3d[i][j] = malloc(rooms->size[i][j] * sizeof(room*));

            for (size_t k = 0; k < rooms->size[i][j]; k++) {
                rooms_3d[i][j][k] = clone_room(rooms->data[i][j][k], max_timeslot);
            }
        }
    }

    clone->data = rooms_3d;
    clone->size = clone_size;

    return clone;
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


void free_matrix_rooms(matrix_rooms *rooms, uint8_t faculty_size, uint8_t max_room_type) {
    for(uint8_t i = 0; i < faculty_size; i++) {
        for(uint8_t j = 0; j < max_room_type; j++) {
            for(uint16_t k = 0; k < rooms->size[i][j]; k++)
                free_room(rooms->data[i][j][k]);
            free(rooms->data[i][j]);
        }
        free(rooms->size[i]);
        free(rooms->data[i]);
    }
    free(rooms->size);
    free(rooms->data);
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
                uint16_t *deps, uint8_t deps_size) {

    exam *new_exam = malloc(sizeof(exam));
    new_exam->exam_id = exam_id;
    new_exam->faculty = faculty;
    new_exam->teacher_id = teacher_id;
    new_exam->enrollment = enrollment;
    new_exam->room_type  = type;

    // Default values for the rest
    new_exam->conflicts = malloc(exams_size * sizeof(bool));
    new_exam->room_id   = NOT_ASSIGNED;
    new_exam->timeslot  = NOT_SCHEDULED;
    new_exam->deps_size = deps_size;

    // Fills arrays with a proper copy
    new_exam->students = malloc(enrollment * sizeof(uint32_t));

    for (uint16_t i = 0; i < new_exam->enrollment; i++) {
        new_exam->students[i] = students[i];
    }

    new_exam->availabilities = malloc(max_timeslot * sizeof(bool));

    for (uint8_t i = 0; i < max_timeslot; i++) {
        new_exam->availabilities[i] = availabilities[i];
    }

    new_exam->deps = malloc(deps_size * sizeof(uint16_t));
    for (uint8_t i = 0; i < deps_size; i++) {
        new_exam->deps[i] = deps[i];
    }
    return new_exam;
}

exam *clone_exam(exam *exam_, uint16_t exams_size, uint8_t max_timeslot) {
    exam *clone = malloc(sizeof(exam));

    clone->exam_id    = exam_->exam_id;
    clone->faculty    = exam_->faculty;
    clone->teacher_id = exam_->teacher_id;
    clone->enrollment = exam_->enrollment;
    clone->room_type  = exam_->room_type;
    clone->room_id    = exam_->room_id;
    clone->timeslot   = exam_->timeslot;
    clone->deps_size  = exam_->deps_size;

    // Fills arrays with a proper copy
    clone->conflicts = malloc(exams_size * sizeof(bool));

    for (uint16_t i = 0; i < exams_size; i++) {
        clone->conflicts[i] = exam_->conflicts[i];
    }

    clone->students = malloc(exam_->enrollment * sizeof(uint32_t));

    for (uint16_t i = 0; i < exam_->enrollment; i++) {
        clone->students[i] = exam_->students[i];
    }

    clone->availabilities = malloc(max_timeslot * sizeof(bool));

    for (uint8_t i = 0; i < max_timeslot; i++) {
        clone->availabilities[i] = exam_->availabilities[i];
    }

    clone->deps = malloc(exam_->deps_size * sizeof(uint16_t));
    for (uint8_t i = 0; i < exam_->deps_size; i++) {
        clone->deps[i] = exam_->deps[i];
    }
    return clone;
}

array_exams *clone_array_exams(array_exams *exams, uint8_t max_timeslot) {
    array_exams *clone = malloc(sizeof(array_exams));
    clone->data = malloc(exams->size * sizeof(exam *));
    clone->size = exams->size;

    for(uint16_t i = 0; i < clone->size; i++)
        clone->data[i] = clone_exam(exams->data[i], clone->size, max_timeslot);

    return clone;
}

void free_exam(exam *e) {
    // Cannot free a pointer non-initialized, 'cause still not handled
    free(e->deps);
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
