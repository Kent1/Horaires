#include "util.h"
#include "structs.h"
#include "graph_heuristics.h"
#include "test_util.h"
#include "preprocessing.h"
#include "CUnit/Basic.h"

static void test_compute_inf_timeslot(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4,
                            exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    preprocess(exams);
    CU_ASSERT_EQUAL(compute_inf_timeslot(exam1, exams), 0);
    CU_ASSERT_EQUAL(compute_inf_timeslot(exam7, exams), 0);
    /* Schedule Exam 1 at timeslot 2 (index 1)*/
    exams->data[0]->timeslot = 1;
    CU_ASSERT_EQUAL(compute_inf_timeslot(exams->data[0], exams), 0);
    /* First timeslot expected for Exam 7 (depends on Exam 1) : 3 (index 2) */
    CU_ASSERT_EQUAL(compute_inf_timeslot(exams->data[6], exams), 2);
    clean_array_exams();
}

static void test_set_possible_timeslot(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4,
                            exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    preprocess(exams);
    /* Timeslots for Exam 1 : same as availabilities (0 exams scheduled) */
    bool *timeslots_exam1 = set_possible_timeslot(exams->data[0], exams,
                                                    MAX_TIMESLOT);
    for(int i = 0; i < MAX_TIMESLOT; i++)
        CU_ASSERT_EQUAL(timeslots_exam1[i], exams->data[0]->availabilities[i]);
    free(timeslots_exam1);
    /* Schedule Exam 1 at timeslot 2 (index 1)*/
    exams->data[0]->timeslot = 1;
    /* Timeslots for Exam 7 : conflict with Exam 1 */
    bool *timeslots_exam7 = set_possible_timeslot(exams->data[6], exams,
                                                    MAX_TIMESLOT);
    CU_ASSERT_EQUAL(timeslots_exam7[0], exams->data[6]->availabilities[0]);
    /* Conflict */
    CU_ASSERT_FALSE(timeslots_exam7[1]);
    CU_ASSERT_EQUAL(timeslots_exam7[2], exams->data[6]->availabilities[2]);
    CU_ASSERT_EQUAL(timeslots_exam7[3], exams->data[6]->availabilities[3]);
    CU_ASSERT_EQUAL(timeslots_exam7[4], exams->data[6]->availabilities[4]);
    free(timeslots_exam7);
    clean_array_exams();
}

