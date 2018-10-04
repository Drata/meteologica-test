/* weather_info.case */
/* implementation of functions related to operations with the weather_info structure */
#include "../include/weather_info.h"

#define C_TO_F 9/5 + 32

weather_info * hash = NULL; // hash table for all weather info

struct weather_info_struct {
    char* m_date; //date of the weather report
    char* m_city; //city of the weather report
    float m_min; //minimum temperature in celsius
    float m_max; //maximum temperature in celsius
    float m_prec; //precipitation in mm
    int m_nub; //nubosity in %

    UT_hash_handle hh; //makes this structure hashable
};

weather_info * create_weather_info(char* date, char *city, float max, float min, float prec, int nub) {

    // allocates memory for the structure
    weather_info * wi = (weather_info *) malloc (sizeof(weather_info));

    // allocates memory for strings
    wi->m_date = (char*) malloc (strlen(date) * sizeof(char));
    wi->m_city = (char*) malloc (strlen(city) * sizeof(char));

    // initialize structure variables with the arguments of the function
    strcpy(wi->m_date, date);
    strcpy(wi->m_city, city);
    wi->m_min = min;
    wi->m_max = max;
    wi->m_prec = prec;
    wi->m_nub = nub;

    return wi;
}

void add_weather(weather_info *wi) {

    char *date, *city;

    city = wi->m_city;
    date = wi->m_date;

    // allocates memory for key string
    char *dest = (char*) malloc ((strlen(city) + strlen(date)) * sizeof(char));

    // concatenates city and date to create the hash key
    sprintf(dest, "%s%s", city, date);

    // adds weather info to the hash table
    HASH_ADD_KEYPTR(hh, hash, dest, strlen(dest), wi);
}

weather_info * find_weather(char *key) {

    weather_info *wi;

    // looks up table for the desired weather_info
    HASH_FIND_STR(hash, key, wi);

    return wi;
}

int weather_info_to_json(char *buffer, int buflen, int fahrenheit, weather_info * wi) {
    int err;
    double max, min;
    // adds name of object to buffer
    char *object_name = "weather_info:\n";
    strcpy(buffer, object_name);

    // converts temperature if necessary
    if(fahrenheit == 1) {
        max = wi->m_max * C_TO_F;
        min = wi->m_min* C_TO_F;
    } else {
        max = wi->m_max;
        min = wi->m_min;
    }

    // opens buffer for writting json
    jwOpen(buffer + strlen(object_name), buflen - strlen(object_name), JW_OBJECT, JW_PRETTY);

    // writes pairs of key - value for json objects
    jwObj_string("ciudad", wi->m_city);
    jwObj_string("fecha", wi->m_date);
    jwObj_double("tmax", max);
    jwObj_double("tmin", min);
    jwObj_double("prec", wi->m_prec);
    jwObj_int("nub", wi->m_nub);

    // closes json and gets error code
    err = jwClose();

    return err;
}

void destroy_weather_info(weather_info * wi) {
    // frees memory allocated for strings
    free(wi->m_date);
    free(wi->m_city);

    // frees memory allocated for structure
    free(wi);
}

void destroy_all_weather_info() {
    weather_info *current, *tmp;

    // iterates through all elements in hash table and removes them from memory
    HASH_ITER(hh, hash, current, tmp) {
        HASH_DEL(hash, current);
        destroy_weather_info(current);
    }
}
