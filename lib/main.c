#include "heuristics.h"
#include "conflict.h"

#define MAX_TIMESLOT 6
#define MAX_EXAM 8

void main() {

	// exam1 - Analyse
	exam exam1;
	exam1.exam_id = 1;
	exam1.teacher_id = 555000;

	exam1.timeslot = 0;

	exam1.enrollment = 3;
	uint32_t students1[3] = {10000, 10001, 10002};
	exam1.students = students1;

	uint8_t availabilities1[MAX_TIMESLOT] = {1, 1, 1};
	exam1.availabilities = availabilities1;

	uint16_t conflicts1[MAX_EXAM] = {};
	exam1.conflicts = conflicts1;

	// exam2 - Fonctionnement des ordis
	exam exam2;
	exam2.exam_id = 2;
	exam2.teacher_id = 555001;

	exam2.timeslot = 0;

	exam2.enrollment = 2;
	uint32_t students2[2] = {10000, 10003};
	exam2.students = students2;

	uint8_t availabilities2[MAX_TIMESLOT] = {0, 1, 1};
	exam2.availabilities = availabilities2;

	uint16_t conflicts2[MAX_EXAM];
	exam2.conflicts = conflicts2;


	// exam3 - MATH1
	exam exam3;
	exam3.exam_id = 3;
	exam3.teacher_id = 555002;

	exam3.timeslot = 0;

	exam3.enrollment = 2;
	uint32_t students3[2] = {10002, 10004};
	exam3.students = students3;

	uint8_t availabilities3[MAX_TIMESLOT] = {0, 1, 1, 1};
	exam3.availabilities = availabilities3;

	uint16_t conflicts3[MAX_EXAM];
	exam3.conflicts = conflicts3;


	// exam4 - Anglais
	exam exam4;
	exam4.exam_id = 4;
	exam4.teacher_id = 555003;

	exam4.timeslot = 0;

	exam4.enrollment = 4;
	uint32_t students4[4] = {10003, 10004, 10005, 10006};
	exam4.students = students4;

	uint8_t availabilities4[MAX_TIMESLOT] = {0, 0, 1};
	exam4.availabilities = availabilities4;

	uint16_t conflicts4[MAX_EXAM];
	exam4.conflicts = conflicts4;


	// exam5 - Anglais
	exam exam5;
	exam5.exam_id = 5;
	exam5.teacher_id = 555001;

	exam5.timeslot = 0;

	exam5.enrollment = 3;
	uint32_t students5[3] = {10000, 10001, 10003};
	exam5.students = students5;

	uint8_t availabilities5[MAX_TIMESLOT] = {0, 0, 1, 1};
	exam5.availabilities = availabilities5;

	uint16_t conflicts5[MAX_EXAM];
	exam5.conflicts = conflicts5;


	// exam6 - chimir
	exam exam6;
	exam6.exam_id = 6;
	exam6.teacher_id = 555004;

	exam6.timeslot = 0;

	exam6.enrollment = 1;
	uint32_t students6[1] = {10004};
	exam6.students = students6;

	uint8_t availabilities6[MAX_TIMESLOT] = {0, 0, 0, 1, 1, 1};
	exam6.availabilities = availabilities6;

	uint16_t conflicts6[MAX_EXAM];
	exam6.conflicts = conflicts6;


	// exam7 - algèbre
	exam exam7;
	exam7.exam_id = 7;
	exam7.teacher_id = 555005;

	exam7.timeslot = 0;

	exam7.enrollment = 1;
	uint32_t students7[1] = {10001};
	exam7.students = students7;

	uint8_t availabilities7[MAX_TIMESLOT] = {0, 0, 0, 0, 1, 1};
	exam7.availabilities = availabilities7;

	uint16_t conflicts7[MAX_EXAM];
	exam7.conflicts = conflicts7;

	// exam8 - jesaispaslire
	exam exam8;
	exam8.exam_id = 8;
	exam8.teacher_id = 555006;

	exam8.timeslot = 0;

	exam8.enrollment = 1;
	uint32_t students8[1] = {10002};
	exam8.students = students8;

	uint8_t availabilities8[MAX_TIMESLOT] = {1, 1, 0, 0, 1, 1};
	exam8.availabilities = availabilities8;

	uint16_t conflicts8[MAX_EXAM];
	exam8.conflicts = conflicts8;


	exam exams[MAX_EXAM] = {exam1, exam2, exam3, exam4, exam5, exam6, exam7, exam8};

	// Preprocessing
	compute_conflicts(exams, MAX_EXAM);

	bool a = color_graph_backtrack(exams, MAX_EXAM, MAX_TIMESLOT);
	printf("%d\n", a);

	int i, j, k;
	for (i = 0; i < MAX_EXAM; i++) {
		printf("Exam %d : %d\n", i+1, exams[i].timeslot);
	}

	for (i = 0; i < MAX_TIMESLOT; i++) {
		printf("Timeslot %d\n", i+1);
		printf("------------\n\n");

		for (j = 0; j < MAX_EXAM; j++) {
			if(exams[j].timeslot == i+1) {
				printf("  Exam %d :\n", j+1);
				printf("      -> Prof : %d\n", exams[j].teacher_id);

				printf("      -> Timeslots available : (");
				for (k = 0; k < MAX_TIMESLOT; k++) {
					printf("%d ", exams[j].availabilities[k]);
				}
				printf(")\n");

				printf("      -> Conflicts detected : (");
				for (k = 0; k < MAX_EXAM; k++) {
					printf("%d ", exams[j].conflicts[k]);
				}
				printf(")\n");

				printf("      -> Students :\n");
				for (k = 0; k < exams[j].enrollment; k++) {
					printf("            %d\n", exams[j].students[k]);
				}
			}
		}
	}
}
