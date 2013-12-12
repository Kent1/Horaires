/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 29/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "util.h"
#include "structs.h"
#include "heuristics.h"
#include "conflict.h"

// This macro let you choose an example between [1,2]
#define EXAMPLE 1

// Following the macro above, defines some parameters
// concerning the chosen example
#if EXAMPLE == 1
#define MAX_TIMESLOT 5
#define MAX_EXAM 8
#elif EXAMPLE == 2
#define MAX_TIMESLOT 2
#define MAX_EXAM 2
#endif

/**
 * Creates and initializes an problem instance, having a solution.
 *
 * @return An array of exams
 */
array_exams *get_example1() {
    // exam1 - Analyse
    exam *exam1 = init_exam(1, 555000,                      // exam id, teacher id
                            3, 10000, 10001, 10002,         // nb + enrollments
                            MAX_TIMESLOT, 1, 1, 1, 0, 0, // nb + availabilities
                            MAX_EXAM, classroom);        // nb of exams + room type

    // exam2 - Fonctionnement des ordis
    exam *exam2 = init_exam(2, 555001,
                            2, 10000, 10003,
                            MAX_TIMESLOT, 0, 1, 1, 0, 0,
                            MAX_EXAM, classroom);

    // exam3 - MATH1
    exam *exam3 = init_exam(3, 555002,
                            2, 10002, 10004,
                            MAX_TIMESLOT, 0, 1, 1, 1, 0,
                            MAX_EXAM, classroom);

    // exam4 - Anglais
    exam *exam4 = init_exam(4, 555003,
                            4, 10003, 10004, 10005, 10006,
                            MAX_TIMESLOT, 0, 0, 1, 0, 0,
                            MAX_EXAM, classroom);

    // exam5 - Anglais
    exam *exam5 = init_exam(5, 555001,
                            3, 10000, 10001, 10003,
                            MAX_TIMESLOT, 0, 0, 1, 1, 0,
                            MAX_EXAM, lab);

    // exam6 - chimir
    exam *exam6 = init_exam(6, 555004,
                            1, 10004,
                            MAX_TIMESLOT, 0, 0, 0, 1, 1,
                            MAX_EXAM, lab);

    // exam7 - algèbre
    exam *exam7 = init_exam(7, 555005,
                            1, 10001,
                            MAX_TIMESLOT, 0, 0, 0, 0, 1,
                            MAX_EXAM, classroom);
    // exam8 - jesaispaslire
    exam *exam8 = init_exam(8, 555006,
                            1, 10002,
                            MAX_TIMESLOT, 1, 1, 0, 0, 1,
                            MAX_EXAM, classroom);

    return init_exams(MAX_EXAM, exam1, exam2, exam3, exam4, exam5, exam6, exam7,
                      exam8);
}

/**
 * Creates and initializes an problem instance, having a solution.
 *
 * @return An array of exams
 */
array_exams *get_example2() {
    // exam1 - Analyse
    exam *exam1 = init_exam(1, 555000,               // exam id, teacher id
                            3, 10000, 10001, 10002,  // nb + enrollments
                            MAX_TIMESLOT, 1, 0,      // nb + availabilities
                            MAX_EXAM, classroom); // nb of exams

    // exam2 - Fonctionnement des ordis
    exam *exam2 = init_exam(2, 555001,
                            2, 10000, 10003,
                            MAX_TIMESLOT, 1, 1,
                            MAX_EXAM, classroom);

    return init_exams(MAX_EXAM, exam1, exam2);
}

/**
 * Calls the right example following the macro EXAMPLE.
 *
 * @return An array of exams
 */
array_exams *get_example() {
    switch (EXAMPLE) {
        case 1:
            return get_example1();

        case 2:
            return get_example2();
    }

    return NULL;
}

/**
 * Creates an array of rooms to use with the two first example.
 *
 * @return An array of rooms
 */
room *get_rooms() {
    // R1 - Salon bleu
    room *room1 = init_room(1, classroom, 1, 0, MAX_TIMESLOT);
    // R2 - Plisnier
    room *room2 = init_room(2, classroom, 2, 0, MAX_TIMESLOT);
    // R3 - Van Gogh
    room *room3 = init_room(3, classroom, 5, 0, MAX_TIMESLOT);
    // R4 - Pascal
    room *room4 = init_room(4, lab, 4, 0, MAX_TIMESLOT);

    return init_rooms(4, room3, room1, room2, room4);
}

/**
 * Following our 3-dim model containing rooms by faculty and type, we need
 * to know the limit of each array of rooms. This provides the limits
 * for each rooms categorized by faculty (1st dimension) and type (2nd
 * dimension).
 *
 * @param room_size Number of rooms, size of the parameter rooms.
 * @param faculty_size Number of faculties.
 * @param rooms An array of room of size room_size.
 * @return Limits for the 3-dimensional array containing categorized rooms.
 */
