#include "util.h"
#include "structs.h"
#include "ils_heuristics.h"
#include "test_util.h"
#include "preprocessing.h"
#include "CUnit/Basic.h"
#include <time.h>

static void test_local_fitness_without_conflict(void) {
    init_test_exam();
    exam *test_exams[] = {exam1};
    init_test_array_exams(1, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 0;
    float fitness = local_fitness(exams, 0);
    /* There is no conflict, expected : 0*/
    CU_ASSERT_DOUBLE_EQUAL(fitness, 0, 1E-5);
    clean_array_exams();
}

static void test_local_fitness_with_conflicts(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam5};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 1;
    exams->data[2]->timeslot = 2;
    exams->data[3]->timeslot = 3;
    /* 3 conflicts (E2, 3, 5), dist : 1 + 2 + 3, expected : 6/3 = 2 */
    float fitness = local_fitness(exams, 0);
    CU_ASSERT_DOUBLE_EQUAL(fitness, 2, 1E-5);
    /* 2 conflicts (E1, E5) dist : 1 + 2 , expected : 3/2 */
    fitness = local_fitness(exams, 1);
    CU_ASSERT_DOUBLE_EQUAL(fitness, 1.5, 1E-5);
    /* 1 conflict (E1) dist : 2, expected : 2 */
    fitness = local_fitness(exams, 2);
    CU_ASSERT_DOUBLE_EQUAL(fitness, 2, 1E-5);
    /* 2 conflict (E1, E2) dist : 3 + 2 , expected : 5/2 */
    fitness = local_fitness(exams, 3);
    CU_ASSERT_DOUBLE_EQUAL(fitness, 2.5, 1E-5);
    clean_array_exams();
}

static void test_fitness_without_conflict(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam4};
    init_test_array_exams(2, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 1;
    exam *worst = NULL;
    float exam_fitness;
    float global_fitness = fitness(exams, &worst, &exam_fitness, 0);
    /* No conflicts, global fitness = 0 */
    CU_ASSERT_DOUBLE_EQUAL(global_fitness, 0, 1E-5);
    /* There is no worst fitness exam */
    CU_ASSERT_PTR_NULL(worst);
    /* Fitness associated of worst exam is FLT_MAX */
    CU_ASSERT_DOUBLE_EQUAL(exam_fitness, FLT_MAX, 1E-5);
    clean_array_exams();
}

static void test_fitness_with_conflicts(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam5};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 1;
    exams->data[2]->timeslot = 2;
    exams->data[3]->timeslot = 3;
    exam *worst;
    float exam_fitness;
    float global_fitness = fitness(exams, &worst, &exam_fitness, 0);

    /* Expected : 2 + 1.5 + 2 + 2.5 */
    CU_ASSERT_DOUBLE_EQUAL(global_fitness, 8, 1E-5);
    /* t fitness : 1.5 (Exam 2) */
    CU_ASSERT_DOUBLE_EQUAL(exam_fitness, 1.5, 1E-5);
    CU_ASSERT_PTR_EQUAL(worst, exams->data[1]);
    clean_array_exams();
}

static void test_perturbation(void) {
    /* Setup */
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam5};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 1;
    exams->data[2]->timeslot = 2;
    exams->data[3]->timeslot = 3;

    init_test_matrix_rooms();
    if(!room_assign(exams, mrooms, FACULTY_SIZE, MAX_TIMESLOT))
        printf("Error during room assignation.\n");

    exam *worst;
    float exam_fitness;
    float global_fitness = fitness(exams, &worst, &exam_fitness, 0);
    /* Small change for the test, so that the perturbation must
       return a solution with a fitness of 9.5. */
    exams->data[1]->availabilities[MAX_TIMESLOT-1] = true;
    // Apply perturbation function
    perturbation(&exams, worst, MAX_TIMESLOT, &mrooms, FACULTY_SIZE, MAX_ROOM_TYPE);

    /* Checking time */
    // Expected : 3 + 2.5 + 2 + 2
    CU_ASSERT_DOUBLE_EQUAL(fitness_bis(exams), 9.5, 1E-5);
    CU_ASSERT_EQUAL(exams->data[1]->timeslot, 4);

    /* Clean Up */
    clean_array_exams();
}

static void test_check_conflict(void) {
    /* Setup */
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam8};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 1;
    exams->data[2]->timeslot = 1;
    exams->data[3]->timeslot = 4;

    /* Checking time */
    // Assuming conflicts
    CU_ASSERT_TRUE(check_conflict(exams, 0, 1));
    CU_ASSERT_TRUE(check_conflict(exams, 2, 0));
    // Assuming no conflict
    CU_ASSERT_FALSE(check_conflict(exams, 3, 0));
    CU_ASSERT_FALSE(check_conflict(exams, 3, 1));
    CU_ASSERT_FALSE(check_conflict(exams, 3, 2));

    /* Clean Up */
    clean_array_exams();
}

