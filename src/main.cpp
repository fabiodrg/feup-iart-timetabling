#include "main.h"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
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
}

Instance importFile(fstream &f) {
  Instance a;
  f >> a.nEvents >> a.nRooms >> a.nFeatures >> a.nStudents; // read first line

  cout << "Loaded " << a.nEvents << " events, " << a.nRooms << " rooms, "
       << a.nFeatures << " features and " << a.nStudents << " students. "
       << endl;

  for (int i = 0; i < a.nRooms; i++) {
    int size;
    f >> size;
    a.rooms.push_back(Room(size, i));
  }

  for (int i = 0; i < a.nStudents; i++) {
    a.students.push_back(Student(i));
  }

  for (int i = 0; i < a.nEvents; i++) {
    a.events.push_back(Event(i));
  }

  for (int i = 0; i < a.nFeatures; i++) {
    a.features.push_back(Feature(i));
  }

  for (int i = 0; i < a.nRooms; i++) {
    cout << "Room " << i << " has " << a.rooms[i].getSize() << " seats."
         << endl;
  }

  for (size_t i = 0; i < a.students.size(); i++) {
    Student s = a.students[i];
    for (size_t j = 0; j < a.events.size(); j++) {
      Event *e = &a.events[j];
      int toadd;
      f >> toadd;
      if (toadd)
        e->addAtendee(s);
    }
  }

  for (size_t i = 0; i < a.rooms.size(); i++) {
    Room *r = &a.rooms[i];
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
    Event *e = &a.events[i];
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

  return a;
}