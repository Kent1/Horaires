/*
 * This file is part of Umons - horaires project
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "util.h"
#include "structs.h"
#include "conflict.h"


bool compute_conflict(exam const *exam1, exam const *exam2) {
    uint32_t i = 0, j = 0;

    while (i < exam1->enrollment && j < exam2->enrollment) {
        if (exam1->students[i] < exam2->students[j])
            i++;
        else if (exam2->students[j] < exam1->students[i])
            j++;
        else { /* exam1->students[i] == exam2->students[j] */
            return true;
        }
    }

    return false;
}


void compute_conflicts(array_exams const *exams) {
    for (uint16_t i = 0; i < exams->size; i++) {
        exam const *exam1 = exams->data[i];
        // Exam is not in conflict with itself
        exam1->conflicts[exam1->exam_id - 1] = false;

        for (uint16_t j = i + 1; j < exams->size; j++) {
            exam const *exam2 = exams->data[j];

            if (compute_conflict(exam1, exam2)) {
                // If the two exams are in conflict
                // i.e. there are at least one student in common
                exam1->conflicts[exam2->exam_id - 1] = true;
                exam2->conflicts[exam1->exam_id - 1] = true;
            } else {
                // Else, they are not in conflict
                exam1->conflicts[exam2->exam_id - 1] = false;
                exam2->conflicts[exam1->exam_id - 1] = false;
            }
        }
    }
}
