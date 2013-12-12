#include "structs.h"

const uint8_t MAX_TIMESLOT = 5;
const uint16_t MAX_EXAM = 8;
exam *exam1, *exam2, *exam3, *exam4, *exam5, *exam6, *exam7, *exam8;
array_exams *exams;

void init_test_exam(void) {
    // exam1 - Analyse
    exam1 = init_exam(1, 555000,                      // exam id, teacher id
                      3, 10000, 10001, 10002,         // nb + enrollments
                      MAX_TIMESLOT, 1, 1, 1, 0, 0, // nb + availabilities
                      MAX_EXAM, classroom);        // nb of exams + room type

    // exam2 - Fonctionnement des ordis
    exam2 = init_exam(2, 555001,
                      2, 10000, 10003,
                      MAX_TIMESLOT, 0, 1, 1, 0, 0,
                      MAX_EXAM, classroom);

    // exam3 - MATH1
    exam3 = init_exam(3, 555002,
                      2, 10002, 10004,
                      MAX_TIMESLOT, 0, 1, 1, 1, 0,
                      MAX_EXAM, classroom);

    // exam4 - Anglais
    exam4 = init_exam(4, 555003,
                      4, 10003, 10004, 10005, 10006,
                      MAX_TIMESLOT, 0, 0, 1, 0, 0,
                      MAX_EXAM, classroom);

    // exam5 - Physique
    exam5 = init_exam(5, 555001,
                      3, 10000, 10001, 10003,
                      MAX_TIMESLOT, 0, 0, 1, 1, 0,
                      MAX_EXAM, lab);

    // exam6 - Chimie
    exam6 = init_exam(6, 555001,
                      1, 10004,
                      MAX_TIMESLOT, 0, 0, 0, 1, 1,
                      MAX_EXAM, lab);

    // exam7 - Algèbre
    exam7 = init_exam(7, 555005,
                      1, 10001,
                      MAX_TIMESLOT, 0, 0, 0, 0, 1,
                      MAX_EXAM, classroom);
    // exam8 - Univers : S & R
    exam8 = init_exam(8, 555006,
                      1, 10002,
                      MAX_TIMESLOT, 1, 1, 0, 0, 1,
                      MAX_EXAM, classroom);
}

void clean_test_exam(void) {
    free_exam(exam1);
    free_exam(exam2);
    free_exam(exam3);
    free_exam(exam4);
    free_exam(exam5);
    free_exam(exam6);
    free_exam(exam7);
    free_exam(exam8);
}

void clean_test_exams(void) {
    free_exams(exams);
}

