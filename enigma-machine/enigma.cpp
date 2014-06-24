#include <iostream>
#include<fstream>
#include <vector>
#include <string> 
#include <cstring> 

using namespace std;

#include "enigma.hpp"
#include "plugboard.hpp"
#include "reflector.hpp"
#include "rotor.hpp"
#include "errors.h"

Enigma::Enigma(string plugboard_path,string reflector_path,string rotor_path[],
               string rotor_position_path, int no_rotors) {
  // creating 1 plugboard and 1 reflector
  plugboard = new Plugboard();
  reflector = new Reflector(); 
  
  if (no_rotors>0) {
    //creating n number of rotors
    rotor = new Rotor*[no_rotors];
    for (int i=0;i<no_rotors;i++){
        rotor[i] = new Rotor();
    }
  }
  no_of_rotors = no_rotors;
}

Enigma::~Enigma() {
  if (no_of_rotors>0){
    for (int i=no_of_rotors-1;i>=0;--i){
      if (rotor[i]) {
	delete rotor[i];
	rotor[i] = NULL;
      }
    }
    if(rotor){
      delete rotor;
      rotor = NULL;
    }
  }
  if (reflector){
    delete reflector;
    plugboard = NULL;
  }
  if (plugboard) {
    delete plugboard;
    plugboard = NULL;
  }
}

// switch the values for the reflector config file
void Enigma::mapping(int value, int no_rotors, char &output){
  int input_number = value - 65;

  if(no_rotors!=0){ // Mechanical Rotation
    int i = 0;
    while(i<no_rotors && rotor[i]->rotate()){
      i++;
    }
  }

  //------- MAPPING FORWARD----------
  input_number = plugboard->mapping(input_number);

  if (no_rotors>0){
    bool is_forward = true;
    for (int i=0;i<no_rotors;i++){ 
      input_number = rotor[i]->mapping(input_number, is_forward);
    }
  }

  input_number = reflector->mapping(input_number);
  //--------MAPPING BACKWARD---------
  if (no_rotors>0){
    bool is_forward = false;
    for (int i=no_rotors-1; i>=0;i--){
      input_number = rotor[i]->mapping(input_number, is_forward);
    }
  }

  input_number = plugboard->mapping(input_number);
  // produce output letter: switch from number to character
  char output_letter = input_number+65;
  //output+=output_letter;
  output=output_letter;
 }
 
const char* Enigma::error_description(int error_code) {
  switch(error_code) {
  case INSUFFICIENT_NUMBER_OF_PARAMETERS: 
    return "Insufficient number of parameters\n";
  case INVALID_INPUT_CHARACTER:
    return "The input character is not valid\n";
  case INVALID_INDEX:
    return "The index is not valid\n";
  case NON_NUMERIC_CHARACTER:
    return "Non numeric character\n";
  case IMPOSSIBLE_PLUGBOARD_CONFIGURATION:
    return "Invalid plugboard configuration\n";
  case INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS:
    return "Incorrect number of plugboard parameters\n";
  case INVALID_ROTOR_MAPPING:
    return "Invalid rotor mapping\n";
  case NO_ROTOR_STARTING_POSITION:
    return "Insufficient number of starting positions\n";
  case INVALID_REFLECTOR_MAPPING:
    return "Invalid reflector mapping\n";
  case INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS:
    return "Incorrect number of reflector parameters\n";
  case ERROR_OPENING_CONFIGURATION_FILE:
    return "Error during opening the configurtion file\n";
  }
  return "Unknown error\n";
}


