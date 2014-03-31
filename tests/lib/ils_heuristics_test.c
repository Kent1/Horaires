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
    /* 3 conflitcs (E2, E3, E5) dist :
     * E1 - E2 : 1 (SAME_DAY) = -20
     * E1 - E3 : 2
     * E1 - E5 : 3
     * Expected : (-20 + 2 + 3)/3 = -15/3 = -5 */
    float fitness = local_fitness(exams, 0);
    CU_ASSERT_DOUBLE_EQUAL(fitness, -5, 1E-5);
    /* 2 conflicts (E1, E5) dist :
     * E2 - E1 : 1 (SAME_DAY) = -20
     * E2 - E5 : 2
     * Expected : (-20+2)/2 = -9 */
    fitness = local_fitness(exams, 1);
    CU_ASSERT_DOUBLE_EQUAL(fitness, -9, 1E-5);
    /* 1 conflict (E1) dist :
     * E3 - E1 : 2
     * Expected : 2/1 = 2 */
    fitness = local_fitness(exams, 2);
    CU_ASSERT_DOUBLE_EQUAL(fitness, 2, 1E-5);
    /* 2 conflict (E1, E2) dist :
     * E3 - E1 : 3
     * E3 - E2 : 2
     * Expected : (2+3)/2 = 5/2 */
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
    float global_fitness = fitness(exams, &worst, &exam_fitness);
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
    float exam_fitness = -FLT_MAX;
    float global_fitness = fitness(exams, &worst, &exam_fitness);

    /* Expected : - 5 - 9 + 2 + 2.5 */
    CU_ASSERT_DOUBLE_EQUAL(global_fitness, -9.5, 1E-5);
    /* t fitness : 1.5 (Exam 2) */
    CU_ASSERT_DOUBLE_EQUAL(exam_fitness, -9, 1E-5);
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
    float exam_fitness = -FLT_MAX;
    fitness(exams, &worst, &exam_fitness);
    // Apply perturbation function
    perturbation(&exams, worst, MAX_TIMESLOT, &mrooms, FACULTY_SIZE,
                    MAX_ROOM_TYPE);

    /* Checking time */
    /* Worst fitness : E2 (-9), moving E2 to T2 */
    /* New E1 fitness :
     * E1 - E2 : 2
     * E1 - E3 : 2
     * E1 - E5 : 3
     * E1 fitness : (2+2+3)/3 = 7/3
     * --
     * New E2 fitness :
     * E2 - E1 : 2
     * E2 - E5 : 1 (SAME_DAY) = -20
     * E2 fitness : (2-20)/2 = -9
     * --
     * No change to E3 fitness : 2
     * --
     * New E5 fitness :
     * E5 - E1 : 3
     * E5 - E2 : 1 (SAME_DAY) = -20
     * E5 fitness : (3 - 20)/2 = -17/2
     * Global fitness expected : 7/3 - 9 + 2 - 17/2 = (14 - 54 + 12 - 51)/6
     *                                              = -79/6
     */
    CU_ASSERT_DOUBLE_EQUAL(fitness_bis(exams), -79.0/6, 1E-5);
    CU_ASSERT_EQUAL(exams->data[1]->timeslot, 2);

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
