#include "structs.h"
#include "test_util.h"
#include "CUnit/Basic.h"

static void test_init_exam(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    exam *exam = init_exam(1, 2, 555000, // Exam ID + Faculty ID + Teacher ID
                           students, 3,// Students + Enrollement (n of students)
                           classroom, // Room type
                           availabilities, 5, // Availabilities + max timeslots
                           2, NULL, 0); // Max exams + deps + deps size
    CU_ASSERT_EQUAL(exam->exam_id, 1);
    CU_ASSERT_EQUAL(exam->faculty, 2);
    CU_ASSERT_EQUAL(exam->teacher_id, 555000);
    CU_ASSERT_EQUAL(exam->enrollment, 3);
    CU_ASSERT_EQUAL(exam->students[0], 100000);
    CU_ASSERT_EQUAL(exam->students[1], 100001);
    CU_ASSERT_EQUAL(exam->students[2], 100002);
    CU_ASSERT_EQUAL(exam->room_type, classroom);
    CU_ASSERT_EQUAL(exam->room_id, NOT_ASSIGNED);
    CU_ASSERT_EQUAL(exam->timeslot, NOT_SCHEDULED);
    CU_ASSERT_EQUAL(exam->deps_size, 0);
    CU_ASSERT_TRUE(exam->availabilities[0]);
    CU_ASSERT_TRUE(exam->availabilities[1]);
    CU_ASSERT_FALSE(exam->availabilities[2]);
    CU_ASSERT_TRUE(exam->availabilities[3]);
    CU_ASSERT_FALSE(exam->availabilities[4]);
    CU_ASSERT_FALSE(exam->conflicts[0]);
    CU_ASSERT_FALSE(exam->conflicts[1]);
    free_exam(exam);
}

static void test_init_exam_with_deps(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    uint16_t deps[] = {2, 3};
    exam *exam = init_exam(1, 2, 555000, students, 3, classroom, availabilities,
                           5, 2, deps, 2);
    CU_ASSERT_EQUAL(exam->exam_id, 1);
    CU_ASSERT_EQUAL(exam->faculty, 2);
    CU_ASSERT_EQUAL(exam->teacher_id, 555000);
    CU_ASSERT_EQUAL(exam->enrollment, 3);
    CU_ASSERT_EQUAL(exam->students[0], 100000);
    CU_ASSERT_EQUAL(exam->students[1], 100001);
    CU_ASSERT_EQUAL(exam->students[2], 100002);
    CU_ASSERT_EQUAL(exam->room_type, classroom);
    CU_ASSERT_EQUAL(exam->room_id, NOT_ASSIGNED);
    CU_ASSERT_EQUAL(exam->timeslot, NOT_SCHEDULED);
    CU_ASSERT_EQUAL(exam->deps_size, 2);
    CU_ASSERT_EQUAL(exam->deps[0], 2);
    CU_ASSERT_EQUAL(exam->deps[1], 3);
    CU_ASSERT_TRUE(exam->availabilities[0]);
    CU_ASSERT_TRUE(exam->availabilities[1]);
    CU_ASSERT_FALSE(exam->availabilities[2]);
    CU_ASSERT_TRUE(exam->availabilities[3]);
    CU_ASSERT_FALSE(exam->availabilities[4]);
    CU_ASSERT_FALSE(exam->conflicts[0]);
    CU_ASSERT_FALSE(exam->conflicts[1]);
    free_exam(exam);
}