uint16_t **get_room_indices(uint16_t room_size, uint8_t faculty_size, room *rooms) {

    // Allocation of the 2-dim array
    uint16_t **rooms_limits = malloc(faculty_size * sizeof(uint16_t *));

    for(uint8_t f = 0; f < faculty_size; f++)
        rooms_limits[f] = calloc(MAX_ROOM_TYPE, sizeof(uint16_t));

    // Filling the limits in this 2-dim array
    for(uint8_t f = 0; f < room_size; f++)
        rooms_limits[rooms[f].faculty][rooms[f].type]++;

    return rooms_limits;
}

/**
 * Sorts the rooms in a 3-dim array, categorized by faculty and type.
 * The 1st dimension is for the faculty, the 2nd dimension for the type and
 * the 3rd contains an array of rooms verifying this faculty and type.
 *
 * @param room_size Number of rooms, size of the parameter rooms.
 * @param faculty_size Number of faculties.
 * @param rooms An array of room of size room_size.
 * @param rooms_limits Limits of the different arrays of room (3rd dimension).
 * @return A sorted 3-dim array containing the rooms.
 */
room ***get_rooms_matrix(uint16_t room_size, uint8_t faculty_size, room *rooms, uint16_t **rooms_limits) {
    // Allocation of an array of counters for each array of rooms (3rd dimension)
    uint16_t **counters  = malloc(faculty_size * sizeof(uint16_t *));
    // rooms_matrix initializes the 3-dim array
    room ***rooms_matrix = malloc(faculty_size * sizeof(room**));

    // Allocates/Initializes the other dimensions
    for (uint8_t i = 0; i < faculty_size; i++) {
        counters[i] = calloc(MAX_ROOM_TYPE, sizeof(uint16_t));

        rooms_matrix[i] = malloc(MAX_ROOM_TYPE * sizeof(room*));
        for (uint8_t j = 0; j < MAX_ROOM_TYPE; j++)
            rooms_matrix[i][j] = malloc(rooms_limits[i][j] * sizeof(room));
    }

    // Fills the 3-dim array using the counters
    for (uint8_t i = 0; i < room_size; i++) {
        uint16_t index = counters[rooms[i].faculty][rooms[i].type]++;
        rooms_matrix[rooms[i].faculty][rooms[i].type][index] = rooms[i];
    }

    // Frees counters
    for (uint8_t i = 0; i < faculty_size; i++)
        free(counters[i]);
    free(counters);

    return rooms_matrix;
}

/**
 * Prints which timeslot have been attributed to each exams.
 *
 * @param exams An array of scheduled exams
 */
void print_summary_schedule(array_exams *exams) {
    printf("Summary\n");
    printf("=======\n");

    for (int i = 0; i < MAX_EXAM; i++) {
        printf("Exam %d : %d\n", i + 1, exams->data[i]->timeslot);
    }

    printf("\n");
}

/**
 * Prints which detailed information from each exams,
 * for each timeslot available.
 *
 * @param exams An array of scheduled exams
 */
void print_detailed_schedule(array_exams *exams) {
    printf("Detailed schedule\n");
    printf("=================\n");

    for (int i = 0; i < MAX_TIMESLOT; i++) {
        printf("Timeslot %d\n", i + 1);
        printf("------------\n\n");

        for (int j = 0; j < exams->size; j++) {
            if (exams->data[j]->timeslot == i) {
                printf("  Exam %d :\n", j + 1);
                printf("      -> Prof : %d\n", exams->data[j]->teacher_id);

                printf("      -> Timeslots available : (");

                for (int k = 0; k < MAX_TIMESLOT; k++) {
                    printf("%d ", exams->data[j]->availabilities[k]);
                }

                printf(")\n");

                printf("      -> Conflicts detected : (");

                for (int k = 0; k < exams->size; k++) {
                    printf("%d ", exams->data[j]->conflicts[k]);
                }

                printf(")\n");

                printf("      -> Students :\n");

                for (int k = 0; k < exams->data[j]->enrollment; k++) {
                    printf("            %d\n", exams->data[j]->students[k]);
                }

                printf("      -> Room : %u\n", exams->data[j]->room_id);
            }
        }
    }
}

/**
 * Main function, execute the heuristics on a simple example.
 *
 */
int main() {
    uint8_t faculty_size = 1;
    // Collect a sample of exams
    array_exams *exams = get_example();

    // Creates and preprocesses a set of rooms
    room *rooms = get_rooms();
    uint16_t **indices = get_room_indices(4, faculty_size, rooms);
    room ***rooms_matrix = get_rooms_matrix(4, faculty_size, rooms, indices);

    // Preprocessing to the coloring graph heuristics
    compute_conflicts(exams);

    // Main heuristic
    bool a = color_graph_backtrack(exams, rooms_matrix, indices, faculty_size, MAX_TIMESLOT);

    printf("%s\n", (a == true) ? "A schedule has been found!\n" :
           "No schedule has been found!\n");

    // Some outputs
    if (a == true) {
        print_summary_schedule(exams);

        print_detailed_schedule(exams);
    }

    return 0;
}
