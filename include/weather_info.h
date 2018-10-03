/* weather_info.h */
/* headers of the functions related to operations with weather_info structure */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "uthash.h"

typedef struct weather_info_struct weather_info;

weather_info * create_weather_info(time_t date, char *city, float min, float max, int prec, int nub);
weather_info * find_weather(char *key);
void add_weather(weather_info *weather);
void print_weather_info(weather_info * wi);
