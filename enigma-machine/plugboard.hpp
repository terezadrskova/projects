#ifndef PLUGBOARD_HPP
#define PLUGBOARD_HPP

#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Plugboard {
private:
  vector<int> conf_vector;
public:
  int mapping(int input_letter);
  int initialisation(const string& plugboard_path);
};
#endif

