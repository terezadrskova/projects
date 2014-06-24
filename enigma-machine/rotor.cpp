#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <algorithm> 
#include <sstream>

using namespace std;

#include "rotor.hpp"
#include "errors.h"


/* check for the notches, if the notch corresponds with the 
   the character at the top of rotor then return tue to
   rotate the next rotor */
bool Rotor::rotate(){
  position++;
  if(position == 26)
    position = 0;

  for (unsigned i=26; i<conf_vector.size(); i++) {
    if (conf_vector[i] == position){
      // found notch with configuration: conf_vector[i]
      return true;
    }
  }
  // didn't find a notch (next rotor won't rotate)
  return false;
}

/* goes through the rotor and does the mapping depending
   whether going backward or forward */
int Rotor::mapping(int input_letter, bool forward){
  int temp_position;
  int temp = input_letter+position;

  temp = (temp+26)%26;
  
  // ----- going forward-----
  if (forward == true) {
    input_letter =  conf_vector.at(temp)-position;
    input_letter = (input_letter+26)%26;
  } 
  // ----- going backward -----
  else {
    for (unsigned i=0; i<26; i++) {
      if (temp==conf_vector.at(i)){
	temp_position = i;
	i = 26;
      }
      input_letter =temp_position;
    }
    input_letter = input_letter-position;
  }
  // check if the number is between 0-25
  input_letter = (input_letter+26)%26;
  return input_letter;
}

// returns error value
int Rotor::initialisation(int no_rotors, string rotor_path, 
		  string rotor_position_path, int current_rotor){
  //----- INITIALISE THE ROTOR POSITION FILE ----
  vector<int> rotor_pos_vector;
  ifstream fin;
  // open if stream with the rotor position file
  fin.open(rotor_position_path.c_str(),ifstream::in);
  if(!fin.is_open()){
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  // save values into the vector
  string temp;
  int number;
  fin >> temp;
  while(!fin.eof()){
    for(unsigned int i=0; i<temp.size(); i++){
      // check if the number is numeric character
      if(!isdigit(temp[i]))
	return NON_NUMERIC_CHARACTER;
    }
    istringstream stream(temp);
    stream >> number; 
    // check if the number is between 0 and 25
    if (number < 0 || number > 25) 
        return INVALID_INDEX;
    rotor_pos_vector.push_back(number);
    fin >> temp;
  }
  fin.close();
    // check if there are enough starting position for the rotors
  if (rotor_pos_vector.size()<no_rotors)
    return NO_ROTOR_STARTING_POSITION;
  // define starting position for the rotor

    
  position = rotor_pos_vector[no_rotors-current_rotor-1];

  //------- PARSING THE ROTOR CONFIG FILE FOR EACH ROTOR--------
  fin.open(rotor_path.c_str(),ifstream::in);
  if(!fin.is_open()){
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  // save values into the vector
  fin >> temp;
  while(!fin.eof()){
    for(unsigned int i=0; i<temp.size(); i++){
      // check if the number is numeric character
      if(!isdigit(temp[i]))
	return NON_NUMERIC_CHARACTER;
    }
    istringstream stream(temp);
    stream >> number; 
    // check if the number is between 0 and 25
    if (number < 0 || number > 25) 
        return INVALID_INDEX;
    conf_vector.push_back(number);
    fin >> temp;
  }
  fin.close();

  if (conf_vector.size()<26){
    return INVALID_ROTOR_MAPPING;
  }

  //int no_notches_array[26];
  vector<int> no_notches_array;
  for (unsigned int i=0; i<26; i++){
    //array without notches (cannot do sort with the notches)
    no_notches_array.push_back(conf_vector.at(i));
  }

  sort(no_notches_array.begin(),no_notches_array.end());
  for (unsigned int i=0; i<26; i++){
    if (no_notches_array[i]==no_notches_array[i+1] && i!=25) {
      return INVALID_ROTOR_MAPPING;
    }
  }

  return NO_ERROR; 
}

