#ifndef HILL_CLIMBING_H
#define HILL_CLIMBING_H
#include "model.h"
#include <queue>

typedef priority_queue<Timetable*, vector<Timetable*>, TimetablePtrCmp> priority_queue_timetable_ptr;

/**
 * @brief Randomly generates an initial state for the problem instance
 * 
 * Method: 
 * - Go through all existing events in the problem instance
 * - Pick an existing room randomly. There's no check against any hard constraint
 * - Pick a random timeslot for scheduling the pair (room, event)
 * - If the room is already in use on the picked timeslot, then try again until it has success, repeat step 2
 * 
 * @param inst 
 * @return Timetable* 
 */
Timetable* get_random_initial_state(Instance& inst);

/**
 * @brief Generates an initial state for the problem instance using a more greedy approach
 * 
 * Method:
 * - Go through all existing events in the problem instance
 * - Go through all rooms available in this problem instance
 * - Check if the picked room has enough capacity for the event
 * - Check if the picked room has all required features for the event
 * - If one of the above constraints fail, pick another room
 * - Otherwise, randomly select a timeslot and try to schedule the pair (room, event)
 * - Obviously, for the picked timeslot the room might be busy. Therefore, select timeslots randomly up to 10 times
 * - Even if after 10 tries it fails, then the event is added to queue of failed scheduled events
 * - After all events being iterated, the queue of failed to be scheduled events is processed. Each event is scheduled to the first free room
 * @param inst 
 * @return Timetable* 
 */
Timetable* get_greedy_initial_state(Instance& inst);

/**
 * @brief Implements Steepest Ascent Hill Climbing
 * 
 * At each iteration, it picks an event randomly and generates all possible timetables by swapping the picked event with all other events
 * The timetable that offers best score, is picked as the current timetable
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
 * At each iteration, it picks an event randomly and generates all possible timetables by swapping the picked event with all other events
 * The new current timetable is randomly picked
 * 
 * @param inst The parsed data
 * @param generate_initial_state The initial solution generator to be used. 
 * @see get_random_initial_state
 * @see get_greedy_initial_state 
 * @return Timetable* The best solution found
 */
Timetable* stochastic_hill_climbing(Instance& inst, Timetable* (*generate_initial_state)(Instance&));

/**
 * @brief 
 * 
 * @param inst 
 * @param generate_initial_state 
 * @return Timetable* 
 */
Timetable* first_choice_hill_climbing(Instance& inst, Timetable* (*generate_initial_state)(Instance&));

#endif