#include <fstream>
#include <iostream>
#include "main.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: ./main <input-file>" << endl;
        return 1;
    }

    fstream inputFile;
    inputFile.open(argv[1], ios::in);

	if(!inputFile){
		cout << "The file "<< argv[1] << " could not be found. Please try again."<< endl;
		return 2;
	}

	// we got a file and could open it. Let's proceed.

	importFile(inputFile);

	inputFile.close();
}

void importFile(fstream &f){

}