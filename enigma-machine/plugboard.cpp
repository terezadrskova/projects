#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <sstream>

using namespace std;

#include "plugboard.hpp"
#include "errors.h"

// swapping the pairs of numbers from the plugboard config file
int Plugboard::mapping(int input_letter){
for (unsigned i=0; i<conf_vector.size(); i++) {
  if (input_letter == conf_vector.at(i)) {
       if (i%2 != 0){
	 // cout << "vector number i: " << i << endl;
	 input_letter = conf_vector.at(i-1);
	 return input_letter;
       }
       else if(i%2 == 0)  {
	 //	 cout << "vector number i: " << i << endl;
	 input_letter = conf_vector.at(i+1);
	 return input_letter;
       }
    }
  }
  return input_letter;
}

// initisation of the plugboard and checking for the errors
int Plugboard::initialisation(const string& plugboard_path){
  // PARSING THE FILE
  ifstream fin;
  fin.open(plugboard_path.c_str(),ifstream::in);
  if(!fin.is_open()){
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  // save values into the vector and check for errors
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
    conf_vector.push_back(number);
    fin >> temp;
  }
  fin.close();

  // ***** CHECKING FOR THE ERRORS ******
  // error if input file contains odd number of numbers
  if (conf_vector.size()%2 != 0) {
    return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
  } 

  /* error if the file tries to connect with itself
     or with more than one other contact
     copying into a new vector, otherwise the mapping
     inhertis the sort ordering */
  vector<int> copy_vector = conf_vector;
  if (copy_vector.size()>=2){
    //sort(copy_vector.begin(),copy_vector.end());
    for (unsigned i=0; i<copy_vector.size()-1; i++) {
       if (copy_vector.at(i) ==copy_vector.at(i+1)) {
	 return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
       }
    }
  }
  return NO_ERROR;
}
