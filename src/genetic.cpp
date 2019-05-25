#include "genetic.h"

Timetable* goGenetic(Instance* inst, uint32_t initial_pop_n, uint32_t max_generations) {

	vector<Timetable*> population;
	for (uint32_t i = 0; i < initial_pop_n; i++) {
		population.push_back(get_greedy_initial_state(*inst));
	}

	for (uint32_t gen = 0; gen < max_generations; gen++) {
	}
	return selection(inst, population).at(0);
}

Timetable* crossover(Instance* inst, Timetable* father, Timetable* mother) {
	Timetable child(*inst);

	uint8_t crossPoint = rand() % (TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY);
	// loop to add father to child
	for (uint8_t point = 0; point < crossPoint; point++) {
		child.timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS] = father->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS];
	}

	vector<Event*> remainderEvents;
	// Loop Mother Time Slots
	for (uint8_t point = crossPoint; point < TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY; point++) {
		TimeSlot timeSlotToAdd = mother->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS];
		// Loop Mother Events
		for (pair<Room*, Event*> p : timeSlotToAdd.getScheduledEvents()) {
			bool eventFound = false;
			// Loop Child Time Slots
			for (uint8_t point2 = 0; point2 < crossPoint; point2++) {
				child.timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_NUMBER_DAYS];
				// Loop Child Events
				for (pair<Room*, Event*> p2 : child.timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_NUMBER_DAYS].getScheduledEvents()) {
					if (p2.second == p.second) {
						eventFound = true;
					}
				}
			}
			if (eventFound == false) {
				child.timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS].addScheduledEvent(p.first, p.second);
			} else {
				remainderEvents.push_back(p.second);
			}
		}
		// go through all events
		for (Event* ev : remainderEvents) {
			bool is_event_scheduled = false;

			// go through all existing rooms
			for (Room& r : inst->rooms) {

				// check if room has the required capacity
				if (r.getSize() < ev->getNumberOfAtendees())
					continue;

				// check if room has all required features
				bool room_has_features = true;
				for (Feature f : ev->getRequiredFeatures()) {
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
						    child.timetable[day][timeslot]
							.addScheduledEvent(&r, ev);
						attempts++;
					} while (!is_added &&
						 attempts < maximum_attempts);

					/**
				 * If failed to randomly add the scheduled
				 * event, try brute force
				 */
					if (!is_added) {
						if (!strict_schedule_event(&child, ev, &r))
							continue; // failed, try next room
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
				child.unallocated_events.push_back(ev);
			}
		}
	}
}

vector<Timetable*> selection(Instance* inst, std::vector<Timetable*> pop) {
	vector<Timetable*> fathers;
	int minScore;
	int minScore1 = pop.at(0)->calculateScore(*inst);
	int minScore2 = pop.at(1)->calculateScore(*inst);
	if (minScore1 < minScore2) {
		fathers.push_back(pop.at(0));
		fathers.push_back(pop.at(1));
		minScore = minScore1;
	} else {
		fathers.push_back(pop.at(1));
		fathers.push_back(pop.at(0));
		minScore = minScore2;
	}
	for (Timetable* ind : pop) {
		int score = ind->calculateScore(*inst);
		if (minScore > score) {
			minScore = score;
			fathers.insert(fathers.begin() + 1, fathers.at(0));
			fathers.insert(fathers.begin(), ind);
		}
	}
	return fathers;
}
