#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/csvparser.h"
#include "../include/weather_info.h"

#define EXIT_SUCCESS 0

int load_data(char * file) {
    int i = 0;

    CsvParser *csvparser = CsvParser_new(file, ";", 1);
    CsvRow *header;
    CsvRow *row;

    header = CsvParser_getHeader(csvparser);

    if (header == NULL) {
        return -1;
    }

    char **headerFields = CsvParser_getFields(header);

    for(i = 0; i < CsvParser_getNumFields(header); i++) {
        printf("Title: %s\n", headerFields[i]);
    }

    while ((row = CsvParser_getRow(csvparser))) {
        printf("NEW LINE:\n");
        char **rowFields = CsvParser_getFields(row);
        for(i = 0; i < CsvParser_getNumFields(row) ; i++) {
            printf("FIELD: %s\n", rowFields[i]);
        }
        CsvParser_destroy_row(row);
    }
    CsvParser_destroy(csvparser);
    return 0;
}

int main() {

    weather_info * wi, *wi2, *wi3;

    wi = create_weather_info(time(NULL), "Madrid", 22.0, 32.0, 0, 12);
    wi2 = create_weather_info(time(NULL), "Barcelona", 20.0, 34.0, 3, 9);

    add_weather(wi);
    add_weather(wi2);

    printf("Se añaden los tiempos.\n");

    wi3 = find_weather("Barcelona");

    char buffer[1024];

    weather_info_to_json(buffer, 1024, wi3);

    printf("%s", buffer);

    print_weather_info(wi3);

    load_data("data/data.csv");

    return EXIT_SUCCESS;
}
