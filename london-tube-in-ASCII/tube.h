#include <string>

enum Direction {N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION};

/* error codes for Question 3 */
#define ERROR_START_STATION_INVALID -1 
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION -5
#define ERROR_OFF_TRACK -6
#define ERROR_OUT_OF_BOUNDS -7

/* define global constants */
const int MAX_SIZE_STRING = 40;

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width);

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width);

/* pre-supplied helper function to describe errors for Question 3 */
const char *error_description(int code);

/* presupplied helper function for converting string to Direction enum */
Direction string_to_direction(const char *token);


/* Question 1 - function to get position of the symbol */
bool  get_symbol_position(char **map, int height, int width, char target, int& r, int& c);


/* Question 2 - function return the symbol for specified station or tube line */
char get_symbol_for_station_or_line(const char name[]);

/* Question 3 - the main function to validate the route */
int validate_route(char **map, int height,int  width, const char start_station[MAX_SIZE_STRING], char route[512], char destination[512]);

/* Question 3 - function to validate the start and endpoint station  */
int station_invalid(const char station[MAX_SIZE_STRING]);

/* Question 3 - function which returns the endpoint station (destination)  */
void get_destination(char **map, int& r, int& c, char *destination);

/* Question 3 - function to return the direction array  */
bool give_me_direction_array(char route[],Direction output_array[],int& size);