static void test_clone_exam(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    uint16_t deps[] = {2, 3};
    exam *exam_ = init_exam(1, 2, 555000, // Exam ID + Faculty ID + Teacher ID
                           students, 3,// Students + Enrollement (n of students)
                           classroom, // Room type
                           availabilities, 5, // Availabilities + max timeslots
                           2, deps, 2); // Max exams + deps + deps size
    exam *clone = clone_exam(exam_, 2, 5);
    CU_ASSERT_EQUAL(clone->exam_id, exam_->exam_id);
    CU_ASSERT_EQUAL(clone->faculty, exam_->faculty);
    CU_ASSERT_EQUAL(clone->teacher_id, exam_->teacher_id);
    CU_ASSERT_EQUAL(clone->enrollment, exam_->enrollment);
    CU_ASSERT_EQUAL(clone->students[0], exam_->students[0]);
    CU_ASSERT_EQUAL(clone->students[1], exam_->students[1]);
    CU_ASSERT_EQUAL(clone->students[2], exam_->students[2]);
    CU_ASSERT_EQUAL(clone->room_type, exam_->room_type);
    CU_ASSERT_EQUAL(clone->room_id, exam_->room_id);
    CU_ASSERT_EQUAL(clone->timeslot, exam_->timeslot);
    CU_ASSERT_EQUAL(clone->deps_size, exam_->deps_size);
    CU_ASSERT_EQUAL(clone->availabilities[0], exam_->availabilities[0]);
    CU_ASSERT_EQUAL(clone->availabilities[1], exam_->availabilities[1]);
    CU_ASSERT_EQUAL(clone->availabilities[2], exam_->availabilities[2]);
    CU_ASSERT_EQUAL(clone->availabilities[3], exam_->availabilities[3]);
    CU_ASSERT_EQUAL(clone->availabilities[4], exam_->availabilities[4]);
    CU_ASSERT_EQUAL(clone->conflicts[0], exam_->conflicts[0]);
    CU_ASSERT_EQUAL(clone->conflicts[1], exam_->conflicts[1]);

    free_exam(exam_);
    free_exam(clone);
}

static void test_init_array_exams(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    exam *exam1 = init_exam(1, 2, 555000, students, 3, classroom,
                            availabilities, 5, 2, NULL, 0);
    exam *exam2 = init_exam(1, 2, 555000, students, 3, classroom,
                            availabilities, 5, 2, NULL, 0);
    exam *exams[] = {exam1, exam2};
    array_exams *array_exams = init_array_exams(2, exams);
    CU_ASSERT_EQUAL(array_exams->size, 2);
    CU_ASSERT_PTR_EQUAL(array_exams->data[0], exam1);
    CU_ASSERT_PTR_EQUAL(array_exams->data[1], exam2);
    free_exams(array_exams);
}

static void test_clone_array_exams(void) {
    uint32_t students[] = {100000, 100001, 100002};
    bool availabilities[] = {true, true, false, true, false};
    exam *exam1 = init_exam(1, 2, 555000, students, 3, classroom,
                            availabilities, 5, 2, NULL, 0);
    exam *exam2 = init_exam(1, 2, 555000, students, 3, classroom,
                            availabilities, 5, 2, NULL, 0);
    exam *exams[] = {exam1, exam2};
    array_exams *array_exams_ = init_array_exams(2, exams);
    array_exams *clone = clone_array_exams(array_exams_, 5);
    CU_ASSERT_EQUAL(clone->size, array_exams_->size);
    CU_ASSERT_PTR_NOT_EQUAL(clone->data[0], array_exams_->data[0]);
    CU_ASSERT_PTR_NOT_EQUAL(clone->data[1], array_exams_->data[1]);
    CU_ASSERT_EQUAL(clone->data[0]->exam_id, exam1->exam_id);
    CU_ASSERT_EQUAL(clone->data[0]->faculty, exam1->faculty);
    CU_ASSERT_EQUAL(clone->data[0]->teacher_id, exam1->teacher_id);
    CU_ASSERT_EQUAL(clone->data[0]->enrollment, exam1->enrollment);
    CU_ASSERT_EQUAL(clone->data[0]->students[0], exam1->students[0]);
    CU_ASSERT_EQUAL(clone->data[0]->students[1], exam1->students[1]);
    CU_ASSERT_EQUAL(clone->data[0]->students[2], exam1->students[2]);
    CU_ASSERT_EQUAL(clone->data[0]->room_type, exam1->room_type);
    CU_ASSERT_EQUAL(clone->data[0]->room_id, exam1->room_id);
    CU_ASSERT_EQUAL(clone->data[0]->timeslot, exam1->timeslot);
    CU_ASSERT_EQUAL(clone->data[0]->deps_size, exam1->deps_size);
    CU_ASSERT_EQUAL(clone->data[0]->availabilities[0], exam1->availabilities[0]);
    CU_ASSERT_EQUAL(clone->data[0]->availabilities[1], exam1->availabilities[1]);
    CU_ASSERT_EQUAL(clone->data[0]->availabilities[2], exam1->availabilities[2]);
    CU_ASSERT_EQUAL(clone->data[0]->availabilities[3], exam1->availabilities[3]);
    CU_ASSERT_EQUAL(clone->data[0]->availabilities[4], exam1->availabilities[4]);
    CU_ASSERT_EQUAL(clone->data[0]->conflicts[0], exam1->conflicts[0]);
    CU_ASSERT_EQUAL(clone->data[0]->conflicts[1], exam1->conflicts[1]);

    CU_ASSERT_EQUAL(clone->data[1]->exam_id, exam2->exam_id);
    CU_ASSERT_EQUAL(clone->data[1]->faculty, exam2->faculty);
    CU_ASSERT_EQUAL(clone->data[1]->teacher_id, exam2->teacher_id);
    CU_ASSERT_EQUAL(clone->data[1]->enrollment, exam2->enrollment);
    CU_ASSERT_EQUAL(clone->data[1]->students[0], exam2->students[0]);
    CU_ASSERT_EQUAL(clone->data[1]->students[1], exam2->students[1]);
    CU_ASSERT_EQUAL(clone->data[1]->students[2], exam2->students[2]);
    CU_ASSERT_EQUAL(clone->data[1]->room_type, exam2->room_type);
    CU_ASSERT_EQUAL(clone->data[1]->room_id, exam2->room_id);
    CU_ASSERT_EQUAL(clone->data[1]->timeslot, exam2->timeslot);
    CU_ASSERT_EQUAL(clone->data[1]->deps_size, exam2->deps_size);
    CU_ASSERT_EQUAL(clone->data[1]->availabilities[0], exam2->availabilities[0]);
    CU_ASSERT_EQUAL(clone->data[1]->availabilities[1], exam2->availabilities[1]);
    CU_ASSERT_EQUAL(clone->data[1]->availabilities[2], exam2->availabilities[2]);
    CU_ASSERT_EQUAL(clone->data[1]->availabilities[3], exam2->availabilities[3]);
    CU_ASSERT_EQUAL(clone->data[1]->availabilities[4], exam2->availabilities[4]);
    CU_ASSERT_EQUAL(clone->data[1]->conflicts[0], exam2->conflicts[0]);
    CU_ASSERT_EQUAL(clone->data[1]->conflicts[1], exam2->conflicts[1]);
    free_exams(array_exams_);
    free_exams(clone);
}

