/**
 * @enum room_type
 *
 * enumeration type for the different room types.
 */
typedef enum {
    classroom,
    lab,
    computer_room,
} room_type;

/**
 * Pointer to examination 1 (initialized in init_test_exam() or
 * init_test_exam_2().
 */
extern exam *exam1,
/**
 * Pointer to examination 2 (initialized in init_test_exam() or
 * init_test_exam_2().
 */
*exam2,
/**
 * Pointer to examination 3 (initialized in init_test_exam() or
 * init_test_exam_2().
 */
*exam3,
/**
 * Pointer to examination 4 (initialized only in init_test_exam()).
 */
*exam4,
/**
 * Pointer to examination 5 (initialized only in init_test_exam()).
 */
*exam5,
/**
 * Pointer to examination 6 (initialized only in init_test_exam()).
 */
*exam6,
/**
 * Pointer to examination 7 (initialized only in init_test_exam()).
 */
*exam7,
/**
 * Pointer to examination 8 (initialized only in init_test_exam()).
 */
*exam8;
/**
 * Pointer to the array_exams struct (initialized in init_test_array_exams()).
 */
extern array_exams *exams;
/**
 * Pointer to room 1 (initialized only in init_test_room()).
 */
extern room *room1,
/**
 * Pointer to room 2 (initialized only in init_test_room()).
 */
*room2,
/**
 * Pointer to room 3 (initialized only in init_test_room()).
 */
*room3,
/**
 * Pointer to room 4 (initialized only in init_test_room()).
 */
*room4,
/**
 * Pointer to room 5 (initialized only in init_test_room()).
 */
*room5;
/**
 * Pointer to the struct array_rooms (initialized in init_test_array_rooms()).
 */
extern array_rooms *rooms;
/**
 * Pointer to the struct matrix_rooms (initialized in init_test_matrix_rooms()).
 */
extern matrix_rooms *mrooms;
/**
 * Defines the maximum timeslot available this session
 * (initialized in init_test_exam() or init_test_exam_2()).
 */
extern uint8_t  MAX_TIMESLOT;
/**
 * Defines the number of examinations to schedule
 * (initialized in init_test_exam() or init_test_exam_2()).
 */
extern uint16_t MAX_EXAM;
/**
 * Defines the maximum rooms available.
 * (initialized in init_test_exam() or init_test_exam_2()).
 */
extern uint16_t MAX_ROOM;
/**
 * Defines the maximum different faculty
 * (initialized in init_test_exam() or init_test_exam_2()).
 */
extern uint8_t  FACULTY_SIZE;


