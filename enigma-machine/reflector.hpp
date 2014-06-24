#ifndef REFLECTOR_HPP
#define REFLECTOR_HPP

#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Reflector { 
private:
  vector<int> conf_vector;
public:
  int mapping(int input_letter);
  int initialisation(const string& reflector_path);
};
#endif