static void test_init_room(void) {
    room *room = init_room(1, classroom, 1000,// Room ID + Room type + capacity
                           1, 2); // Faculty +  max_timeslot
    CU_ASSERT_EQUAL(room->room_id, 1);
    CU_ASSERT_EQUAL(room->type, classroom);
    CU_ASSERT_EQUAL(room->capacity, 1000);
    CU_ASSERT_EQUAL(room->faculty, 1);
    CU_ASSERT_EQUAL(room->assignation[0], NOT_ASSIGNED);
    CU_ASSERT_EQUAL(room->assignation[1], NOT_ASSIGNED)
    free_room(room);
}

static void test_clone_room(void) {
    room *room_ = init_room(1, classroom, 1000, 1, 2);
    room *clone = clone_room(room_, 2);

    CU_ASSERT_EQUAL(clone->room_id, room_->room_id);
    CU_ASSERT_EQUAL(clone->type, room_->type);
    CU_ASSERT_EQUAL(clone->capacity, room_->capacity);
    CU_ASSERT_EQUAL(clone->faculty, room_->faculty);
    CU_ASSERT_EQUAL(clone->assignation[0], room_->assignation[0]);
    CU_ASSERT_EQUAL(clone->assignation[1], room_->assignation[1]);

    free_room(room_);
    free_room(clone);
}

static void test_init_array_rooms(void) {
    room *room1 = init_room(1, classroom, 1000, 0, 2);
    room *room2 = init_room(2, classroom, 1000, 0, 2);
    room *rooms[] = {room1, room2};
    array_rooms *array_rooms = init_array_rooms(2, rooms);
    CU_ASSERT_EQUAL(array_rooms->size, 2);
    CU_ASSERT_PTR_EQUAL(array_rooms->data[0], room1);
    CU_ASSERT_PTR_EQUAL(array_rooms->data[1], room2);
    free_rooms(array_rooms);
}

