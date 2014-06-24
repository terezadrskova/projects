#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <sstream>

#include "reflector.hpp"
#include "errors.h"

using namespace std;

int Reflector::mapping(int input_letter){
  for (unsigned i=0; i<conf_vector.size(); i++) {
    if (input_letter == conf_vector.at(i)) {
      if (i%2 != 0){
        input_letter = conf_vector.at(i-1);
        return input_letter;
       } else if(i%2 == 0)  {
        input_letter = conf_vector.at(i+1);
        return input_letter;
      }
    }
  }
  return input_letter;
}

// returns error value
int Reflector::initialisation(const string& reflector_path){
  // PARSING THE FILE
  ifstream fin;
  fin.open(reflector_path.c_str(),ifstream::in);
  if(!fin.is_open()){
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  // save values into the vector
  string temp;
  int number;
  fin >> temp;
  while(!fin.eof()){
    for(unsigned int i=0; i<temp.size(); i++){
      if(!isdigit(temp[i]))
	return NON_NUMERIC_CHARACTER;
    }
    istringstream stream(temp);
    stream >> number; 
    conf_vector.push_back(number);
    fin >> temp;
  }
  fin.close();

  // ***** CHECKING FOR THE ERRORS ******
  // error if input file contains odd number of numbers
  if (conf_vector.size() != 26) {
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }  

  for (unsigned i=0; i<conf_vector.size(); i++) {
     // error if the number is not between 0 and 25
    if (conf_vector.at(i) < 0 || conf_vector.at(i) > 25) {
      return INVALID_INDEX;
    }
  }

  /* error if the file tries to connect with itself
     or with more than one other contact
     copying into a new vector, otherwise the mapping
     inhertis the sort ordering */
  vector<int> copy_vector = conf_vector;
  //sort(conf_vector.begin(),conf_vector.end());
  for (unsigned i=0; i<copy_vector.size()-1; i++) {
    if (copy_vector.at(i) == copy_vector.at(i+1)) {
      return INVALID_REFLECTOR_MAPPING;
    }
  }
  return NO_ERROR;
}

