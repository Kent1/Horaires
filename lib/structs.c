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

array_exams *init_array_exams(uint16_t exams_size) {
    array_exams *exams = malloc(sizeof(array_exams));
    exams->data = malloc(exams_size * sizeof(exam *));
    exams->size = exams_size;

    return exams;
}

exam *init_exam(uint16_t exam_id, uint8_t faculty, uint32_t teacher_id,
                uint16_t enrollment, room_type type, uint16_t exams_size) {

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
    new_exam->students       = NULL;
    new_exam->availabilities = NULL;

    return new_exam;
}

void set_students_to_exam(exam *exam_, uint32_t *students) {
    exam_->students = malloc(exam_->enrollment * sizeof(uint32_t));

    for (uint16_t i = 0; i < exam_->enrollment; i++) {
        exam_->students[i] = students[i];
    }
}

void set_availabilities_to_exam(exam *exam_, bool *availabilities,
                                uint8_t max_timeslot) {
    exam_->students = malloc(max_timeslot * sizeof(bool));

    for (uint8_t i = 0; i < max_timeslot; i++) {
        exam_->availabilities[i] = availabilities[i];
    }
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
