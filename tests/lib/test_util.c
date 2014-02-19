#include "structs.h"

uint8_t  MAX_TIMESLOT = 5;
uint16_t MAX_EXAM     = 8;
uint16_t MAX_ROOM     = 5;
uint8_t  FACULTY_SIZE = 2;

exam *exam1, *exam2, *exam3, *exam4, *exam5, *exam6, *exam7, *exam8;
array_exams *exams;
room *room1, *room2, *room3, *room4, *room5;
array_rooms *rooms;
matrix_rooms *mrooms;

void init_test_exam(void) {

    MAX_TIMESLOT = 5;
    MAX_EXAM     = 8;
    MAX_ROOM     = 5;
    FACULTY_SIZE = 2;

    {
        // Exam 1 - Analyse
        uint32_t students[] = {100000, 100001, 100002};
        bool availabilities[] = {true, true, true, false, false};
        exam1 = init_exam(10, 1, 555000, students, 3, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 2 - Fonctionnement des ordinateurs
        uint32_t students[] = {100000, 100003};
        bool availabilities[] = {false, true, true, false, false};
        exam2 = init_exam(20, 1, 555001, students, 2, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 3 - Math 1
        uint32_t students[] = {100002, 100004};
        bool availabilities[] = {false, true, true, true, false};
        uint16_t deps[] = {80};
        exam3 = init_exam(30, 1, 555002, students, 2, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, deps, 1);
    }

    {
        // Exam 4 - Anglais
        uint32_t students[] = {100003, 100004, 100005, 100006};
        bool availabilities[] = {false, false, true, false, false};
        exam4 = init_exam(40, 1, 555003, students, 4, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 5 - Physique
        uint32_t students[] = {100000, 100001, 100003};
        bool availabilities[] = {false, false, true, true, false};
        exam5 = init_exam(50, 1, 555001, students, 3, lab, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 6 - Chimie
        uint32_t students[] = {100004};
        bool availabilities[] = {false, false, false, true, true};
        exam6 = init_exam(60, 1, 555001, students, 1, lab, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 7 - Algèbre
        uint32_t students[] = {100001};
        bool availabilities[] = {true, true, true, true, true};
        uint16_t deps[] = {10};
        exam7 = init_exam(70, 1, 555005, students, 1, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, deps, 1);
    }

    {
        // Exam 8 - Univers : S & R
        uint32_t students[] = {100006};
        bool availabilities[] = {true, true, false, false, true};
        exam8 = init_exam(80, 1, 555006, students, 1, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }
}

void init_test_exam_2(void) {

    MAX_TIMESLOT = 3;
    MAX_EXAM     = 3;
    MAX_ROOM     = 5;
    FACULTY_SIZE = 2;

    {
        uint32_t students[] = {100000};
        bool availabilities[] = {true, false, true};
        exam1 = init_exam(10, 1, 555000, students, 3, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        uint32_t students[] = {100000};
        bool availabilities[] = {false, true, true};
        exam2 = init_exam(20, 1, 555001, students, 2, lab, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        uint32_t students[] = {100000};
        bool availabilities[] = {true, true, false};
        exam3 = init_exam(30, 0, 555002, students, 2, computer_room, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }
}

void init_test_array_exams(uint16_t exams_size, exam **test_exams) {
    exams = init_array_exams(exams_size, test_exams);
}

void print_array_exams(array_exams *exams) {
    printf("\n\n");
    for(uint8_t i = 0; i < exams->size; i++) {
        printf("Exam %d\n========\n", i);
        printf("Exam ID :\t%d\n", exams->data[i]->exam_id);
        printf("Teacher ID :\t%d\n", exams->data[i]->teacher_id);
        printf("Enrollment :\t%d\n", exams->data[i]->enrollment);

        printf("\tStudents : ");
        for(uint8_t j = 0; j < exams->data[i]->enrollment; j++)
            printf("%d, ", exams->data[i]->students[j]);
        printf("\n\n");

        printf("Room ID : \t%d\n", exams->data[i]->room_id);
        printf("Timeslot : \t%d\n\n", exams->data[i]->timeslot);

    }
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

void clean_test_exam_2(void) {
    free_exam(exam1);
    free_exam(exam2);
    free_exam(exam3);
}

void clean_array_exams(void) {
    clean_test_exam();
    free(exams);
}

void clean_array_exams_2(void) {
    clean_test_exam_2();
    free(exams);
}

void init_test_room(void) {
    // Room 1 - Salon Bleu
    room1 = init_room(1, classroom, 1, 1, MAX_TIMESLOT);
    // Room 2 - Plisnier
    room2 = init_room(2, classroom, 2, 1, MAX_TIMESLOT);
    // Room 3 - Van Gogh
    room3 = init_room(3, classroom, 5, 1, MAX_TIMESLOT);
    // Room 4 - Pascal
    room4 = init_room(4, lab, 4, 1, MAX_TIMESLOT);
    // Room 5 - IG Lab
    room5 = init_room(5, computer_room, 10, 0, MAX_TIMESLOT);
}

void init_test_array_rooms(void) {
    init_test_room();
    room *test_rooms[] = {room1, room2, room3, room4, room5};
    rooms = init_array_rooms(MAX_ROOM, test_rooms);
}

void init_test_matrix_rooms(void) {
    init_test_array_rooms();
    size_t **rooms_limits = get_rooms_sizes(FACULTY_SIZE, rooms);
    mrooms = get_rooms_matrix(FACULTY_SIZE, rooms, rooms_limits);
}

void clean_test_room(void) {
    free(room1);
    free(room2);
    free(room3);
    free(room4);
    free(room5);
}

void clean_array_rooms(void) {
    free_rooms(rooms);
}
