#include "util.h"
#include "structs.h"
#include "room_assign.h"
#include "test_util.h"
#include "CUnit/Basic.h"

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
    room_assign(exams, matrix_rooms, 2, MAX_TIMESLOT);
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

int room_assign_test_suite(void) {
    CU_TestInfo tests[] = {
        {"room_assign()", test_room_assign},
        CU_TEST_INFO_NULL
    };

    CU_SuiteInfo suites[] = {
        {"room_assign.c", NULL, NULL, tests},
        CU_SUITE_INFO_NULL
    };

    return CU_register_suites(suites);
}
