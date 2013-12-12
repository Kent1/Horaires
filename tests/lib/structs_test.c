#include "structs.h"
#include "CUnit/Basic.h"

static void test_init_exam(void) {
    exam *exam = init_exam(1, 555000,
                           3, 100000, 100001, 100002,
                           5, 1, 1, 1, 0, 0,
                           2, classroom);
    CU_ASSERT_EQUAL(exam->exam_id, 1);
    CU_ASSERT_EQUAL(exam->room_id, UINT16_MAX);
    CU_ASSERT_EQUAL(exam->timeslot, NOT_SCHEDULED);
    CU_ASSERT_EQUAL(exam->teacher_id, 555000);
    CU_ASSERT_EQUAL(exam->enrollment, 3);
    CU_ASSERT_EQUAL(exam->students[0], 100000);
    CU_ASSERT_EQUAL(exam->students[1], 100001);
    CU_ASSERT_EQUAL(exam->students[2], 100002);
    CU_ASSERT_EQUAL(exam->availabilities[0], 1);
    CU_ASSERT_EQUAL(exam->availabilities[1], 1);
    CU_ASSERT_EQUAL(exam->availabilities[2], 1);
    CU_ASSERT_EQUAL(exam->availabilities[3], 0);
    CU_ASSERT_EQUAL(exam->availabilities[4], 0);
    CU_ASSERT_EQUAL(exam->conflicts[0], 0);
    CU_ASSERT_EQUAL(exam->conflicts[1], 0);
    CU_ASSERT_EQUAL(exam->room_type, classroom);
    free_exam(exam);
}

static void test_init_exams(void) {
    exam *exam1 = init_exam(1, 555000,
                            3, 100000, 100001, 100002,
                            5, 1, 1, 1, 0, 0,
                            2, classroom);
    exam *exam2 = init_exam(2, 555000,
                            3, 100000, 100001, 100002,
                            5, 1, 1, 1, 0, 0,
                            2, classroom);
    array_exams *exams = init_exams(2, exam1, exam2);
    CU_ASSERT_EQUAL(2, exams->size);
    CU_ASSERT_PTR_EQUAL(exam1, exams->data[0]);
    CU_ASSERT_PTR_EQUAL(exam2, exams->data[1]);
    free_exams(exams);
}

static void test_init_room(void) {
    room *room = init_room(1, classroom, 1000, 0, 2);
    CU_ASSERT_EQUAL(room->room_id, 1);
    CU_ASSERT_EQUAL(room->type, classroom);
    CU_ASSERT_EQUAL(room->capacity, 1000);
    CU_ASSERT_EQUAL(room->faculty, 0);
    CU_ASSERT_EQUAL(room->assignation[0], NOT_ASSIGNED);
    CU_ASSERT_EQUAL(room->assignation[1], NOT_ASSIGNED)
    free_room(room);
}

static void test_init_rooms(void) {
    room *room1 = init_room(1, classroom, 1000, 0, 2);
    room *room2 = init_room(2, classroom, 1000, 0, 2);
    array_rooms *rooms = init_rooms(2, room1, room2);
    CU_ASSERT_EQUAL(2, rooms->size);
    CU_ASSERT_PTR_EQUAL(room1, rooms->data[0]);
    CU_ASSERT_PTR_EQUAL(room2, rooms->data[1]);
    free_rooms(rooms);
}

int structs_test_suite(void) {
    CU_TestInfo tests[] = {
        {"init_exam()", test_init_exam},
        {"init_exams()", test_init_exams},
        {"init_room()", test_init_room},
        {"init_rooms()", test_init_rooms},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"structs.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}