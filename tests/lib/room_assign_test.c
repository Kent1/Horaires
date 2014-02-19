#include "util.h"
#include "structs.h"
#include "room_assign.h"
#include "test_util.h"
#include "preprocessing.h"
#include "CUnit/Basic.h"

static void test_room_assign_single_exam(void) {
    init_test_exam();
    exam *test_exams[] = {exam4, exam1};
    init_test_array_exams(2, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    exams->data[0]->timeslot = 2;
    exams->data[1]->timeslot = 2;
    /* Exam 4, find a room : R3 */
    CU_ASSERT_TRUE(room_assign_single_exam(exams->data[0], matrix_rooms));
    CU_ASSERT_EQUAL(exams->data[0]->room_id, room3->room_id);
    CU_ASSERT_EQUAL(room3->assignation[2], exams->data[0]->exam_id);
    /* No room for Exam 1 : R3 already picked */
    CU_ASSERT_FALSE(room_assign_single_exam(exams->data[0], matrix_rooms));
    clean_test_room();
    clean_array_exams();
}

static void test_room_assign(void) {
    init_test_exam_2();
    exam *test_exams[] = {exam1, exam2, exam3};
    init_test_array_exams(3, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    room *test_rooms[] = {room1, room2, room3, room4, room5};
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* Manually scheduled : E1 : 1, E2 : 3, E3 : 2 */
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 2;
    exams->data[2]->timeslot = 1;
    CU_ASSERT_TRUE(room_assign(exams, matrix_rooms, 2, MAX_TIMESLOT));
    for(int i = 0 ; i < MAX_EXAM; i++) {
        /* Room assigned ? */
        uint16_t room_id = exams->data[i]->room_id;
        CU_ASSERT_NOT_EQUAL(room_id, NOT_ASSIGNED);
        for(int j = 0; j < MAX_ROOM; j++) {
            if(test_rooms[j]->room_id == room_id) {
                CU_ASSERT_EQUAL(test_rooms[j]->type, exams->data[i]->room_type);
            }
        }
    }
    clean_test_room();
    clean_array_exams_2();
}

static void test_reset_room_assigned(void) {
    /* Assign room (cf previous test) */
    init_test_exam_2();
    exam *test_exams[] = {exam1, exam2, exam3};
    init_test_array_exams(3, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    room *test_rooms[] = {room1, room2, room3, room4, room5};
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* Manually scheduled : E1 : 1, E2 : 3, E3 : 2 */
    exams->data[0]->timeslot = 0;
    exams->data[1]->timeslot = 2;
    exams->data[2]->timeslot = 1;
    CU_ASSERT_TRUE(room_assign(exams, matrix_rooms, 2, MAX_TIMESLOT));

    /* Now reset */
    reset_room_assigned(exams, matrix_rooms, FACULTY_SIZE, MAX_TIMESLOT);
    CU_ASSERT_EQUAL(exams->data[0]->room_id, NOT_ASSIGNED);
    CU_ASSERT_EQUAL(exams->data[1]->room_id, NOT_ASSIGNED);
    CU_ASSERT_EQUAL(exams->data[2]->room_id, NOT_ASSIGNED);
    for(uint8_t i = 0; i < MAX_ROOM; i++) {
        for(uint8_t j = 0; j < MAX_TIMESLOT; j++) {
            CU_ASSERT_EQUAL(test_rooms[i]->assignation[j], NOT_ASSIGNED);
        }
    }

    clean_test_room();
    clean_array_exams_2();
}

static void test_assign_by_timeslot_without_conflict(void) {
    /* assign_by_timeslot use room_assign_single_exam */
    init_test_exam();
    exam *test_exams[] = {exam4, exam5, exam3, exam7};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* Schedule all exam at T3 */
    for(uint8_t i = 0; i < 4; i++) {
        exams->data[i]->timeslot = 2;
    }
    /* No conflicts, expected : true */
    CU_ASSERT_TRUE(assign_by_timeslot(exams, matrix_rooms, 2));
    for(uint8_t i = 0; i < 4; i++) {
        CU_ASSERT_NOT_EQUAL(exams->data[i]->room_id, NOT_ASSIGNED);
    }
    clean_test_room();
    clean_array_exams();
}

static void test_assign_by_timeslot_with_conflicts(void) {
    init_test_exam();
    exam *test_exams[] = {exam4, exam1};
    init_test_array_exams(2, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* Schedule all exam at T3 */
    for(uint8_t i = 0; i < 2; i++) {
        exams->data[i]->timeslot = 2;
    }
    /* R3 is needed by the 2 exams, expected : false */
    CU_ASSERT_FALSE(assign_by_timeslot(exams, matrix_rooms, 2));
    clean_test_room();
    clean_array_exams();
}

static void test_reset_room_by_timeslot(void) {
    init_test_exam();
    exam *test_exams[] = {exam4, exam5, exam3, exam7};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    room *test_rooms[] = {room1, room2, room3, room4, room5};
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* Schedule all exam at T3 */
    for(uint8_t i = 0; i < 5; i++) {
        exams->data[i]->timeslot = 2;
    }
    /* No conflicts, expected : true */
    CU_ASSERT_TRUE(assign_by_timeslot(exams, matrix_rooms, 2));
    reset_room_by_timeslot(exams, matrix_rooms, 2);
    for(uint8_t i = 0 ; i < 4; i++) {
        CU_ASSERT_EQUAL(exams->data[i]->room_id, NOT_ASSIGNED);
    }
    /* Only exams assigned : we expect NOT_ASSIGNED everywhere */
    for(uint8_t i = 0; i < MAX_ROOM; i++) {
        for(uint8_t j = 0; j < MAX_TIMESLOT; j++) {
            CU_ASSERT_EQUAL(test_rooms[i]->assignation[j], NOT_ASSIGNED);
        }
    }
}

static void test_valid_assign_by_timeslot(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam4};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* Manually scheduled : E1 : T3, E2 : T2, E3 : T4, E4 : T3 */
    /* E1, T3, R3 */
    exams->data[0]->timeslot = 2;
    exams->data[0]->room_id = 3;
    /* E2, T2, R3 */
    exams->data[1]->timeslot = 1;
    exams->data[1]->room_id = 3;
    /* E3, T4, R3 */
    exams->data[2]->timeslot = 3;
    exams->data[2]->room_id = 3;
    /* E4, T3, R3 */
    exams->data[3]->timeslot = 2;
    exams->data[3]->room_id = 3;

    /* Expected : TRUE */
    CU_ASSERT_TRUE(valid_assign_by_timeslot(exams, matrix_rooms, 0));
    /* Expected : TRUE */
    CU_ASSERT_TRUE(valid_assign_by_timeslot(exams, matrix_rooms, 1));
    /* Expected : FALSE (conflict) */
    CU_ASSERT_FALSE(valid_assign_by_timeslot(exams, matrix_rooms, 2));
    /* Expected : TRUE */
    CU_ASSERT_TRUE(valid_assign_by_timeslot(exams, matrix_rooms, 3));
    /* Expected : TRUE */
    CU_ASSERT_TRUE(valid_assign_by_timeslot(exams, matrix_rooms, 4));
    clean_test_room();
    clean_array_exams();
}

static void test_is_valid_with_solution(void) {
    init_test_exam();
    exam *test_exams[] = {exam1, exam2, exam3, exam8};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* E1 and E3 : T3, R3 assigned; E2 et E8 : T2 */
    exams->data[0]->timeslot = 2;
    exams->data[0]->room_id = 3;

    exams->data[1]->timeslot = 1;
    exams->data[1]->room_id = 2;

    exams->data[2]->timeslot = 2;
    exams->data[2]->room_id = 3;

    exams->data[3]->timeslot = 1;
    exams->data[3]->room_id = 2;
    /* Switch rooms lead to a valid solution */
    CU_ASSERT_TRUE(is_valid(exams, matrix_rooms, 2, 1));
    /* E1 : expected R3 (only room with enough capacity) */
    CU_ASSERT_EQUAL(exams->data[0]->room_id, 3);
    /* E2 : expected R2 (best fit) */
    CU_ASSERT_EQUAL(exams->data[1]->room_id, 2);
    /* E3 : expected R2 (best fit) */
    CU_ASSERT_EQUAL(exams->data[2]->room_id, 2);
    /* E8 : expected R1 (best fit) */
    CU_ASSERT_EQUAL(exams->data[3]->room_id, 1);
    clean_test_room();
    clean_array_exams();
}

static void test_is_valid_without_solution(void) {
    init_test_exam();
    exam *test_exams[] = {exam4, exam1, exam2, exam8};
    init_test_array_exams(4, test_exams);
    preprocess(exams);
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    /* E1 and E4 : T3, R3 assigned; E2 et E8 : T2 */
    exams->data[0]->timeslot = 2;
    exams->data[0]->room_id = 3;

    exams->data[1]->timeslot = 2;
    exams->data[1]->room_id = 3;

    exams->data[2]->timeslot = 1;
    exams->data[2]->room_id = 2;

    exams->data[3]->timeslot = 1;
    exams->data[3]->room_id = 2;
    /* E1 and E4 can't be on the same timeslot */
    CU_ASSERT_FALSE(is_valid(exams, matrix_rooms, 2, 1));
    clean_test_room();
    clean_array_exams();
}

int room_assign_test_suite(void) {
    CU_TestInfo tests[] = {
        {"room_assign_single_exam()", test_room_assign_single_exam},
        {"room_assign()", test_room_assign},
        {"reset_room_assigned()", test_reset_room_assigned},
        {"assign_by_timeslot() (without conflict)",
            test_assign_by_timeslot_without_conflict},
        {"assign_by_timeslot() (with conflicts)",
            test_assign_by_timeslot_with_conflicts},
        {"reset_room_by_timeslot()", test_reset_room_by_timeslot},
        {"valid_assign_by_timeslot()",
            test_valid_assign_by_timeslot},
        {"is_valid() (with solution)", test_is_valid_with_solution},
        {"is_valid() (without solution)", test_is_valid_without_solution},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"room_assign.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}
