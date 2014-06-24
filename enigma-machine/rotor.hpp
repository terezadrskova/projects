#ifndef ROTOR_HPP
#define ROTOR_HPP

#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Rotor {
private:
  vector<int> conf_vector;   // Rotor configuration
  int position;                // Starting position of the rotor
public:
  int mapping(int input_letter, bool forward);
  bool rotate();
  int initialisation(int no_rotors,string rotor_path, string rotor_position_path,int current_rotor);
};
#endif
