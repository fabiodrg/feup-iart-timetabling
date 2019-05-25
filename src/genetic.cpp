#include "genetic.h"
#include "hill_climbing.h"
#include <unordered_map>

Timetable* goGenetic(Instance* inst, uint32_t initial_pop_n, uint32_t max_generations) {

	map<int, Timetable*> population;
	for (uint32_t i = 0; i < initial_pop_n; i++) {
		Timetable* t = get_greedy_initial_state(*inst);
		population[t->calculateScore()] = t;
	}

	for (uint32_t gen = 0; gen < max_generations; gen++) {
	}
	vector<Timetable*> s = selection(inst, population, 10, 5);

	Timetable* child;
	for (unsigned short i = 0; i < max_generations; i++) {
		try {
			child = crossover(inst, s[0], s[1]);
			int cScore = child->calculateScore();
			cout << i << "th generation:" << endl;
			cout << "Father events: " << s[0]->getNumberOfEvents() << "	score: " << s[0]->calculateScore() << endl;
			cout << "Mother events: " << s[1]->getNumberOfEvents() << "	score: " << s[1]->calculateScore() << endl;
			cout << "Child events: " << child->getNumberOfEvents() << "	score: " << cScore << endl;
			population[cScore] = child;
		} catch (exception e) {
			cout << e.what();
		}
		s = selection(inst, population, 0, 20);
	}

	return selection(inst, population, 0, 1).at(0);
}

/**
 * 
 * 
 */
Timetable* crossover(Instance* inst, Timetable* father, Timetable* mother) {
	// cout << "Father events: "<< father->getNumberOfEvents() << endl;
	// cout << "Mother events: "<< mother->getNumberOfEvents() << endl;

	Timetable* child = new Timetable(*inst);

	//return child;
	uint8_t crossPoint = rand() % (TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY);
	// loop to add father to child
	for (uint8_t point = 0; point < crossPoint; point++) {
		//child.timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS] = father->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS];
	}

	vector<Event*> repeatedEvents;
	// Loop Mother Time Slots
	for (uint8_t point = crossPoint; point < TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY; point++) {
		TimeSlot timeSlotToAdd = mother->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS];
		// Loop Mother Events
		for (pair<Room*, Event*> p : timeSlotToAdd.getScheduledEvents()) {
			bool eventFound = false;
			// Loop Child Time Slots
			for (uint8_t point2 = 0; point2 < crossPoint; point2++) {
				//child.timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_NUMBER_DAYS];
				// Loop Child Events
				for (pair<Room*, Event*> p2 : child->timetable[point2 / TIMETABLE_SLOTS_PER_DAY][point2 % TIMETABLE_NUMBER_DAYS].getScheduledEvents()) {
					if (p2.second == p.second) {
						eventFound = true;
					}
				}
			}
			if (eventFound == false) {
				child->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS].addScheduledEvent(p.first, p.second);
			} else {
				repeatedEvents.push_back(p.second);
			}
		}
	}

	// find remaining events
	set<Event*> remainingEvents;
	for (uint8_t point = 0; point < crossPoint; point++) {
		for (pair<Room*, Event*> p : father->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS].getScheduledEvents()) {
			remainingEvents.insert(p.second);
		}
	}

	for (uint8_t point = crossPoint; point < TIMETABLE_NUMBER_DAYS * TIMETABLE_SLOTS_PER_DAY; point++) {
		for (pair<Room*, Event*> p : mother->timetable[point / TIMETABLE_SLOTS_PER_DAY][point % TIMETABLE_NUMBER_DAYS].getScheduledEvents()) {
			set<Event*>::iterator it = remainingEvents.find(p.second);
			if (it != remainingEvents.end()) {
				// event found in mother. remove.
				remainingEvents.erase(it);
			}
		}
	}

	// go through all remainig events and try to timetable them
	for (Event* ev : remainingEvents) {
		if (ev == NULL)
			continue;
		bool is_event_scheduled = false;

		// go through all existing rooms
		for (size_t i = 0; i < inst->rooms.size() && !is_event_scheduled; i++) {
			Room& r = inst->rooms.at(i);

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
				// Found a candidate room to host the event
				// Try to pick a timetable spot randomly for the
				// event
				int attempts = 0;
				const int maximum_attempts = 10;

				do {
					int day =
						rand() % TIMETABLE_NUMBER_DAYS,
					    timeslot = rand() %
						       TIMETABLE_SLOTS_PER_DAY;

					is_event_scheduled = child->timetable[day][timeslot].addScheduledEvent(&r, ev);

					attempts++;
				} while (!is_event_scheduled && attempts < maximum_attempts);
				if (attempts >= maximum_attempts)
					throw runtime_error("child is deffective :(");
			}
		}
	}
	return child;
}

vector<Timetable*> selection(Instance* inst, std::map<int, Timetable*> pop, int rf, int rc) {
	vector<Timetable*> fathers;

	int r = rand() % rc + rf;
	map<int, Timetable*>::iterator b = pop.begin();

	for (unsigned short i = 0; i < r; i++) {
		b++;
	}

	for (unsigned short i = 0; i < 2; i++) {
		//get two fathers
		fathers.push_back(b->second);
		b++;
	}
	return fathers;
}
