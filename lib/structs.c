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

array_rooms *init_array_rooms(uint16_t rooms_size) {
    array_rooms *rooms = malloc(sizeof(array_rooms));
    rooms->data = malloc(rooms_size * sizeof(room *));
    rooms->size = rooms_size;

    return rooms;
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

array_exams *init_array_exams(uint16_t exams_size, exam *exams) {
    array_exams *array = malloc(sizeof(array_exams));
    array->data = malloc(exams_size * sizeof(exam *));
    array->size = exams_size;

    for(uint16_t i = 0; i < exams_size; i++) {
        array->data[i] = &exams[i];
    }

    return array;
}

exam *init_exam(uint16_t exam_id, uint8_t faculty, uint32_t teacher_id,
                uint32_t *students, uint16_t enrollment, room_type type,
                bool *availabilities, uint16_t exams_size, uint8_t max_timeslot) {

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
    new_exam->deps           = NULL;

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
