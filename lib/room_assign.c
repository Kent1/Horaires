/*
 * This file is part of UMons - horaires project
 *
 * These algorithms are use to assign to each scheduled exam a room.
 *
 *  Created on: 10/02/2014
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#include "util.h"
#include "structs.h"
#include "room_assign.h"


bool room_assign(array_exams *exams, matrix_rooms *rooms, uint8_t faculty_size,
                 uint8_t max_timeslot) {
    /* For each exam, having is own faculty and room_type, we'll select a room
       not assigned, corresponding with these parameters. If an exam has no
       room after the research, then the room assignation failed and another
       shedule has to be found => launch backtrack for the scheduling. */
    for (uint16_t i = 0; i < exams->size; i++) {
        exam *exam_ = exams->data[i];

        /* If true, the reseach has failed and all values are reset before
           launching the backtrack for the schedule. */
        if (!room_assign_single_exam(exam_, rooms)) {
            reset_room_assigned(exams, rooms, faculty_size, max_timeslot);
            return false;
        }
    }

    // Only if a room assignement has been found
    return true;
}

bool room_assign_single_exam(exam *exam_, matrix_rooms *rooms) {
    // Research a unassigned room for the exam exam_
    for (uint16_t j = 0; j < rooms->size[exam_->faculty][exam_->room_type]; j++) {

        room *room_ = rooms->data[exam_->faculty][exam_->room_type][j];

        /* If a room, corresponding in faculty & type, is unassigned and
           can contain the number of students, then the room is assigned.*/
        if (room_->assignation[exam_->timeslot] == NOT_ASSIGNED &&
                room_->capacity >= exam_->enrollment) {
            room_->assignation[exam_->timeslot] = exam_->exam_id;
            exam_->room_id = room_->room_id;
            return true;
        }
    }

    return false;
}

void reset_room_assigned(array_exams *exams, matrix_rooms *rooms,
                         uint8_t faculty_size, uint8_t max_timeslot) {
    for (uint16_t i = 0; i < exams->size; i++)
        exams->data[i]->room_id = NOT_ASSIGNED;

    for (uint8_t i = 0; i < faculty_size; i++)
        for (uint16_t j = 0; j < MAX_ROOM_TYPE; j++)
            for (uint16_t k = 0; k < rooms->size[i][j]; k++)
                for (uint16_t l = 0; l < max_timeslot; l++)
                    rooms->data[i][j][k]->assignation[l] = NOT_ASSIGNED;
}

void reset_room_by_timeslot(array_exams *exams, matrix_rooms *rooms,
                            uint8_t timeslot) {
    for (uint16_t i = 0; i < exams->size; i++) {
        if(exams->data[i]->timeslot == timeslot) {
            uint8_t faculty = exams->data[i]->faculty;
            room_type type = exams->data[i]->room_type;
            for(uint16_t j = 0; j < rooms->size[faculty][type]; j++) {
                if(rooms->data[faculty][type][j]->room_id
                        == exams->data[i]->room_id) {
                    rooms->data[faculty][type][j]->assignation[timeslot] =
                                                                NOT_ASSIGNED;
                    break;
                }
            }
            exams->data[i]->room_id = NOT_ASSIGNED;
        }
    }
}

bool assign_by_timeslot(array_exams *exams, matrix_rooms *rooms,
                        uint8_t timeslot) {
    for(uint16_t i = 0; i < exams->size; i++) {
        if(exams->data[i]->timeslot == timeslot) {
            if(!room_assign_single_exam(exams->data[i], rooms));
                return false;
        }
    }
    return true;
}

bool is_valid(array_exams *exams, matrix_rooms *rooms,
                                uint8_t timeslot1, uint8_t timeslot2) {
    /* Is there a conflict ? */
    bool valid_t1 = valid_assign_by_timeslot(exams, rooms, timeslot1);
    /* Room assigned is valid */
    if(!valid_t1) {
        /* Reset room assignement for this timeslot */
        reset_room_by_timeslot(exams, rooms, timeslot1);
        valid_t1 = assign_by_timeslot(exams, rooms, timeslot1);
    }
    bool valid_t2 = valid_assign_by_timeslot(exams, rooms, timeslot2);
    if(!valid_t2) {
        reset_room_by_timeslot(exams, rooms, timeslot2);
        valid_t2 = assign_by_timeslot(exams, rooms, timeslot1);
    }
    return (valid_t1 && valid_t2);
}


bool valid_assign_by_timeslot(array_exams *exams, matrix_rooms *rooms,
                                uint8_t timeslot) {
    uint16_t already_assigned[exams->size];
    for(uint16_t i = 0; i < exams->size; i++) {
        already_assigned[i] = NOT_ASSIGNED;
        if(exams->data[i]->timeslot == timeslot) {
            uint16_t k = 0;
            while(already_assigned[k] != NOT_ASSIGNED) {
                if(already_assigned[k] == exams->data[i]->room_id) {
                    /* Conflict ! */
                    return false;
                }
                k++;
            }
            already_assigned[k] = exams->data[i]->room_id;
        }
    }
    return true;
}
