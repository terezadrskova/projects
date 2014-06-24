#ifndef ENIGMA_HPP
#define ENIGMA_HPP

#include <string>

#include "plugboard.hpp"
#include "reflector.hpp"
#include "rotor.hpp"


class Enigma {
private:
  string rotor_path;
  int error_code;
  int no_of_rotors;
public:
  Plugboard *plugboard;
  Reflector *reflector;
  Rotor **rotor;
  Enigma(string plugboard_path,string reflector_path,string rotor_path[], 
	   const string rotor_position_pat,int no_rotors);
  void mapping(int value, int no_rotors,char &output);
  const char* error_description(int error_code);
  ~Enigma();
};
#endif

