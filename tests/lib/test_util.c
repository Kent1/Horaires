#include "structs.h"

const uint8_t MAX_TIMESLOT = 5;
const uint16_t MAX_EXAM = 8;
exam *exam1, *exam2, *exam3, *exam4, *exam5, *exam6, *exam7, *exam8;
array_exams *exams;

void init_test_exam(void) {
    {
        // Exam 1 - Analyse
        uint32_t students[] = {100000, 100001, 100002};
        bool availabilities[] = {true, true, true, false, false};
        exam1 = init_exam(1, 1, 555000, students, 3, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 2 - Fonctionnement des ordinateurs
        uint32_t students[] = {100000, 100003};
        bool availabilities[] = {false, true, true, false, false};
        exam2 = init_exam(2, 1, 555001, students, 2, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 3 - Math 1
        uint32_t students[] = {100002, 100004};
        bool availabilities[] = {false, true, true, true, false};
        exam3 = init_exam(3, 1, 555002, students, 2, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 4 - Anglais
        uint32_t students[] = {100003, 100004, 100005, 100006};
        bool availabilities[] = {false, false, true, false, false};
        exam4 = init_exam(4, 1, 555003, students, 4, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 5 - Physique
        uint32_t students[] = {100000, 100001, 100003};
        bool availabilities[] = {false, false, true, true, false};
        exam5 = init_exam(5, 1, 555001, students, 3, lab, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 6 - Chimie
        uint32_t students[] = {100004};
        bool availabilities[] = {false, false, false, true, true};
        exam6 = init_exam(6, 1, 555001, students, 1, lab, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 7 - Algèbre
        uint32_t students[] = {100001};
        bool availabilities[] = {false, false, false, false, true};
        exam7 = init_exam(7, 1, 555005, students, 1, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }

    {
        // Exam 8 - Univers : S & R
        uint32_t students[] = {100006};
        bool availabilities[] = {true, true, false, false, true};
        exam8 = init_exam(8, 1, 555006, students, 1, classroom, availabilities,
                          MAX_TIMESLOT, MAX_EXAM, NULL, 0);
    }
}

void init_test_array_exams(uint16_t exams_size, exam **exams) {
    init_test_exam();
    init_array_exams(exams_size, exams);
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

void clean_array_exams(void) {
    clean_test_exam();
    free(exams);
}

