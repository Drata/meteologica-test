/* weather_info.case */
/* implementation of functions related to operations with the weather_info structure */
#include "../include/weather_info.h"

#define MAX_BUFFER 1024
#define C_TO_F 9/5 + 32

weather_info * hash = NULL;

struct weather_info_struct {
    time_t m_date; //date of the weather report
    char* m_city; //city of the weather report
    float m_min; //minimum temperature in celsius
    float m_max; //maximum temperature in celsius
    int m_prec; //precipitation in mm
    int m_nub; //nubosity in %

    UT_hash_handle hh; //makes this structure hashable
};

weather_info * create_weather_info(time_t date, char *city, float min, float max, int prec, int nub) {

    weather_info * wi = (weather_info *) malloc (sizeof(weather_info));

    wi->m_date = date;
    wi->m_city = city;
    wi->m_min = min;
    wi->m_max = max;
    wi->m_prec = prec;
    wi->m_nub = nub;

    return wi;
}

void add_weather(weather_info *weather) {

    HASH_ADD_KEYPTR(hh, hash, weather->m_city, strlen(weather->m_city), weather);
}

weather_info * find_weather(char *key) {

    weather_info * wi = (weather_info *) malloc (sizeof(weather_info));

    HASH_FIND_STR(hash, key, wi);

    return wi;
}

void print_weather_info(weather_info * wi) {
    char *c_time_string = ctime(&wi->m_date);

    printf("\n\n---------------------------------------------\n");

    printf("Fecha: %s\n", c_time_string);
    printf("Ciudad: %s\n", wi->m_city);
    printf("Temperatura minima (C): %.2f\n", wi->m_min);
    printf("Temperatura maxima (C): %.2f\n", wi->m_max);
    printf("Precipitacion (mm): %d \n", wi->m_prec);
    printf("Nubosidad: %d \n", wi->m_nub);

    printf("\n---------------------------------------------\n\n");
}

int weather_info_to_json(char *buffer, int buflen, weather_info * wi) {
    int err;

    // adds name of object to buffer
    char *object_name = "weather_info:\n";
    strcpy(buffer, object_name);

    // converts date to string
    const char *src = ctime(&wi->m_date);
    char *cstring = (char* ) malloc (sizeof(char) * (strlen(src)-1));
    strncpy(cstring, src, strlen(src)-1);

    // opens buffer for writting json
    jwOpen(buffer + strlen(object_name), buflen - strlen(object_name), JW_OBJECT, JW_PRETTY);

    // writes pairs of key, value for json objects
    jwObj_string("ciudad", wi->m_city);
    jwObj_string("fecha", cstring);
    jwObj_double("tmin", wi->m_min);
    jwObj_double("tmax", wi->m_max);
    jwObj_int("prec", wi->m_prec);
    jwObj_int("nub", wi->m_nub);

    err = jwClose();

    sprintf(buffer, "%s\n", buffer);

    return err;
}