/**
 * Initializes a set of 8 examinations (variables exam1 to exam8) with :
 * <ul>
 * <li>MAX_TIMESLOT : 5</li>
 * <li>MAX_EXAM : 8</li>
 * <li>MAX_ROOM : 5</li>
 * <li>FACULTY_SIZE : 2</li>
 * </ul>
 *
 * The 8 examinations uses 6 students : 100000 to 100006
 * and 6 professors : 555000 to 555006.
 *
 * Examinations are defined as :
 *
 * Examination 1 (exam1) :
 * <ul>
 * <li>ID : 10</li>
 * <li>Faculty : 1</li>
 * <li>Students : 3 (100000, 100001, 100002)</li>
 * <li>Availabilities : T1, T2, T3</li>
 * <li>Professor : 555000</li>
 * <li>Room : Classroom</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Examination 2 (exam2) :
 * <ul>
 * <li>ID : 20</li>
 * <li>Faculty : 1</li>
 * <li>Students : 2 (100000, 100003)</li>
 * <li>Availabilities : T2, T3</li>
 * <li>Professor : 555001</li>
 * <li>Room : Classroom</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Examination 3 (exam3) :
 * <ul>
 * <li>ID : 30</li>
 * <li>Faculty : 1</li>
 * <li>Students : 2 (100002, 100004)</li>
 * <li>Availabilities : T2, T3, T4</li>
 * <li>Professor : 555002</li>
 * <li>Room : Classroom</li>
 * <li>Dependency : 80 (Examination 8)</li>
 * </ul>
 *
 * Examination 4 (exam4) :
 * <ul>
 * <li>ID : 40</li>
 * <li>Faculty : 1</li>
 * <li>Students : 4 (100003, 100004, 100005, 100006)</li>
 * <li>Availabilities : T3</li>
 * <li>Professor : 555003</li>
 * <li>Room : Classroom</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Examination 5 (exam5) :
 * <ul>
 * <li>ID : 50</li>
 * <li>Faculty : 1</li>
 * <li>Students : 3 (100000, 100001, 100003)</li>
 * <li>Availabilities : T3, T4</li>
 * <li>Professor : 555001</li>
 * <li>Room : Laboratory</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Examination 6 (exam6) :
 * <ul>
 * <li>ID : 60</li>
 * <li>Faculty : 1</li>
 * <li>Students : 1 (100001)</li>
 * <li>Availabilities : T1, T2, T3, T4, T5</li>
 * <li>Professor : 555001</li>
 * <li>Room : Laboratory</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Examination 7 (exam2) :
 * <ul>
 * <li>ID : 70</li>
 * <li>Faculty : 1</li>
 * <li>Students : 1 (100001)</li>
 * <li>Availabilities : T1, T2, T3, T4, T5</li>
 * <li>Professor : 555005</li>
 * <li>Room : Classroom</li>
 * <li>Dependancy : 10 (Examination 1)</li>
 * </ul>
 *
 * Examination 8 (exam8) :
 * <ul>
 * <li>ID : 20</li>
 * <li>Faculty : 1</li>
 * <li>Students : 1 (100006)</li>
 * <li>Availabilities : T1, T2, T5</li>
 * <li>Professor : 555001</li>
 * <li>Room : Classroom</li>
 * <li>No dependency</li>
 * </ul>
 *
 * The conflicts matrix is (S : students; P : professor, X : no conflict) :
 * <table>
 * <tr>
 * <th></th><th>1</th><th>2</th><th>3</th><th>4</th><th>5</th><th>6</th>
 * <th>7</th><th>8</th>
 * </tr>
 * <tr>
 * <th>1</th><td>/</td><td>S</td><td>S</td><td>X</td>
 * <td>S</td><td>X</td><td>S</td><td>S</td>
 * </tr>
 * <tr>
 * <th>2</th><td>S</td><td>/</td><td>X</td><td>S</td>
 * <td>P/S</td><td>P</td><td>X</td><td>X</td>
 * </tr>
 * <tr>
 * <th>3</th><td>S</td><td>X</td><td>/</td><td>S</td>
 * <td>X</td><td>S</td><td>X</td><td>S</td>
 * </tr>
 * <tr>
 * <th>4</th><td>X</td><td>S</td><td>S</td><td>/</td>
 * <td>S</td><td>S</td><td>X</td><td>X</td>
 * </tr>
 * <tr>
 * <th>5</th><td>S</td><td>P/S</td><td>X</td><td>S</td>
 * <td>/</td><td>P</td><td>S</td><td>X</td>
 * </tr>
 * <tr>
 * <th>6</th><td>X</td><td>P</td><td>S</td><td>S</td>
 * <td>P</td><td>/</td><td>X</td><td>X</td>
 * </tr>
 * <tr>
 * <th>7</th><td>S</td><td>X</td><td>X</td><td>X</td>
 * <td>S</td><td>X</td><td>/</td><td>X</td>
 * </tr>
 * <tr>
 * <th>8</th><td>S</td><td>X</td><td>S</td><td>X</td>
 * <td>X</td><td>X</td><td>X</td><td>/</td>
 * </tr>
 * </table>
 */
void init_test_exam(void);

/**
 * Initializes a set of 8 examinations (variables exam1 to exam8) with :
 * <ul>
 * <li>MAX_TIMESLOT : 3</li>
 * <li>MAX_EXAM : 3</li>
 * <li>MAX_ROOM : 5</li>
 * <li>FACULTY_SIZE : 2</li>
 * </ul>
 *
 * The 8 examinations uses 1 student : 100000
 * and 3 professors : 555000 to 555002.
 *
 * Examinations are defined as :
 *
 * Examination 1 (exam1) :
 * <ul>
 * <li>ID : 10</li>
 * <li>Faculty : 1</li>
 * <li>Students : 1 (100000)</li>
 * <li>Availabilities : T1, T3</li>
 * <li>Professor : 555000</li>
 * <li>Room : Classroom</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Examination 2 (exam1) :
 * <ul>
 * <li>ID : 20</li>
 * <li>Faculty : 1</li>
 * <li>Students : 1 (100000)</li>
 * <li>Availabilities : T2, T3</li>
 * <li>Professor : 555001</li>
 * <li>Room : Laboratory</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Examination 3 (exam3) :
 * <ul>
 * <li>ID : 30</li>
 * <li>Faculty : 0</li>
 * <li>Students : 1 (100000)</li>
 * <li>Availabilities : T1, T2</li>
 * <li>Professor : 555002</li>
 * <li>Room : Computer Room</li>
 * <li>No dependency</li>
 * </ul>
 *
 * Every examinations are in conflicts with every other examinations
 * (same student).
 */
