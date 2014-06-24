#include <iostream>
#include<fstream>
#include <vector>
#include <string>

using namespace std;

#include "enigma.hpp"
#include "errors.h"

int main(int argc, char **argv) {
  int no_rotors, error_code;
  string plugboard_path, reflector_path,rotor_position_path;

  // set number of rotors (mean numbers or rotors and not "no rotors"
  no_rotors = argc - 4;
  // sets number to zero if there is no rotor
  if (no_rotors == -1){
    no_rotors = 0;
  } else if(no_rotors<-1) {
    cerr << "Insufficient number of parameters\n";
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }

  //save plugboard and reflectors path as variables
  plugboard_path = argv[1];
  reflector_path = argv[2];

  string rotor_path[no_rotors];
  // save rotor paths into variables
  if (no_rotors > 0) {
    rotor_position_path = argv[argc-1];
    int x = 0;
    // saves the rotors position starting the last one at number 0
    for (int i = argc-2; i >= 3; i--, x++) {
      rotor_path[x] = argv[i]; 
    } 
  } else {
    rotor_path[0] = "";
    rotor_position_path = "";
  }

  Enigma enigma(plugboard_path,reflector_path,rotor_path,
		rotor_position_path,no_rotors);

  /* do the initialisation of the components including - reading
     the files and saving them into vectors and checking for errors */

  error_code = enigma.plugboard->initialisation(plugboard_path);
  if (error_code != 0) {
    cerr <<  enigma.error_description(error_code);
    return error_code;
  }

  error_code = enigma.reflector->initialisation(reflector_path);
  if (error_code != 0) {
    cerr <<  enigma.error_description(error_code);
    return error_code;
  }
  if (no_rotors > 0) {
    for (int i=0;i<no_rotors;i++){
      error_code = enigma.rotor[i]->initialisation(no_rotors,
				    rotor_path[i],rotor_position_path,i);
      if (error_code != 0) {
	cerr <<  enigma.error_description(error_code);
	return error_code;
      }
    }  
  }
 
  /* take in entire string of characters and then map
     the string leter by letter and output back string  */
  char input,output;

  cin >> input;

  while(!(cin.eof())){
    if (!(input>='A' && input<='Z')){
      error_code = INVALID_INPUT_CHARACTER;
      cerr <<  enigma.error_description(error_code);
      return error_code;
    }
    else {
      enigma.mapping(input, no_rotors, output);
    }
    cout << output;
    cin >> input;
  }

  return 0;
}
