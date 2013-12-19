
extern exam *exam1, *exam2, *exam3, *exam4, *exam5, *exam6, *exam7, *exam8;
extern array_exams *exams;
extern room *room1, *room2, *room3, *room4, *room5;
extern array_rooms *rooms;
extern const uint8_t  MAX_TIMESLOT;
extern const uint16_t MAX_EXAM;
extern const uint16_t MAX_ROOM;
extern const uint8_t  FACULTY_SIZE;

void init_test_exam(void);
void init_test_array_exams(uint16_t exams_size, exam **exams);
void clean_test_exam(void);
void clean_array_exams(void);
void init_test_room(void);
void init_test_array_rooms(void);
void clean_test_room(void);
void clean_array_rooms(void);