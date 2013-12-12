#include "util.h"
#include "structs.h"
#include "heuristics.h"
#include "test_util.h"
#include "CUnit/Basic.h"

static void test_get_exams_saturation_degree(void) {
    /* 1 exam, 5 timeslot, 2 unavailable timeslot : expected : 3 */
    init_test_exams(1, exam1);
    uint8_t *sat_degree = get_exams_saturation_degree(exams, MAX_TIMESLOT);
    CU_ASSERT_EQUAL(sat_degree[0], MAX_TIMESLOT - 2);
    free(sat_degree);
    clean_test_exams();
}

int heuristics_test_suite(void) {
    CU_TestInfo tests[] = {
        {"get_exams_saturation_degree()", test_get_exams_saturation_degree},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"heuristics.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}