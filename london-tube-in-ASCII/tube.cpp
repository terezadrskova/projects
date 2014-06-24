#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <sstream>

using namespace std;


#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

/*  QUESTION 1 - function to get the coordinates of the station symbol 
    later used in the validate_route() to find the location
    of the current symbol */
bool get_symbol_position(char **map, int height, int width, char target, int& r, int& c){
   // defines all the stations on the map
  if((target>='A' && target <='Z') || 
     (target>='0' && target <='9') || 
     (target >='a' && target <= 'q')){  
    for (int rows = 0; rows<height; rows++) {
    // T: changing row numbers
      for (int cols = 0; cols<width; cols++) {
       // T: changing col numbers
        if (map[rows][cols]==target){
	  r = rows;
	  c = cols;
        } 
      }  
    }
    return true;
  }
  // for all the stations which are NOT on the map
  else{
    // T: setting the coordinates to [-1][-1] is station is not on the map
    r = -1;
    c = -1;
    return false;
  }
}

/*  QUESTION 2 - function returning the symbol for specified station 
    or tube line
    this symbol will be used later in the function validate_route()
    to check the validity of the start/endpoint station, 
    route, direction, etc */
char get_symbol_for_station_or_line(const char name[]) {
  char ch[MAX_SIZE_STRING];

  // Lines - In-stream lines.txt to compare each line with the given keyword 
  ifstream in_stream_lines;
  in_stream_lines.open("lines.txt");

  // what to do if in-stream lines.txt fails 
  if (in_stream_lines.fail()) { 
    cout << "Sorry, the file lines.txt couldn't be opened!\n";
    return 0;
  } 

  // loop for going through the textlines in the lines.txt
  while (!in_stream_lines.fail()) {
    in_stream_lines.getline(ch, MAX_SIZE_STRING);
    if (strcmp(ch+2,name) == 0)
     return ch[0];
  }
  in_stream_lines.close();

  // Stations - In-stream stations.txt to compare each line with the given keyword 
  ifstream in_stream_stations;
  in_stream_stations.open("stations.txt");

  // what to do if in-stream stations.txt fails 
  if (in_stream_stations.fail()) { 
    cout << "Sorry, the file stations.txt couldn't be opened!\n";
    return 0;
  }  

  // loop for going through the lines in the stations.txt 
  while (!in_stream_stations.fail()) {
    in_stream_stations.getline(ch, MAX_SIZE_STRING);
    if (strcmp(ch+2,name) == 0) 
     return ch[0];
  }
  in_stream_stations.close();
  // return blank space if the symbol is not found in stations.txt or lines.txt
  return ' ';

} // end of function  get_symbol_for_station_or_line


/* QUESTION 3 - main function to validate the route
   this function validates the route and throws an error in the case of
   invalid start/endpoint station, invalid direction, being out of bound 
   or off the track,hopping between stations  backtracking */