static void test_kempe_chains(void) {
    /* Setup */
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam5};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 1;
    exams->data[1]->timeslot = 2;
    exams->data[2]->timeslot = 2;
    exams->data[3]->timeslot = 3;
    // Init swap
    uint8_t swaps[]  = {NOT_SCHEDULED, NOT_SCHEDULED, NOT_SCHEDULED, NOT_SCHEDULED};
    uint8_t swaps2[] = {NOT_SCHEDULED, NOT_SCHEDULED, NOT_SCHEDULED, NOT_SCHEDULED};
    // Apply Kempe Chains - Swaps exam3 (exams->data[2]) to timeslot 1
    bool op_status  = kempe_chains(exams, 2, 1, swaps);
    bool op_status2 = kempe_chains(exams, 3, 1, swaps2);

    /* Checking time */
    // First Kempe Chain - Success
    CU_ASSERT_TRUE(op_status);
    CU_ASSERT_EQUAL(swaps[0], 2);
    CU_ASSERT_EQUAL(swaps[1], 1);
    CU_ASSERT_EQUAL(swaps[2], 1);
    CU_ASSERT_EQUAL(swaps[3], NOT_SCHEDULED);

    // Second Kempe Chain - Fail (Exam1 cannot go to timeslot 3)
    // Exam5 can not go to timeslot 1, but for this test.
    CU_ASSERT_FALSE(op_status2);
    CU_ASSERT_EQUAL(swaps2[0], NOT_SCHEDULED);
    CU_ASSERT_EQUAL(swaps2[1], NOT_SCHEDULED);
    CU_ASSERT_EQUAL(swaps2[2], NOT_SCHEDULED);
    CU_ASSERT_EQUAL(swaps2[3], 1);

    /* Clean Up */
    clean_array_exams();
}

static void test_swap_timeslots(void) {
    /* Setup */
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam5};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 1;
    exams->data[2]->timeslot = 2;
    exams->data[3]->timeslot = 2;
    // Init swap
    uint8_t swaps[] = {NOT_SCHEDULED, 2, 1, 1};
    // Apply swap_timeslots
    swap_timeslots(exams, swaps);

    /* Checking time */
    CU_ASSERT_EQUAL(exams->data[0]->timeslot, 0);
    CU_ASSERT_EQUAL(exams->data[1]->timeslot, 2);
    CU_ASSERT_EQUAL(exams->data[2]->timeslot, 1);
    CU_ASSERT_EQUAL(exams->data[3]->timeslot, 1);

    /* Cleaning up */
    clean_array_exams();
}

static void test_termination_condition(void) {
    /* Setup */
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam5};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    // Init other parameters
    time_t start = time(NULL);

    /* Checking time */
    // threshold == 0, time not exceeded, counter not exceeded
    CU_ASSERT_FALSE(termination_condition(exams, 10,  0, start, 60, 0, 10000));
    // threshold not exceeded, time not exceeded, counter not exceeded
    CU_ASSERT_FALSE(termination_condition(exams, 10, 20, start, 60, 0, 10000));
    // threshold exceeded, time not exceeded, counter not exceeded
    CU_ASSERT_TRUE(termination_condition(exams,  10, 10, start, 60, 0, 10000));
    // threshold not exceeded, time exceeded, counter not exceeded
    CU_ASSERT_TRUE(termination_condition(exams,  10,  0, start-120, 60, 0, 10000));
    // threshold not exceeded, time not exceeded, counter exceeded
    CU_ASSERT_TRUE(termination_condition(exams,  10,  0, start, 60, 10001, 10000));

    /* Cleaning up */
    clean_array_exams();
}

int ils_heuristics_test_suite(void) {
    CU_TestInfo tests[] = {
        {"local_fitness() without conflict", test_local_fitness_without_conflict},
        {"local_fitness() with conflicts", test_local_fitness_with_conflicts},
        {"fitness() without conflict", test_fitness_without_conflict},
        {"fitness() with conflicts", test_fitness_with_conflicts},
        {"perturbation()", test_perturbation},
        {"check_conflict()", test_check_conflict},
        {"kempe_chains()", test_kempe_chains},
        {"swap_timeslots()", test_swap_timeslots},
        {"termination_condition()", test_termination_condition},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"ils_heuristics.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}
