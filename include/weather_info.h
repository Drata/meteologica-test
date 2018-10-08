/* weather_info.h */
/* headers of the functions related to operations with weather_info structure */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "uthash.h"
#include "jWrite.h"
#include "str_util.h"

typedef struct weather_info_struct weather_info;

/*
 * Function:  create_weather_info
 * --------------------
 * Creates a weather_info report with the information passed in the arguments
 *
 *  date: string with the date of the information
 *  city: string with the name of the city
 *  max: maximum temperature
 *  min: minimum temperature
 *  prec: mm of precipitation
 *  nub: % of nubosity
 *
 *  returns: a pointer to the structure with the created weather_info or NULL
 *            if there was an error.
 */
weather_info * create_weather_info(char* date, char *city, float max, float min, float prec, int nub);

/*
 * Function: find_weather
 * --------------------
 * Searches for a certain weather_info report in the hash table
 *
 *  key: key of the weather info that you want to find
 *
 *  returns: a pointer to the structure with the encountered weather_info or NULL
 *          if no weather_info was found.
 */
weather_info * find_weather(char *key);

/*
 * Function:  add_weather
 * --------------------
 *  Adds a weather_info report to the hash table.
 *
 *  wi: pointer to the weather info that you want to add to the hash table.
 */
void add_weather(weather_info *wi);

/*
 * Function:  weather_info_to_json
 * --------------------
 *  Creates a string with the weather_info information in form of json.
 *
 *  buffer: string to store the json
 *  buflen: length of the buffer passed to store the json
 *  fahrenheit: 0 to print temperature information as celsius, 1 to print it as
 *              fahrenheit
 *
 *  wi: pointer to the weather_info you want to convert to json
 *
 *  int: code error if something went wrong, 0 if it has go correctly
 */
int weather_info_to_json(char *buffer, int buflen, int fahrenheit, weather_info * wi);

/*
 * Function:  destroy_weather_info
 * --------------------
 *  Frees the memory allocated for a weather_info
 *
 *  wi: pointer to the weather_info you want to free
 *
 */
void destroy_weather_info(weather_info * wi);

/*
 * Function:  destroy_all_weather_info
 * --------------------
 *  Frees the memory allocated for all weather_info and destroys the hash table
 */
void destroy_all_weather_info();