static void test_get_exams_saturation_degree_init(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4,
                            exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    preprocess(exams);
    uint8_t *sat_degree = get_exams_saturation_degree(exams, MAX_TIMESLOT);
    /* Exam 1 : 5 timeslots, 2 unavailable timeslots, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[0], MAX_TIMESLOT - 2);
    /* Exam 2 : 5 timeslots, 3 unavailable timeslots, expected : 2 */
    CU_ASSERT_EQUAL(sat_degree[1], MAX_TIMESLOT - 3);
    /* Exam 3 : 5 timeslots, 2 unavailable timeslots,
     * deps : Exam 8 (not scheduled), expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[2], NO_SAT);
    /* Exam 4 : 5 timeslots, 4 unavailable timeslots, expected : 1 */
    CU_ASSERT_EQUAL(sat_degree[3], MAX_TIMESLOT - 4);
    /* Exam 5 : 5 timeslots, 3 unavailable timeslots, expected : 2 */
    CU_ASSERT_EQUAL(sat_degree[4], MAX_TIMESLOT - 3);
    /* Exam 6 : 5 timeslots, 3 unavailable timeslots, expected : 2*/
    CU_ASSERT_EQUAL(sat_degree[5], MAX_TIMESLOT - 3);
    /* Exam 7 : 5 timeslots, 0 unavailable timeslot,
     * deps : Exam 1 (not scheduled), expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[6], NO_SAT);
    /* Exam 8 : 5 timeslots, 2 unavailable timeslots, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[7], MAX_TIMESLOT - 2);
    free(sat_degree);
    clean_array_exams();
}

static void test_get_exams_saturation_degree_scheduled(void) {

    /* Case 2 : Exam 1 is already scheduled */
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4,
                            exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    preprocess(exams);
    exams->data[0]->timeslot = 1;
    uint8_t *sat_degree = get_exams_saturation_degree(exams, MAX_TIMESLOT);
    /* Exam 1 : scheduled, expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[0], NO_SAT);
    /* Exam 2 : 5 timeslots, 3 unavailable timeslots,
     * conflit with Exam 1 (scheduled), expected : 1 */
    CU_ASSERT_EQUAL(sat_degree[1], MAX_TIMESLOT - 4);
    /* Exam 3 : 5 timeslots, 2 unavailable timeslots,
     * deps : Exam 8 (not scheduled), expected : NO_SAT */
    CU_ASSERT_EQUAL(sat_degree[2], NO_SAT);
    /* Exam 4 : 5 timeslots, 4 unavailable timeslots, expected : 1 */
    CU_ASSERT_EQUAL(sat_degree[3], MAX_TIMESLOT - 4);
    /* Exam 5 : 5 timeslots, 3 unavailable timeslots, expected : 2 */
    CU_ASSERT_EQUAL(sat_degree[4], MAX_TIMESLOT - 3);
    /* Exam 6 : 5 timeslots, 3 unavailable timeslots, expected : 2*/
    CU_ASSERT_EQUAL(sat_degree[5], MAX_TIMESLOT - 3);
    /* Exam 7 : 5 timeslots, 0 unavailable timeslot,
     * deps : Exam 1 (scheduled), min timeslot : 2, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[6], MAX_TIMESLOT - 2);
    /* Exam 8 : 5 timeslots, 2 unavailable timeslots, expected : 3 */
    CU_ASSERT_EQUAL(sat_degree[7], MAX_TIMESLOT - 2);
    free(sat_degree);
    clean_array_exams();
}

static void test_get_first_exam(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4,
                            exam5, exam6, exam7, exam8};
    init_test_array_exams(8, test_exams);
    preprocess(exams);
    exam *first = get_first_exam(exams, MAX_TIMESLOT);
    /* Initially, lower sat degree is Exam 4 */
    CU_ASSERT_PTR_EQUAL(first, exams->data[3]);
    /* Assign Exam 4 */
    exams->data[3]->timeslot = 2;
    first = get_first_exam(exams, MAX_TIMESLOT);
    /* Tie break between Exam 2, 5 and 6.
     * Exam 5 has the most students enrolled */
    CU_ASSERT_PTR_EQUAL(first, exams->data[4]);
    clean_array_exams();
}

static void test_color_graph_backtrack(void) {
    init_test_exam_2();
    exam *test_exams[] = {exam1, exam2, exam3};
    init_test_array_exams(3, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    uint8_t max_room_type = 3;
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, max_room_type, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, max_room_type, rooms,
                                 rooms_limit);
    bool result = color_graph_backtrack(exams, matrix_rooms, FACULTY_SIZE, max_room_type,
                                        MAX_TIMESLOT);
    CU_ASSERT_TRUE(result);
    /* Expected : E1 : 1, E2: 3, E3 : 2 */
    CU_ASSERT_EQUAL(exams->data[0]->timeslot, 0);
    CU_ASSERT_EQUAL(exams->data[1]->timeslot, 2);
    CU_ASSERT_EQUAL(exams->data[2]->timeslot, 1);
    clean_test_room();
    clean_array_exams_2();
}

int graph_heuristics_test_suite(void) {
    CU_TestInfo tests[] = {
        {"compute_inf_timeslot()", test_compute_inf_timeslot},
        {"set_possible_timeslot()", test_set_possible_timeslot},
        {"get_exams_saturation_degree() (first iteration)",
            test_get_exams_saturation_degree_init},
        {"get_exams_saturation_degree() (Exam 1 scheduled)",
            test_get_exams_saturation_degree_scheduled},
        {"get_first_exam()", test_get_first_exam},
        {"color_graph_backtrack()", test_color_graph_backtrack},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"graph_heuristics.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}
