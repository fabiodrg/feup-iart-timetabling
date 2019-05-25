#include "hill_climbing.h"
#include "model.h"
#include <climits>

Timetable* goGenetic(Instance* inst, uint32_t initial_pop_n, uint32_t max_generations);

vector<Timetable*> selection(std::map<int, Timetable*> pop, int rf, int rc);

Timetable* crossover(Instance* inst, Timetable* father, Timetable* mother);
