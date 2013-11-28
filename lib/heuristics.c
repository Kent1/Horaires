/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 28/11/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "heuristics.h"

/**
 * This function returns a pointer to the next exam to color,
 * following the saturation degree first and the largest
 * enrollment in case of tie-break.
 *
 * @param exams An array of exams.
 * @return The next exam to color following our heuristics.
 */
exam* get_first_exam(exam* exams, ushort size) {
    // Initialization
    int i, best = -1;
    exam* first = NULL;

    // Compute all saturation degrees for the given exams
    puchar sat_degree = get_exams_saturation_degree(exams, size);

    // Find the exam with the max saturation degree and resolve
    // tie-break with largest enrollment
    for (i = 1; i < size; i++) {
        // Exam 'i' already colored
        if(sat_degree[i] == 255)
            continue;

        if(sat_degree[i] > best) {
            best  = sat_degree[i];
            first = &exams[i];
        } else if(sat_degree[i] == best &&
                    exams[i].enrollment > first->enrollment) {
            best  = sat_degree[i];
            first = &exams[i];
        }
    }

    return first;
}

/**
 * This function returns an array regrouping the saturation
 * degree for each given exams. The degree of already colored exams
 * is set to 255, considered as a special value to not consider.
 *
 * @param exams An array of exams.
 * @return An array regrouping saturation degree for the given parameter.
 */
puchar get_exams_saturation_degree(exam* exams, ushort size) {
    int i, j;
    puchar sat_degree = calloc(size, sizeof(uchar));

    for (i = 0; i < size; i++) {
        // already colored
        if(exams[i].timeslot != 0) {
            sat_degree[i] = 255;
        } else { // not colored, compute all conflicts
            sat_degree[i] = 0;

            for (j = 0; j < size; j++) {
                // A conflict represents an edge between i and j,
                // then if j has a timeslot => j is colored
                if(exams[i].conflicts[j] && exams[j].timeslot != 0)
                    sat_degree[i]++;
            }
        }
    }

    return sat_degree;
}