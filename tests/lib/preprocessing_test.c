#include "util.h"
#include "structs.h"
#include "conflict.h"
#include "test_util.h"
#include "CUnit/Basic.h"

static void test_compute_conflict(void) {
    init_test_exam();
    /* Conflict with itself */
    CU_ASSERT_TRUE(compute_conflict(exam1, exam1));
    /* Conflict with students */
    CU_ASSERT_TRUE(compute_conflict(exam1, exam2));
    /* Conflict with professor */
    CU_ASSERT_TRUE(compute_conflict(exam2, exam6));
    /* COnflict with professor and students */
    CU_ASSERT_TRUE(compute_conflict(exam2, exam5));
    clean_test_exam();
}

static void test_compute_conflicts(void) {
    init_test_exams(6, exam1, exam2, exam3, exam4, exam5, exam6);
    compute_conflicts(exams);
    /* Exam 1 and 2 : conflict with students */
    CU_ASSERT_TRUE(exam1->conflicts[exam2->exam_id - 1]);
    CU_ASSERT_TRUE(exam2->conflicts[exam1->exam_id - 1]);
    /* Exam 2 and 6 : conflict with professor */
    CU_ASSERT_TRUE(exam2->conflicts[exam6->exam_id - 1]);
    CU_ASSERT_TRUE(exam6->conflicts[exam2->exam_id - 1]);
    /* Exam 2 and 5 : conflict with students and professor */
    CU_ASSERT_TRUE(exam2->conflicts[exam5->exam_id - 1]);
    CU_ASSERT_TRUE(exam5->conflicts[exam2->exam_id - 1]);
    /* Exam 1 and 4 : no conflict */
    CU_ASSERT_FALSE(exam1->conflicts[exam4->exam_id - 1]);
    CU_ASSERT_FALSE(exam4->conflicts[exam1->exam_id - 1]);
    clean_test_exams();
}


int conflict_test_suite(void) {
    CU_TestInfo tests[] = {
        {"compute_conflict()", test_compute_conflict},
        {"compute_conflicts()", test_compute_conflicts},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"conflict.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}