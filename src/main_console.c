#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <signal.h>
#include "../include/str_util.h"
#include "../include/csvparser.h"
#include "../include/weather_info.h"

#define EXIT_SUCCESS 0
#define MAX_LENGTH 64

int load_data(char * file) {
    CsvParser *csvparser = CsvParser_new(file, ";", 1);
    CsvRow *row;

    // iterates through all rows of the data file
    while ((row = CsvParser_getRow(csvparser))) {
        // gets fields of current row
        char **rowFields = CsvParser_getFields(row);

        char date[MAX_LENGTH], city[MAX_LENGTH], c_max[MAX_LENGTH],
             c_min[MAX_LENGTH], c_prec[MAX_LENGTH], c_nub[MAX_LENGTH];
        weather_info * wi;
        float max, min, prec;
        int nub;

        // checks if key strings are empty, if they are continue with next iteration
        if(strlen(rowFields[0]) != 0 && strlen(rowFields[0]) != 0) {
            // cleans white spaces from strings
            remove_white_space(date, rowFields[0]);
            remove_white_space(city, rowFields[1]);

            // cleans max tº and checks if its empty
            if (strlen(rowFields[2]) != 0) {
              remove_white_space(c_max, rowFields[2]);
              point_for_coma(c_max, c_max);
              max = atof(c_max);
            } else {
              max = -1.0;
            }

            // cleans min tº and checks if its empty
            if (strlen(rowFields[3]) != 0) {
                remove_white_space(c_min, rowFields[3]);
                point_for_coma(c_min, c_min);
                min = atof(c_min);
            } else {
                min = -1.0;
            }

            // obtains precipitation and checks if its empty
            if (strlen(rowFields[4]) != 0) {
                remove_white_space(c_prec, rowFields[4]);
                point_for_coma(c_prec, c_prec);
                prec = atof(c_prec);
            } else {
                prec = -1.0;
            }

            // obtains nubosity and checks if its empty
            if (strlen(rowFields[5]) != 0) {
                remove_white_space(c_nub, rowFields[5]);
                nub = atoi(c_nub);
            } else {
                nub = -1;
            }

        } else {
            // free memory allocated for row
            CsvParser_destroy_row(row);
            continue;
        }

        // creates weather info for the current row
        wi = create_weather_info(date, city, max, min, prec, nub);

        // adds weather info to the hash table
        add_weather(wi);

        // free memory allocated for row
        CsvParser_destroy_row(row);
    }

    // free memory allocated for parser
    CsvParser_destroy(csvparser);
    return 0;
}

// callback ctrl+c
void intHandler() {
    // frees memory allocated for hash
    destroy_all_weather_info();
}


int main() {

    // signal handler
    signal(SIGINT, intHandler);

    //loads data from csv
    load_data("data/data.csv");

    char city[MAX_LENGTH], date[MAX_LENGTH], key[MAX_LENGTH], unit;
    int i_unit;
    int flag = 0;

    // main loop
    while(strcmp(key, "exit")) {

        if (flag != 0) {
            getchar();
        }

        flag = 1;

        weather_info *wi;

        // asks for query
        printf("\nEnter city: ");
        fgets (city, MAX_LENGTH, stdin);
        printf("\n");

        printf("Enter date (yyyy/mm/dd): ");
        fgets (date, MAX_LENGTH, stdin);
        printf("\n");

        printf("Choose measure unit (Celsius default): \n");
        printf("Fahrenheit - Type f\n");
        printf("Celsius - Type c\n");

        unit = getchar();

        switch(unit) {
            case 'f': i_unit = 1; break;
            case 'c': i_unit = 0; break;
            default: i_unit = 0; break;
        }

        // removes new line
        strtok(city, "\n");
        strtok(date, "\n");

        // creates key
        sprintf(key, "%s%s", city, date);

        // cleans strings
        remove_white_space(key, key);
        remove_accents(key, key);
        to_lower_case(key, key);

        // search weather info in the hash
        wi = find_weather(key);

        // if there's info print json
        if (wi) {
            char buffer[1024];
            weather_info_to_json(buffer, 1024, i_unit, wi);
            printf("%s\n", buffer);
        } else {
            printf("\n\n------------------------------------------\n");
            printf("Weather report not found.\nYour search was:\nCity: %s\nDate: %s\n", city, date);
            printf("\n------------------------------------------\n\n");
        }
    }

    return EXIT_SUCCESS;
}
