/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 11/12/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "structs.h"
/**
 * Makes allocation for an array of students given in
 * parameters. This parameter is variable length to be flexible.
 *
 * @param enrollment Number of students, also size of the array.
 * @param para A length variable parameter containing the students.
 * @return An array containing all the given parameters
 */
static uint32_t *init_students(int enrollment, va_list *para);

/**
 * Makes allocation for an array of availabilities given in
 * parameters.This parameter is variable length to be flexible.
 *
 * @param max_timeslot Number of timeslots, also size of the array.
 * @param para A length variable parameter containing the availabilities.
 * @return An array containing all the given parameters.
 */
static bool *init_availabilities(int max_timeslot, va_list *para);

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

room *init_rooms(int rooms_size, ...) {
    room *rooms = malloc(rooms_size * sizeof(room));

    va_list para;
    va_start(para, rooms_size);

    for (int i = 0; i < rooms_size; i++)
        rooms[i] = *va_arg(para, room *);

    va_end(para);

    return rooms;
}

void free_room(room *r) {
    free(r->assignation);
    free(r);
}

/* Functions associated with the type exam */

static uint32_t *init_students(int enrollment, va_list *para) {
    uint32_t *students_list = malloc(enrollment * sizeof(uint32_t));

    for (int i = 0; i < enrollment; i++)
        students_list[i] = va_arg(*para, uint32_t);

    return students_list;
}

static bool *init_availabilities(int max_timeslot, va_list *para) {
    bool *availabilities_list = malloc(max_timeslot * sizeof(bool));

    for (int i = 0; i < max_timeslot; i++)
        availabilities_list[i] = (uint8_t) va_arg(*para, uint32_t);

    return availabilities_list;
}

exam *init_exam(uint16_t id, ...) {
    va_list para;
    va_start(para, id);

    // Initializes non-changing values
    exam *new_exam = malloc(sizeof(exam));
    new_exam->exam_id  = id;
    new_exam->room_id  = UINT16_MAX;
    new_exam->timeslot = NOT_SCHEDULED; // Default value for non-assigned exam

    new_exam->teacher_id = (uint32_t) va_arg(para, int);
    new_exam->enrollment = (uint16_t) va_arg(para, int);

    new_exam->students       = init_students(new_exam->enrollment, &para);
    new_exam->availabilities = init_availabilities(va_arg(para, int), &para);

    new_exam->conflicts = calloc(va_arg(para, int), sizeof(bool));
    new_exam->room_type = va_arg(para, room_type);

    va_end(para);

    return new_exam;
}

exam *init_exams(int exams_size, ...) {
    exam *exams = malloc(exams_size * sizeof(exam));

    va_list para;
    va_start(para, exams_size);

    for (int i = 0; i < exams_size; i++)
        exams[i] = *va_arg(para, exam *);

    va_end(para);

    return exams;
}

void free_exam(exam *e) {
    // Cannot free a pointer non-initialized, 'cause still not handled
    //free(e->deps);
    free(e->students);
    free(e->availabilities);
    free(e->conflicts);
    free(e);
}
