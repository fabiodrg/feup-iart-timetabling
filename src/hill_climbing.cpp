#include "hill_climbing.h"
#include <stdlib.h>
#include <time.h>

Timetable *get_random_initial_state(const Instance &inst)
{
    srand(time(NULL));
    Timetable *tt = new Timetable();

    for (const Event &ev : inst.events) {
        bool added = false;
        do {
            uint32_t room_index = rand() % inst.getNumRooms();
            const Room &r = inst.rooms.at(room_index);
            uint8_t day = rand() % 5, timeslot = rand() % 9;
            added = tt->timetable[day][timeslot].addScheduledEvent(r, ev);
        } while (!added);
    }

    return tt;
}