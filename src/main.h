#include "model.h"
#include <fstream>

using namespace std;

/**
 * @brief Reads the file contents and populates an Instance with the relevant
 * information.
 * @param an fstream of the input file
 * @return the instance of the problem the passed file represents
 */
Instance importFile(fstream& f);

/**
 * @brief Entry point. Input file name should be used as first argument.
 * @param argc
 * @param argv
 */

int main(int argc, char** argv);