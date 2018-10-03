#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather_info.h"

#define EXIT_SUCCESS 0

int main() {

    weather_info * wi, *wi2;

    wi = create_weather_info(time(NULL), "Madrid", 22.0, 32.0, 0, 12);
    wi2 = create_weather_info(time(NULL), "Barcelona", 20.0, 34.0, 3, 9);

    add_weather(wi);
    add_weather(wi2);

    printf("Se añaden los tiempos.\n");

    print_weather_info(find_weather("Barcelona"));
    print_weather_info(find_weather("Madrid"));

    printf("Se obtiene el tiempo.\n");

    return EXIT_SUCCESS;
}