void init_test_exam_2(void);

/**
 * This function intializes the pointer exams (struct array_exams) with an array
 * of examinations.
 * @param exams_size Number of examinations to use (size of the exams array).
 * @param exams      Array of pointer of each pointer of examination.
 */
void init_test_array_exams(uint16_t exams_size, exam **exams);

/**
 * This functions print informations about an array_exams.
 * The informations are :
 * <ul>
 * <li>Examination's ID</li>
 * <li>Teacher/Professor's ID</li>
 * <li>Enrollment (number of students registered to this examination)</li>
 * <li>Students ID registered to this examination</li>
 * <li>Room's ID assigned</li>
 * <li>Timeslot assigned</li>
 * </ul>
 * @param exams array_exams informations to print.
 */
void print_array_exams(array_exams *exams);

/**
 * This function cleans memory for the set of examination 1, i.e., it uses the
 * free() funcion on variables exam1 to exam8.
 */
void clean_test_exam(void);

/**
 * This function cleans memory for the set of examination 2, i.e., it uses the
 * free() funcion on variables exam1 to exam3.
 */
void clean_test_exam_2(void);

/**
 * This functions cleans memory for the struct array_exams (variable exams)
 * and the 8 examinations (variables exam1 to exam8).
 */
void clean_array_exams(void);

/**
 * This functions cleans memory for the struct array_exams (variable exams)
 * and the 3 examinations (variables exam1 to exam3).
 */
void clean_array_exams_2(void);

/**
 * This function initialize 5 tests rooms. The timeslot use the variable
 * MAX_TIMESLOT defined in init_test_exam() or init_test_exam_2().
 *
 * The rooms are defined as :
 *
 * Room 1 (room1) :
 * <ul>
 * <li>ID : 1</li>
 * <li>Type : Classroom</li>
 * <li>Capacity : 1</li>
 * <li>Faculty : 1</li>
 * </ul>
 *
 * Room 2 (room2) :
 * <ul>
 * <li>ID : 2</li>
 * <li>Type : Classroom</li>
 * <li>Capacity : 2</li>
 * <li>Faculty : 1</li>
 * </ul>
 *
 * Room 3 (room3) :
 * <ul>
 * <li>ID : 3</li>
 * <li>Type : Classroom</li>
 * <li>Capacity : 5</li>
 * <li>Faculty : 1</li>
 * </ul>
 *
 * Room 4 (room4) :
 * <ul>
 * <li>ID : 4</li>
 * <li>Type : Laboratory</li>
 * <li>Capacity : 4</li>
 * <li>Faculty : 1</li>
 * </ul>
 *
 * Room 5 (room5) :
 * <ul>
 * <li>ID : 5</li>
 * <li>Type : Computers room</li>
 * <li>Capacity : 10</li>
 * <li>Faculty : 0</li>
 * </ul>
 */
void init_test_room(void);

/**
 * This function initializes the pointer rooms (struct array_rooms) with the
 * 5 rooms defined in init_test_room() function.
 */
void init_test_array_rooms(void);

/**
 * This function initializes the pointer mrooms (struct matrix_rooms) using
 * the variable rooms (initialized in init_test_array_rooms() function).
 *
 * The variable FACULTY_SIZE used is defined in function init_test_exam() or
 * init_test_exam_2().
 */
void init_test_matrix_rooms(void);

/**
 * This function cleans memory : the 5 rooms pointer (room1 to room5).
 */
void clean_test_room(void);

/**
 * This function cleans the pointer rooms and the 5 rooms pointers
 * (room1 to room5). It uses the function free_rooms().
 */
void clean_array_rooms(void);
