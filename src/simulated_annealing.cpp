#include "simulated_annealing.h"
#include <cmath>
#include <limits>

using namespace std;

Timetable* _get_next_state(Timetable* tt, Instance& inst) {
	// generate a random days and slots
	uint8_t day_a, slot_a, day_b, slot_b;

	do {
		day_a = rand() % TIMETABLE_NUMBER_DAYS;
		slot_a = rand() % TIMETABLE_SLOTS_PER_DAY;
		day_b = rand() % TIMETABLE_NUMBER_DAYS;
		slot_b = rand() % TIMETABLE_SLOTS_PER_DAY;
	} while (day_a != day_b || slot_a != slot_b);

	// Get all allocated events from the timeslot (day_a, slot_a)
	vector<Event*> allocated_events = tt->timetable[day_a][slot_a].getAllocatedEvents();

	if (allocated_events.empty()) {
		cout << "ups" << endl;
		return nullptr;
	}

	// randomly pick the event meant to be moved around
	int index_a = rand() % allocated_events.size();
	Event* picked_event = allocated_events.at(index_a);
	Room* picked_event_room = tt->timetable[day_a][slot_a].getAllocatedEventRoom(picked_event);

	// randomly pick the new spot for the event
	map<Room*, Event*, RoomPtrCmp> scheduled_events = tt->timetable[day_b][slot_b].getScheduledEvents();
	map<Room*, Event*, RoomPtrCmp>::iterator scheduled_events_it = scheduled_events.begin();

	int index_b = rand() % scheduled_events.size();
	for (int i = 0; i < index_b; i++) {
		scheduled_events_it++;
	}

	// clone the current timetable
	Timetable* new_tt = new Timetable(*tt);

	// make the swap
	new_tt->timetable[day_a][slot_a].updateScheduledEvent(picked_event_room, scheduled_events_it->second);
	new_tt->timetable[day_b][slot_b].updateScheduledEvent(scheduled_events_it->first, picked_event);

	return new_tt;
}

double cooling_schedule_A(annealing_info info, uint current_iteration) {
	return info.initial_temperature * pow((1 / info.initial_temperature), (double)current_iteration / info.iterations);
}

Timetable* simulated_annealing(Instance& inst, Timetable* (*generate_initial_state)(Instance&), annealing_info info) {
	// generate initial solution
	Timetable* tt = generate_initial_state(inst);

	// calculate the score for this random solution
	int current_score = tt->calculateScore();
	int iteration = 0;
	double temperature = info.initial_temperature;
	for (int i = info.iterations; i > 0; i--) {
		for (int j = 0; j < 10; j++) {
			// generate possible next state
			Timetable* next_tt = _get_next_state(tt, inst);
			if (next_tt == nullptr) continue;
			int next_score = next_tt->calculateScore();
			int delta_score = next_score - current_score;

			// evaluate the new score and accept or discard the new state
			if (delta_score < 0) {
				// it is an enhancent, accept it
				cout << "Delta: " << delta_score;
				current_score = next_score;
				delete tt;
				tt = next_tt;
			} else {
				double probability = exp(-delta_score / temperature);
				double r = (double)rand() / RAND_MAX;
				cout << "Delta: " << delta_score << " | P: " << probability << " | E: " << r << endl;
				if (probability > r) {
					// this is a bad score, but given the probability, let's accept it
					//cout << "Bad score, but accept it" << endl;
					current_score = next_score;
					delete tt;
					tt = next_tt;
				} else {
					//cout << "Bad score, reject it" << endl;
					// drop the generated next state
					delete next_tt;
				}
			}

			cout << "New score : " << next_score << endl;
		}

		// decrease the temperature
		temperature = cooling_schedule_A(info, iteration);
		cout << temperature << endl;
	}
	return tt;
}