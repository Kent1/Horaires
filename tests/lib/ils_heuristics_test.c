#include "util.h"
#include "structs.h"
#include "ils_heuristics.h"
#include "test_util.h"
#include "preprocessing.h"
#include "CUnit/Basic.h"

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
    float global_fitness = fitness(exams, worst, &exam_fitness, 0);
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

int ils_heuristics_test_suite(void) {
    CU_TestInfo tests[] = {
        {"local_fitness() without conflict", test_local_fitness_without_conflict},
        {"local_fitness() with conflicts", test_local_fitness_with_conflicts},
        {"fitness() without conflict", test_fitness_without_conflict},
        {"fitness() with conflicts", test_fitness_with_conflicts},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"ils_heuristics.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}