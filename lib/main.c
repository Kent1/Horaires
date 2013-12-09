/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 29/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "heuristics.h"
#include "conflict.h"

// This macro let you choose an example between [1,2]
#define EXAMPLE 1

// Following the macro above, defines some parameters
// concerning the chosen example
#if EXAMPLE == 1
    #define MAX_TIMESLOT 5
    #define MAX_EXAM 8
#endif
#if EXAMPLE == 2
    #define MAX_TIMESLOT 2
    #define MAX_EXAM 2
#endif

/**
 * Make allocation for an array of students given in
 * parameters but with a variable length to be flexible.
 *
 * @param size Size of the array
 * @return An array containing all the given parameters
 */
uint32_t* init_students(int size, ...) {
    int i;
    uint32_t *new_tab = calloc(size, sizeof(uint32_t));

    va_list para;
    va_start(para, size);

    for (i=0; i < size; i++)
        new_tab[i] = va_arg(para, uint32_t);;

    va_end(para);

    return new_tab;
}

/**
 * Make allocation for an array of availabilities given in
 * parameters but with a variable length to be flexible.
 *
 * @param size Size of the array
 * @return An array containing all the given parameters
 */
uint8_t* init_availabilities(int size, ...) {
    int i;
    uint8_t *new_tab = calloc(size, sizeof(uint8_t));

    va_list para;
    va_start(para, size);

    for (i=0; i < size; i++)
        new_tab[i] = (uint8_t) va_arg(para, uint32_t);

    va_end(para);

    return new_tab;
}

/**
 * Make allocation for an array of conflicts initialized to 0.
 *
 * @param size Size of the array
 * @return An array containing 0s.
 */
uint16_t* init_conflicts(int size) {
    return calloc(size, sizeof(uint16_t));
}

/**
 * Make allocation for an array of exams given in
 * parameters but with a variable length to be flexible.
 *
 * @param size Size of the array
 * @return An array containing all the given parameters
 */
exam* init_exams(int size, ...) {
    int i;
    exam *exams = calloc(size, sizeof(exam));

    va_list para;
    va_start(para, size);

    for (i=0; i < size; i++)
        exams[i] = *va_arg(para, exam*);

    va_end(para);

    return exams;
}

// Simpler method, but still bugs
/*exam* init_exam(uint16_t id, ...) {
    int i;
    va_list para;
    va_start(para, id);

    exam *exam = calloc(1, sizeof(exam));
    exam->exam_id    = id;
    exam->teacher_id = (uint32_t) va_arg(para, int);

    exam->timeslot = 0; // Default value for non-assigned exam

    exam->enrollment = (uint16_t) va_arg(para, int);
    // problem with the two lines below
    uint32_t *test = init_students2(exam->enrollment, &para);
    exam->students = test;

    exam->availabilities = init_availabilities2(va_arg(para, int), &para);

    exam->conflicts = init_conflicts(va_arg(para, int));

    return exam;
}*/

/**
 * Creates and initializes an problem instance, having a solution.
 *
 * @return An array of exams
 */
exam* get_example1() {
    // exam1 - Analyse
    exam *exam1 = calloc(1, sizeof(exam));
    exam1->exam_id = 1;
    exam1->teacher_id = 555000;

    exam1->timeslot = NOT_SCHEDULED;

    exam1->enrollment = 3;
    exam1->students = init_students(3, 10000, 10001, 10002);

    exam1->availabilities = init_availabilities(MAX_TIMESLOT, 1, 1, 1, 0, 0, 0);
    exam1->conflicts = init_conflicts(MAX_EXAM);

    // exam2 - Fonctionnement des ordis
    exam *exam2 = calloc(1, sizeof(exam));
    exam2->exam_id = 2;
    exam2->teacher_id = 555001;

    exam2->timeslot = NOT_SCHEDULED;

    exam2->enrollment = 2;
    exam2->students = init_students(2, 10000, 10003);

    exam2->availabilities = init_availabilities(MAX_TIMESLOT, 0, 1, 1, 0, 0, 0);
    exam2->conflicts = init_conflicts(MAX_EXAM);


    // exam3 - MATH1
    exam *exam3 = calloc(1, sizeof(exam));
    exam3->exam_id = 3;
    exam3->teacher_id = 555002;

    exam3->timeslot = NOT_SCHEDULED;

    exam3->enrollment = 2;

    exam3->students = init_students(2, 10002, 10004);

    exam3->availabilities = init_availabilities(MAX_TIMESLOT, 0, 1, 1, 1, 0, 0);
    exam3->conflicts = init_conflicts(MAX_EXAM);


    // exam4 - Anglais
    exam *exam4 = calloc(1, sizeof(exam));
    exam4->exam_id = 4;
    exam4->teacher_id = 555003;

    exam4->timeslot = NOT_SCHEDULED;

    exam4->enrollment = 4;
    exam4->students = init_students(4, 10003, 10004, 10005, 10006);

    exam4->availabilities = init_availabilities(MAX_TIMESLOT, 0, 0, 1, 0, 0, 0);
    exam4->conflicts = init_conflicts(MAX_EXAM);


    // exam5 - Anglais
    exam *exam5 = calloc(1, sizeof(exam));
    exam5->exam_id = 5;
    exam5->teacher_id = 555001;

    exam5->timeslot = NOT_SCHEDULED;

    exam5->enrollment = 3;
    exam5->students = init_students(3, 10000, 10001, 10003);

    exam5->availabilities = init_availabilities(MAX_TIMESLOT, 0, 0, 1, 1, 0, 0);
    exam5->conflicts = init_conflicts(MAX_EXAM);


    // exam6 - chimir
    exam *exam6 = calloc(1, sizeof(exam));
    exam6->exam_id = 6;
    exam6->teacher_id = 555004;

    exam6->timeslot = NOT_SCHEDULED;

    exam6->enrollment = 1;
    exam6->students = init_students(1, 10004);

    exam6->availabilities = init_availabilities(MAX_TIMESLOT, 0, 0, 0, 1, 1, 1);
    exam6->conflicts = init_conflicts(MAX_EXAM);


    // exam7 - algèbre
    exam *exam7 = calloc(1, sizeof(exam));
    exam7->exam_id = 7;
    exam7->teacher_id = 555005;

    exam7->timeslot = NOT_SCHEDULED;

    exam7->enrollment = 1;
    exam7->students = init_students(1, 10001);

    exam7->availabilities = init_availabilities(MAX_TIMESLOT, 0, 0, 0, 0, 1, 1);
    exam7->conflicts = init_conflicts(MAX_EXAM);

    // exam8 - jesaispaslire
    exam *exam8 = calloc(1, sizeof(exam));
    exam8->exam_id = 8;
    exam8->teacher_id = 555006;

    exam8->timeslot = NOT_SCHEDULED;

    exam8->enrollment = 1;
    exam8->students = init_students(1, 10002);

    exam8->availabilities = init_availabilities(MAX_TIMESLOT, 1, 1, 0, 0, 1, 1);
    exam8->conflicts = init_conflicts(MAX_EXAM);


    return init_exams(MAX_EXAM, exam1, exam2, exam3, exam4, exam5, exam6, exam7, exam8);
}

