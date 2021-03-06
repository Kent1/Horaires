#include <CUnit/Basic.h>

int structs_test_suite(void);
int preprocessing_test_suite(void);
int graph_heuristics_test_suite(void);
int room_assign_test_suite(void);
int ils_heuristics_test_suite(void);


int main(void) {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    int (* suites_functions[])(void) = {structs_test_suite,
                                        preprocessing_test_suite,
                                        graph_heuristics_test_suite,
                                        room_assign_test_suite,
                                        ils_heuristics_test_suite
                                        };

    int size = sizeof(suites_functions)/sizeof(suites_functions)[0];

    for (int i = 0; i < size; i++) {
        if (CUE_SUCCESS != suites_functions[i]()) {
            CU_cleanup_registry();
            return CU_get_error();
        }
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
