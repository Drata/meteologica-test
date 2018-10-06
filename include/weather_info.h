/* weather_info.h */
/* headers of the functions related to operations with weather_info structure */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "uthash.h"
#include "jWrite.h"
#include "str_util.h"

typedef struct weather_info_struct weather_info;

weather_info * create_weather_info(char* date, char *city, float max, float min, float prec, int nub);
weather_info * find_weather(char *key);
void add_weather(weather_info *wi);
int weather_info_to_json(char *buffer, int buflen, int fahrenheit, weather_info * wi);
void destroy_weather_info(weather_info * wi);
void destroy_all_weather_info();
