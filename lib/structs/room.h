/*
 * This file is part of UMons - horaires project
 *
 *  Created on: 09/12/2013
 *      Authors: Quentin Loos, Alexandre Devaux, Jérôme Dauge
 *
 */

#ifndef ROOM_H_
#define ROOM_H_

#include "../util.h"

#define MAX_ROOMS 4

typedef enum { /* Defines an enumeration type */
    amphitheater,
    class,
    lab,
    computer_room,
} room_type;

typedef struct {
    uint16_t  room_id;
    room_type type;         // Type of the room (Amphitheater, computer room)
    uint16_t  capacity;     // Number of seats
    uint8_t   faculty;      // Faculty which owns the room
    uint16_t  *assignation; // contains for each timeslot the assigned exam_id
} room;

#endif /*ROOM_H_*/
