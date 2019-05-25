#include "main.h"
#include "genetic.h"
#include "hill_climbing.h"
#include "menu.h"
#include <fstream>
#include <iostream>

using namespace std;

const void runGenetic(Instance* a) {
	Timetable* t2 = goGenetic(a, 100, 5000);
	cout << "Genetic Score: " << t2->calculateScore() << endl;
	delete (t2);
}

const void runHillClimbing(Instance* a) {
	Timetable* tt = first_choice_hill_climbing(*a, get_greedy_initial_state);
	cout << *tt;
	delete (tt);
}

int main(int argc, char** argv) {
	srand(time(NULL));

	if (argc != 2) {
		cout << "usage: ./main <input-file>" << endl;
		return 1;
	}

	fstream inputFile;
	inputFile.open(argv[1], ios::in);

	if (!inputFile) {
		cout << "The file " << argv[1] << " could not be found. Please try again."
		     << endl;
		return 2;
	}

	// we got a file and could open it. Let's proceed.

	Instance a = importFile(inputFile);
	inputFile.close();

  Menu menu("Timetabling", NULL, NULL, 0);

	menu.addOption("Genetic", 'g', runGenetic, &a);
	menu.addOption("Hill Climbing", 'h', runHillClimbing, &a);

	while (1) {
		menu.print();
		menu.waitAndRun();
	}
}

Instance importFile(fstream& f) {
	Instance a;
	int nEvents, nRooms, nFeatures, nStudents;
	f >> nEvents >> nRooms >> nFeatures >> nStudents; //read first line

	cout << "Loaded " << nEvents << " events, " << nRooms << " rooms, " << nFeatures << " features and " << nStudents << " students. " << endl;

	for (int i = 0; i < nRooms; i++) {
		int size;
		f >> size;
		a.rooms.push_back(Room(size, i));
	}

	for (int i = 0; i < nStudents; i++) {
		a.students.push_back(Student(i));
	}

	for (int i = 0; i < nEvents; i++) {
		a.events.push_back(Event(i));
	}

	for (int i = 0; i < nFeatures; i++) {
		a.features.push_back(Feature(i));
	}

	for (int i = 0; i < nRooms; i++) {
		cout << "Room " << i << " has " << a.rooms[i].getSize() << " seats." << endl;
	}

	for (size_t i = 0; i < a.students.size(); i++) {
		Student s = a.students[i];
		for (size_t j = 0; j < a.events.size(); j++) {
			Event* e = &a.events[j];
			int toadd;
			f >> toadd;
			if (toadd)
				e->addAtendee(s);
		}
	}

	for (size_t i = 0; i < a.rooms.size(); i++) {
		Room* r = &a.rooms[i];
		for (size_t j = 0; j < a.features.size(); j++) {
			Feature fe = a.features[j];
			int toadd;
			f >> toadd;
			if (toadd)
				r->addFeature(fe);
		}
		cout << "Room " << r->getId() << " has " << r->getNumberOfFeatures()
		     << " features." << endl;
	}

	for (size_t i = 0; i < a.events.size(); i++) {
		Event* e = &a.events[i];
		for (size_t j = 0; j < a.features.size(); j++) {
			Feature fe = a.features[j];
			int toadd;
			f >> toadd;
			if (toadd)
				e->addFeature(fe);
		}
		cout << "Event " << e->getId() << " has " << e->getNumberOfFeatures()
		     << "  features and " << e->getNumberOfAtendees() << " atendees."
		     << endl;
	}

	// sort rooms by capacity (ascending)
	a.sortRoomsByCapacity();

	return a;
}
