#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H
#include "model.h"
#include <queue>

typedef priority_queue<Timetable*, vector<Timetable *>, TimetablePtrCmp> priority_queue_timetable_ptr;

Timetable* get_random_initial_state(const Instance& inst);

Timetable* get_greedy_initial_state(const Instance& inst);

priority_queue_timetable_ptr get_neighbors(Timetable *tt, Instance &inst);
#endif