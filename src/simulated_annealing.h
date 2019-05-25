#include "hill_climbing.h"
#include "model.h"

struct annealing_info {
	double initial_temperature;
	uint iterations;
};

Timetable* simulated_annealing(Instance& inst, Timetable* (*generate_initial_state)(Instance&), annealing_info info);