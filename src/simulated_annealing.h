#include "hill_climbing.h"
#include "model.h"

struct annealing_info {
	double initial_temperature;
	uint iterations;
	double (*cooling_schedule)(annealing_info, uint);
};

double cooling_schedule_A(annealing_info info, uint current_iteration);

double cooling_schedule_B(annealing_info info, uint current_iteration);

Timetable* simulated_annealing(Instance& inst, Timetable* (*generate_initial_state)(Instance&), annealing_info info);