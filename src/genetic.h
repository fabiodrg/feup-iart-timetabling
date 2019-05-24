#include "model.h"
#include "hill_climbing.h"
#include <climits>

Timetable* goGenetic(Instance* inst, uint32_t initial_pop_n, uint32_t max_generations);

vector<Timetable*> selection(Instance* inst, std::vector<Timetable*> pop);