/**
 * Creates and initializes an problem instance, having a solution.
 *
 * @return An array of exams
 */
exam* get_example2() {
    // exam1 - Analyse
    /*
    int number_of_exams = 2;
    exam *exam1 = init_exam(1, 555000,                  // exam id, teacher id
                            3, 10000, 10001, 10002,    // nb + enrollments
                            2, 1, 0,                    // nb + availabilities
                            number_of_exams);           // nb of exams
    */

    exam *exam1 = calloc(1, sizeof(exam));
    exam1->exam_id = 1;
    exam1->teacher_id = 555000;

    exam1->timeslot = NOT_SCHEDULED;

    exam1->enrollment = 3;
    exam1->students = init_students(3, 10000, 10001, 10002);

    exam1->availabilities = init_availabilities(MAX_TIMESLOT, 1, 0);

    exam1->conflicts = init_conflicts(MAX_EXAM);

    // exam2 - Fonctionnement des ordis
    exam *exam2 = calloc(1, sizeof(exam));
    exam2->exam_id = 2;
    exam2->teacher_id = 555001;

    exam2->timeslot = NOT_SCHEDULED;

    exam2->enrollment = 2;
    exam2->students = init_students(2, 10000, 10003);

    exam2->availabilities = init_availabilities(MAX_TIMESLOT, 1, 1);
    exam2->conflicts = init_conflicts(MAX_EXAM);

    return init_exams(MAX_EXAM, exam1, exam2);
}

/**
 * Calls the right example following the macro EXAMPLE.
 *
 * @return An array of exams
 */
exam* get_example() {
    switch (EXAMPLE) {
    case 1:
        return get_example1();
    case 2:
        return get_example2();
    }
    return NULL;
}

/**
 * Prints which timeslot have been attributed to each exams.
 *
 * @param exams An array of scheduled exams
 */
void print_summary_schedule(exam *exams) {
    int i;

    printf("Summary\n");
    printf("=======\n");

    for (i = 0; i < MAX_EXAM; i++) {
        printf("Exam %d : %d\n", i+1, exams[i].timeslot);
    }
    printf("\n");
}

/**
 * Prints which detailed information from each exams,
 * for each timeslot available.
 *
 * @param exams An array of scheduled exams
 */
void print_detailed_schedule(exam *exams) {
    int i, j, k;

    printf("Detailed schedule\n");
    printf("=================\n");

    for (i = 0; i < MAX_TIMESLOT; i++) {
        printf("Timeslot %d\n", i+1);
        printf("------------\n\n");

        for (j = 0; j < MAX_EXAM; j++) {
            if(exams[j].timeslot == i+1) {
                printf("  Exam %d :\n", j+1);
                printf("      -> Prof : %d\n", exams[j].teacher_id);

                printf("      -> Timeslots available : (");
                for (k = 0; k < MAX_TIMESLOT; k++) {
                    printf("%d ", exams[j].availabilities[k]);
                }
                printf(")\n");

                printf("      -> Conflicts detected : (");
                for (k = 0; k < MAX_EXAM; k++) {
                    printf("%d ", exams[j].conflicts[k]);
                }
                printf(")\n");

                printf("      -> Students :\n");
                for (k = 0; k < exams[j].enrollment; k++) {
                    printf("            %d\n", exams[j].students[k]);
                }
            }
        }
    }
}

/**
 * Main function, execute the heuristics on a simple example.
 *
 */
int main() {
    // Collect a sample of exams
    exam *exams = get_example();

    // Preprocessing to the coloring graph heuristics
    compute_conflicts(exams, MAX_EXAM);

    // Main heuristic
    bool a = color_graph_backtrack(exams, MAX_EXAM, MAX_TIMESLOT);

    printf("%s\n", (a == true) ? "A schedule has been found!\n" :
                                    "No schedule has been found!\n");
    // Some outputs
    if(a == true) {
        print_summary_schedule(exams);

        //print_detailed_schedule(exams);
    }

    return 0;
}