static void test_clone_array_rooms(void) {
    room *room1 = init_room(1, classroom, 1000, 0, 2);
    room *room2 = init_room(2, classroom, 1000, 0, 2);
    room *rooms[] = {room1, room2};
    array_rooms *array_rooms_ = init_array_rooms(2, rooms);
    array_rooms *clone = clone_array_rooms(array_rooms_, 2);
    CU_ASSERT_EQUAL(clone->size, 2);
    CU_ASSERT_PTR_NOT_EQUAL(clone->data[0], room1);
    CU_ASSERT_PTR_NOT_EQUAL(clone->data[1], room2);
    CU_ASSERT_EQUAL(clone->data[0]->room_id, room1->room_id);
    CU_ASSERT_EQUAL(clone->data[0]->type, room1->type);
    CU_ASSERT_EQUAL(clone->data[0]->capacity, room1->capacity);
    CU_ASSERT_EQUAL(clone->data[0]->faculty, room1->faculty);
    CU_ASSERT_EQUAL(clone->data[0]->assignation[0], room1->assignation[0]);
    CU_ASSERT_EQUAL(clone->data[0]->assignation[1], room1->assignation[1]);
    CU_ASSERT_EQUAL(clone->data[1]->room_id, room2->room_id);
    CU_ASSERT_EQUAL(clone->data[1]->type, room2->type);
    CU_ASSERT_EQUAL(clone->data[1]->capacity, room2->capacity);
    CU_ASSERT_EQUAL(clone->data[1]->faculty, room2->faculty);
    CU_ASSERT_EQUAL(clone->data[1]->assignation[0], room2->assignation[0]);
    CU_ASSERT_EQUAL(clone->data[1]->assignation[1], room2->assignation[1]);

    free_rooms(array_rooms_);
    free_rooms(clone);
}

static void test_get_rooms_sizes(void) {
    init_test_array_rooms();
    size_t **size = get_rooms_sizes(FACULTY_SIZE, rooms);
    CU_ASSERT_EQUAL(size[0][classroom], 0);
    CU_ASSERT_EQUAL(size[0][lab], 0);
    CU_ASSERT_EQUAL(size[0][computer_room], 1);
    CU_ASSERT_EQUAL(size[1][classroom], 3);
    CU_ASSERT_EQUAL(size[1][lab], 1);
    CU_ASSERT_EQUAL(size[1][computer_room], 0);
    clean_array_rooms();
}

static void test_get_rooms_matrix(void) {
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->size, rooms_limit);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[0][computer_room][0], room5);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][classroom][0], room1);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][classroom][1], room2);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][classroom][2], room3);
    CU_ASSERT_PTR_EQUAL(matrix_rooms->data[1][lab][0], room4);
    clean_test_room();
}

static void test_clone_matrix_rooms(void) {
    init_test_array_rooms();
    size_t **rooms_limit = get_rooms_sizes(FACULTY_SIZE, rooms);
    matrix_rooms *matrix_rooms_ = get_rooms_matrix(FACULTY_SIZE, rooms,
                                 rooms_limit);
    matrix_rooms *clone = clone_matrix_rooms(matrix_rooms_, MAX_TIMESLOT, FACULTY_SIZE, MAX_ROOM_TYPE);

    CU_ASSERT_PTR_NOT_EQUAL(clone, matrix_rooms_);
    CU_ASSERT_PTR_NOT_EQUAL(clone->size, matrix_rooms_->size);
    CU_ASSERT_EQUAL(clone->data[0][computer_room][0]->room_id, matrix_rooms_->data[0][computer_room][0]->room_id);
    CU_ASSERT_EQUAL(clone->data[0][computer_room][0]->type, matrix_rooms_->data[0][computer_room][0]->type);
    CU_ASSERT_EQUAL(clone->data[0][computer_room][0]->faculty, matrix_rooms_->data[0][computer_room][0]->faculty);
    CU_ASSERT_EQUAL(clone->data[0][computer_room][0]->capacity, matrix_rooms_->data[0][computer_room][0]->capacity);
    for(uint8_t i = 0; i < MAX_TIMESLOT; i++) {
        CU_ASSERT_EQUAL(clone->data[0][computer_room][0]->assignation[i], matrix_rooms_->data[0][computer_room][0]->assignation[i]);
    }
}

int structs_test_suite(void) {
    CU_TestInfo tests[] = {
        {"init_exam()", test_init_exam},
        {"init_exam() with deps", test_init_exam_with_deps},
        {"clone_exam()", test_clone_exam},
        {"init_array_exams()", test_init_array_exams},
        {"clone_array_exams()", test_clone_array_exams},
        {"init_room()", test_init_room},
        {"clone_room()", test_clone_room},
        {"init_array_rooms()", test_init_array_rooms},
        {"clone_array_rooms()", test_clone_array_rooms},
        {"get_rooms_sizes()", test_get_rooms_sizes},
        {"get_rooms_matrix()", test_get_rooms_matrix},
        {"clone_matrix_rooms()", test_clone_matrix_rooms},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"structs.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}
