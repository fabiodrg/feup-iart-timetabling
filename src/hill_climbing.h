#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H
#include "model.h"
#include <queue>

typedef priority_queue<Timetable*, vector<Timetable*>, TimetablePtrCmp> priority_queue_timetable_ptr;

Timetable* get_random_initial_state(Instance& inst);

Timetable* get_greedy_initial_state(Instance& inst);

priority_queue_timetable_ptr get_neighbors(Timetable* tt, Instance& inst);

Timetable* get_best_neighbor(Timetable* tt, Instance& inst);

Timetable* get_random_neighbor(Timetable* tt, Instance& inst);

/**
 * @brief Implements Steepest Ascent Hill Climbing
 * At each iteration, it picks an event randomly and generates all possible timetables by swapping the picked event with all other events
 * 
 * @param inst The parsed data
 * @param generate_initial_state The initial solution generator to be used. 
 * @see get_random_initial_state
 * @see get_greedy_initial_state
 * @return Timetable* The best solution found
 */
Timetable* steepest_ascent_hill_climbing(Instance& inst, Timetable* (*generate_initial_state)(Instance&));

/**
 * @brief Implements Stochastic Hill Climbing
 * 
 * @param inst The parsed data
 * @param generate_initial_state The initial solution generator to be used. 
 * @see get_random_initial_state
 * @see get_greedy_initial_state 
 * @return Timetable* The best solution found
 */
Timetable* stochastic_hill_climbing(Instance& inst, Timetable* (*generate_initial_state)(Instance&));
#endif