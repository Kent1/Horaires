
extern exam *exam1, *exam2, *exam3, *exam4, *exam5, *exam6, *exam7, *exam8;
extern array_exams *exams;
extern const uint8_t MAX_TIMESLOT;
extern const uint16_t MAX_EXAM;

void init_test_exam(void);
void init_test_array_exams(uint16_t exams_size, exam **exams);
void clean_test_exam(void);
void clean_array_exams(void);