int validate_route(char **map, int height,int  width, const char start_station[MAX_SIZE_STRING], char route[512], char *destination) {
  int r, c, valid_start_station_temp, valid_endpoint_station_temp, changes = 0;
  char symbol, current_symbol, old_symbol, new_symbol;
  stringstream ss(route);
  string token;

  // checking if the start station is valid
  valid_start_station_temp = station_invalid(start_station);
  if (valid_start_station_temp == -1)
    return ERROR_START_STATION_INVALID;

  // getting symbol for the start station from get_symbol_position()
  symbol = get_symbol_for_station_or_line(start_station);

  // call function to get back the coordinates of the current location
  get_symbol_position(map,height,width,symbol,r, c);

  // get array of the directions, and number of directions.
  Direction dir_array[512];
  int dir_size = 0;
  if(!give_me_direction_array(route,dir_array,dir_size))
     return ERROR_INVALID_DIRECTION;

  // step from 0 to the size of the directions
  for(int route_step = 0; route_step<dir_size; route_step++) {

    Direction new_direction = dir_array[route_step];

    // getting previous symbol
    if (route_step >= 1 &&  route_step<dir_size)
      old_symbol = current_symbol;

     // symbol before changing coordinates
    current_symbol = map[r][c]; 

    // changing the coordinates for r and c for each step
    switch (new_direction){
      case N: r--;
	      break;
      case S: r++;
	      break;
      case W: c--;
	      break;
      case E: c++;
             break;
      case NE: c++;
               r--;
               break;
      case NW: c--;
               r--;
               break;
      case SE: c++;
               r++;
               break;
      case SW: c--;
               r++;
               break;
      default: return ERROR_INVALID_DIRECTION; 
    } 
    
    // outside of the bounds  error
    if ((r >= height) || (c >= width) || (r < 0) || (c < 0))
     return ERROR_OUT_OF_BOUNDS;
   
    //  symbol after changing coordinates
    new_symbol = map[r][c]; 

    if (new_symbol == ' ') 
      return ERROR_OFF_TRACK; 
    // hopping between stations error
    if ((current_symbol=='*' || current_symbol=='-' ||
	 current_symbol=='#' || current_symbol=='&' ||
	 current_symbol=='$' || current_symbol=='+' ||
	 current_symbol=='|' || current_symbol=='>' || 
	 current_symbol=='<')
	 &&
	(new_symbol=='*' || new_symbol=='-' ||
	 new_symbol=='#' || new_symbol=='&' ||
	 new_symbol=='$' || new_symbol=='+' ||
	 new_symbol=='|' || new_symbol=='>' || 
	 new_symbol=='<'))  {
      // check if it's the same line
      if (current_symbol!=new_symbol) 
	 return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
    }
    
   // check for backtracking between the stations
    if ((dir_array[route_step]==N && dir_array[route_step+1]==S) ||
	(dir_array[route_step]==S && dir_array[route_step+1]==N) ||
	(dir_array[route_step]==W && dir_array[route_step+1]==E) ||
	(dir_array[route_step]==E && dir_array[route_step+1]==W) || 
	(dir_array[route_step]==NE && dir_array[route_step+1]==SW) || 
	(dir_array[route_step]==SW && dir_array[route_step+1]==NE) ||
	(dir_array[route_step]==NW && dir_array[route_step+1]==SE) || 
	(dir_array[route_step]==SE && dir_array[route_step+1]==NW)) { 
      // check if the change of direction was inside the station
      if((new_symbol>='A' && new_symbol <='Z') ||
	 (new_symbol>='0' && new_symbol <='9') || 
	 (new_symbol >='a' && new_symbol <= 'q')) {
      } else {
	 // return backtracking between stations error
         return ERROR_BACKTRACKING_BETWEEN_STATIONS;
      }
    }
    
    // find how many changes has been made during the journey
    // startign from step 1 - otherwise old symbol is undefined
    if (route_step >= 1 && route_step<dir_size){
      // check if the symbol before and after station is the same
      if ((old_symbol=='*' || old_symbol=='-' ||
	   old_symbol=='#' || old_symbol=='&' ||
	   old_symbol=='$' || old_symbol=='+' ||
	   old_symbol=='|' || old_symbol=='>' || 
	   old_symbol=='<')
	 &&
	(new_symbol=='*' || new_symbol=='-' ||
	 new_symbol=='#' || new_symbol=='&' ||
	 new_symbol=='$' || new_symbol=='+' ||
	 new_symbol=='|' || new_symbol=='>' || 
	 new_symbol=='<'))  {
	if (old_symbol!=new_symbol){
	  changes++;
	}
      }
     } 

  } 
  // get the name of the endpoint station
  get_destination(map,r,c,destination);
 
  // checking if the endpoint station is correct
  valid_endpoint_station_temp = station_invalid(destination);
  if (valid_endpoint_station_temp == -1) {
     // return error code for invalid endpoint station
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
    } 
   // T: returns number of changes - excluding endpoint
  return changes;
} // end of validate_route() function

// function to validate the start and endpoint station name 
int station_invalid(const char station[MAX_SIZE_STRING]){
  char textline[MAX_SIZE_STRING];

  // Stations -  compare each line with the given station
  ifstream in_stream_stations;
  in_stream_stations.open("stations.txt");
 
  // loop for going through the lines in the stations.txt 
  while (!in_stream_stations.fail()) {
    in_stream_stations.getline(textline, MAX_SIZE_STRING);
    if (strcmp(textline+2,station) == 0) {
      // station is on the map
      return 0;
    }
  }
   // station is not on the map
  return -1;
} 

// function to find the coordinates for the current locationh
void get_destination(char **map, int& r, int& c, char *destination){
  char ch[MAX_SIZE_STRING];
  
  // Stations -  compare each line with the given keyword 
  ifstream in_stream_stations;
  in_stream_stations.open("stations.txt");

  // loop for going through the lines in the stations.txt 
  while (!in_stream_stations.fail()) {
   in_stream_stations.getline(ch, MAX_SIZE_STRING);
   if (ch[0] ==  map[r][c]) 
     strcpy(destination, ch+2); 
  } 
  in_stream_stations.close();
}

// function to return the direction array
bool give_me_direction_array(char route[],Direction output_array[],int& size)
{
  char *pch;
  // string token function to get tokens
  pch = strtok(route,",");
  // check if the direction are only commas or nothing - returns false
  if(pch==NULL)
    return false;
  while(pch) {
    Direction this_direction = string_to_direction(pch);
    if(this_direction==INVALID_DIRECTION)
      return false;
    output_array[size++]=this_direction;
    pch = strtok(NULL,",");
  }
  return true;
}
