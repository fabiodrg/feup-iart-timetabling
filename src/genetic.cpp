#include "genetic.h"

Timetable* goGenetic(Instance* inst, uint32_t initial_pop_n, uint32_t max_generations) {
   
  vector<Timetable*> population;
 
 for (uint32_t i = 0; i< initial_pop_n; i++) {
  population.push_back(get_random_initial_state(*inst));
 }

	return selection(inst, population).at(0);
}

bool crossover(Timetable t1, Timetable t2){
  
}

vector<Timetable*> selection(Instance* inst, std::vector<Timetable*> pop){
  vector<Timetable*> fathers;
  int minScore;
  int minScore1 = pop.at(0)->calculateScore(*inst);
  int minScore2 = pop.at(1)->calculateScore(*inst);
  if(minScore1 < minScore2){
    fathers.push_back(pop.at(0));
    fathers.push_back(pop.at(1));
    minScore = minScore1;
  } else{
    fathers.push_back(pop.at(1));
    fathers.push_back(pop.at(0));
    minScore = minScore2;
  }
  for (Timetable* ind : pop) {
    int score = ind->calculateScore(*inst);
    if(minScore > score){
      minScore = score;
      fathers.insert(fathers.begin()+1, fathers.at(0));
      fathers.insert(fathers.begin(), ind);
    }
  }
}
 
