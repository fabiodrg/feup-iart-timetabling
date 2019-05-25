#include "../src/model.h"
#include "../src/hill_climbing.h"
#include <iostream>
#include <fstream>
#include <set>
using namespace std;

void assert_equal(int expected, int result, string test)
{
    static int test_no = 1;
    cout << "Test " << test_no++ << ": " << test << " .";
    if (expected == result) {
        cout << " OK" << endl;
    } else {
        cout << " FAILED. Expected " << expected << ". Got " << result << "." << endl;
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

// needs to be updated but whatever
void test_score_calculation()
{
    Instance inst;

    Feature f1(1);
    Feature f2(2);
    Feature f3(3);
    inst.features = {f1, f2, f3};
    

    Room r1(2, 1);
    r1.addFeature(f1);
    Room r2(10, 2);
    r2.addFeature(f1);
    inst.rooms = {r1, r2};

    Student s1(1);
    Student s2(2);
    Student s3(3);
    Student s4(4);
    inst.students = {s1, s2, s3, s4};

    Event e1(1);
    e1.addFeature(f1);
    e1.addAtendee(s1);

    Event e2(2);
    e2.addFeature(f1);
    e2.addAtendee(s1);

    inst.events = {e1, e2};

    /**
     * @brief Student can't have multiple events on same timeslot
     */
    Timetable tt(inst);
    TimeSlot ts1;
    int penalty_acc = 0;

    tt.timetable[0][0].addScheduledEvent(&inst.rooms.at(0), &inst.events.at(0));
    assert_equal(penalty_acc, tt.calculateScore(inst), "Single event for student per timeslot");

    tt.timetable[1][0].addScheduledEvent(&r1, &e2);
    assert_equal(penalty_acc, tt.calculateScore(inst), "Single event for student per timeslot");

    tt.timetable[0][0].addScheduledEvent(&r2, &e2);
    penalty_acc += PENALTY_STUDENT_OVERLAPPED_EVENTS;
    assert_equal(penalty_acc, tt.calculateScore(inst), "Overlapped events for student");

    /**
     * Rooms must have enough capacity for holding an event
     */
    TimeSlot ts2;
    e1.addAtendee(s2);
    ts2.addScheduledEvent(&r1, &e1);
    tt.timetable[0][0] = ts2;
    assert_equal(0, tt.calculateScore(inst), "Room still has space");
    
    TimeSlot ts3;
    e1.addAtendee(s3);
    e1.addAtendee(s4);
    ts3.addScheduledEvent(&r1, &e1);
    tt.timetable[0][0] = ts3;
    assert_equal(PENALTY_ROOM_OUT_OF_SPACE, tt.calculateScore(inst), "Room out of space");

    ts3.addScheduledEvent(&r2, &e2);
    tt.timetable[0][0] = ts3;
    assert_equal(PENALTY_ROOM_OUT_OF_SPACE + PENALTY_STUDENT_OVERLAPPED_EVENTS, tt.calculateScore(inst), "Room out of space & overlapped event");
}

void test_duplicated_events(char *filename) {
    fstream inputFile;
	inputFile.open(filename, ios::in);
    Instance a = importFile(inputFile);
	inputFile.close();

    Timetable* tt = stochastic_hill_climbing(a, get_greedy_initial_state);
    set<Event*> events;

    for(int i = 0; i < TIMETABLE_NUMBER_DAYS; i++) {
		for(int j = 0; j < TIMETABLE_SLOTS_PER_DAY; j++) {
			TimeSlot &slot = tt->timetable[i][j];
			vector<Event *> allocated_events = slot.getAllocatedEvents();
			
            for(Event *ev : allocated_events) {
                pair<set<Event*>::iterator, bool> p = events.insert(ev);
                if(!p.second) {
                    cout << "Event with ID " << ev->getId() << "is duplicated" << endl;
                } 
            }
		}
	}
}

int main(int argc, char** argv) {
    test_duplicated_events(argv[1]);
}