#include "util.h"
#include "structs.h"
#include "preprocessing.h"
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
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam4, exam5, exam6};
    init_test_array_exams(5, test_exams);
    compute_conflicts(exams);
    /* Exam 1 and 2 : conflict with students */
    CU_ASSERT_TRUE(exam1->conflicts[1]);
    CU_ASSERT_TRUE(exam2->conflicts[0]);
    /* Exam 2 and 6 : conflict with professor */
    CU_ASSERT_TRUE(exam2->conflicts[4]);
    CU_ASSERT_TRUE(exam6->conflicts[1]);
    /* Exam 2 and 5 : conflict with students and professor */
    CU_ASSERT_TRUE(exam2->conflicts[3]);
    CU_ASSERT_TRUE(exam5->conflicts[1]);
    /* Exam 1 and 4 : no conflict */
    CU_ASSERT_FALSE(exam1->conflicts[2]);
    CU_ASSERT_FALSE(exam4->conflicts[0]);
    clean_array_exams();
}

static void test_compute_deps(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4, exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    /* Exam 3 dep : Exam 8 */
    CU_ASSERT_EQUAL(exam3->deps[0], 80);
    /* Exam 7 dep : Exam 1 */
    CU_ASSERT_EQUAL(exam7->deps[0], 10);
    compute_deps(exams);
    CU_ASSERT_EQUAL(exam3->deps[0], 7);
    CU_ASSERT_EQUAL(exam7->deps[0], 0);
    clean_array_exams();
}


int preprocessing_test_suite(void) {
    CU_TestInfo tests[] = {
        {"compute_conflict()", test_compute_conflict},
        {"compute_conflicts()", test_compute_conflicts},
        {"compute_deps()", test_compute_deps},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"preprocessing.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}