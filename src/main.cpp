#include "main.h"
#include "hill_climbing.h"
#include <fstream>
#include <iostream>

using namespace std;

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

	// testing
	Timetable* tt = get_greedy_initial_state(a);

	cout << tt->calculateScore(a) << endl;

	const int maximum_lost_attempts = 10; // maximum number of times for consecutive lack of improved solutions
	int lost_attempts = 0;
	for(int i = 0; i < 1000 && lost_attempts < maximum_lost_attempts; i++) {
		Timetable *new_tt = get_best_neighbor(tt, a);
		if(new_tt == nullptr) {
			lost_attempts++;
			continue;
		} 
		lost_attempts = 0;
		cout << "Melhor: " << new_tt->myScore << endl;
		delete(tt);
		tt = new_tt;
	}
	
	delete (tt);
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