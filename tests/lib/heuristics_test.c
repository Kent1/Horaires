#include "util.h"
#include "structs.h"
#include "heuristics.h"
#include "test_util.h"
#include "preprocessing.h"
#include "CUnit/Basic.h"

static void test_get_exams_saturation_degree_init(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4, exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    preprocess(exams);
    uint8_t *sat_degree = get_exams_saturation_degree(exams, MAX_TIMESLOT);
    /* Exam 1 : 5 timeslots, 2 unavailable timeslots, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[0], MAX_TIMESLOT - 2);
    /* Exam 2 : 5 timeslots, 3 unavailable timeslots, expected : 2 */
    CU_ASSERT_EQUAL(sat_degree[1], MAX_TIMESLOT - 3);
    /* Exam 3 : 5 timeslots, 2 unavailable timeslots, deps : Exam 8 (not scheduled), expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[2], NO_SAT);
    /* Exam 4 : 5 timeslots, 4 unavailable timeslots, expected : 1 */
    CU_ASSERT_EQUAL(sat_degree[3], MAX_TIMESLOT - 4);
    /* Exam 5 : 5 timeslots, 3 unavailable timeslots, expected : 2 */
    CU_ASSERT_EQUAL(sat_degree[4], MAX_TIMESLOT - 3);
    /* Exam 6 : 5 timeslots, 3 unavailable timeslots, expected : 2*/
    CU_ASSERT_EQUAL(sat_degree[5], MAX_TIMESLOT - 3);
    /* Exam 7 : 5 timeslots, 0 unavailable timeslot, deps : Exam 1 (not scheduled), expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[6], NO_SAT);
    /* Exam 8 : 5 timeslots, 2 unavailable timeslots, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[7], MAX_TIMESLOT - 2);
    free(sat_degree);
    clean_array_exams();
}

static void test_get_exams_saturation_degree_scheduled(void) {

    /* Case 2 : Exam 1 is already scheduled */
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4, exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 1;
    uint8_t *sat_degree = get_exams_saturation_degree(exams, MAX_TIMESLOT);
    /* Exam 1 : scheduled, expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[0], NO_SAT);
    /* Exam 2 : 5 timeslots, 3 unavailable timeslots, conflit with Exam 1 (scheduled), expected : 1 */
    CU_ASSERT_EQUAL(sat_degree[1], MAX_TIMESLOT - 4);
    /* Exam 3 : 5 timeslots, 2 unavailable timeslots, deps : Exam 8 (not scheduled), expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[2], NO_SAT);
    /* Exam 4 : 5 timeslots, 4 unavailable timeslots, expected : 1 */
    CU_ASSERT_EQUAL(sat_degree[3], MAX_TIMESLOT - 4);
    /* Exam 5 : 5 timeslots, 3 unavailable timeslots, expected : 2 */
    CU_ASSERT_EQUAL(sat_degree[4], MAX_TIMESLOT - 3);
    /* Exam 6 : 5 timeslots, 3 unavailable timeslots, expected : 2*/
    CU_ASSERT_EQUAL(sat_degree[5], MAX_TIMESLOT - 3);
    /* Exam 7 : 5 timeslots, 0 unavailable timeslot, deps : Exam 1 (scheduled), min timeslot : 2, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[6], MAX_TIMESLOT - 2);
    /* Exam 8 : 5 timeslots, 2 unavailable timeslots, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[7], MAX_TIMESLOT - 2);
    free(sat_degree);
    clean_array_exams();
}

int heuristics_test_suite(void) {
    CU_TestInfo tests[] = {
        {"get_exams_saturation_degree() (first iteration)", test_get_exams_saturation_degree_init},
        {"get_exams_saturation_degree() (Exam 1 scheduled)", test_get_exams_saturation_degree_scheduled},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"heuristics.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}