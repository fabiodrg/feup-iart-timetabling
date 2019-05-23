#include "hill_climbing.h"
#include "model.h"
#include <queue>
#include <stdlib.h>
#include <time.h>

Timetable* get_random_initial_state(const Instance& inst) {
	Timetable* tt = new Timetable();

	for (const Event& ev : inst.events) {
		bool added = false;
		do {
			uint32_t room_index = rand() % inst.getNumRooms();
			const Room& r = inst.rooms.at(room_index);
			uint8_t day = rand() % 5, timeslot = rand() % 9;
			added = tt->timetable[day][timeslot].addScheduledEvent(r, ev);
		} while (!added);
	}

	return tt;
}

/**
 * @brief Loops through all timetable slots and attempts to scheduled the pair
 * (event, room)
 *
 * @param tt The timetable being generated
 * @param ev The event to be scheduled
 * @param r The room that satisfies the event requirements
 * @return Returns true if the event was scheduled successfully. False otherwise
 */
bool strict_schedule_event(Timetable* tt, const Event& ev, const Room& r) {
	bool event_added = false;
	for (size_t i = 0; i < TIMETABLE_NUMBER_DAYS && !event_added; i++) {
		for (size_t j = 0; j < TIMETABLE_SLOTS_PER_DAY && !event_added;
		     j++) {
			if (tt->timetable[i][j].addScheduledEvent(r, ev)) {
				event_added = true;
				break;
			}
		}
	}

	return event_added;
}

Timetable* get_greedy_initial_state(const Instance& inst) {
	Timetable* tt = new Timetable(inst);

	// go through all events
	for (const Event& ev : inst.events) {
		bool is_event_scheduled = false;

		// go through all existing rooms
		for (const Room& r : inst.rooms) {

			// check if room has the required capacity
			if (r.getSize() < ev.getNumberOfAtendees())
				continue;

			// check if room has all required features
			bool room_has_features = true;
			for (Feature f : ev.getRequiredFeatures()) {
				if (!r.hasFeature(f)) {
					room_has_features = false;
					break;
				}
			}

			if (room_has_features) {
				// Found the room to host the event
				// Try to pick a timetable spot randomly for the
				// event
				int attempts = 0;
				bool is_added = false;
				const int maximum_attempts = 10;
				do {
					int day =
						rand() % TIMETABLE_NUMBER_DAYS,
					    timeslot = rand() %
						       TIMETABLE_SLOTS_PER_DAY;
					is_added =
					    tt->timetable[day][timeslot]
						.addScheduledEvent(r, ev);
					attempts++;
				} while (!is_added &&
					 attempts < maximum_attempts);

				/**
				 * If failed to randomly add the scheduled
				 * event, try brute force
				 */
				if (!is_added) {
					if (!strict_schedule_event(tt, ev, r))
						continue; // failed, try next
							  // room
					else
						is_event_scheduled = true; // success
				} else {
					is_event_scheduled = true; // success
				}
			}
		}

		if (!is_event_scheduled) {
			cout << "Failed to schedule event: \n"
			     << ev << endl;
		}
	}

	return tt;
}


priority_queue_timetable_ptr get_neighbors(Timetable* tt, Instance& inst) {

	uint8_t day = rand() % TIMETABLE_NUMBER_DAYS, timeslot = rand() % TIMETABLE_SLOTS_PER_DAY;
	map<Room, Event*> scheduled_events = tt->timetable[day][timeslot].getScheduledEvents();
	map<Room, Event*>::iterator scheduled_events_it = scheduled_events.begin();

	while ((*scheduled_events_it).second == nullptr && scheduled_events_it != scheduled_events.end()) {
		scheduled_events_it++;
	}

	TimetablePtrCmp cmp;
	priority_queue_timetable_ptr neighbors(cmp);

	if (scheduled_events_it == scheduled_events.end()) {
		cout << "ERROR";
		return neighbors;
	}

	int current_score = tt->calculateScore(inst);

	for (int i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
		if (i == day) continue;

		for (int j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
			if (j == timeslot) continue;

			// clone the original table
			Timetable* new_tt = new Timetable(*tt);
			// swap the two events
			map<Room, Event*>::iterator idk = new_tt->timetable[i][j].getScheduledEvents().begin();
			tt->timetable[day][timeslot].updateScheduledEvent(scheduled_events_it->first, scheduled_events_it->second);

			new_tt->timetable[i][j].updateScheduledEvent(idk->first, scheduled_events_it->second);

			// calculate the score of the new timetable
			int new_score = new_tt->calculateScore(inst);
			cout << new_tt->myScore << endl;
			if (new_score < current_score) {
				neighbors.push(new_tt);
			}
		}
	}

	return neighbors;
}