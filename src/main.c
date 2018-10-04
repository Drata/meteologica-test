#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/csvparser.h"
#include "../include/weather_info.h"

#define EXIT_SUCCESS 0

char * remove_white_space(char *str) {

    // To keep track of non-space character count
    int count = 0;

    // Allocates memory for new string
    char *str_blank = (char*) malloc (sizeof(str));

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ') {
            str_blank[count++] = str[i]; // here count is incremented
        }
    }

    // finish the string
    str_blank[count] = '\0';

    return str_blank;
}

char * point_for_coma(char *str) {

    // allocates memory for new string
    char *res = (char*) malloc (strlen(str) * sizeof(char));

    // copies string, if theres a coma copy a point
    for (int i = 0; str[i]; i++) {
        if(str[i] != ',') {
            res[i] = str[i];
        } else {
            res[i] = '.';
        }
    }

    return res;
}

int load_data(char * file) {
    CsvParser *csvparser = CsvParser_new(file, ";", 1);
    CsvRow *row;

    // iterates through all rows of the data file
    while ((row = CsvParser_getRow(csvparser))) {
        // gets fields of current row
        char **rowFields = CsvParser_getFields(row);

        char *date, *city;
        weather_info * wi;
        float max, min, prec;
        int nub;

        // checks if key strings are empty, if they are continue with next iteration
        if(strlen(rowFields[0]) != 0 && strlen(rowFields[0]) != 0) {
            // obtains date and city
            date = remove_white_space(rowFields[0]);
            city = remove_white_space(rowFields[1]);

            // obtains max tº and checks if its empty
            if (strlen(rowFields[2]) != 0) {
              max = atof(point_for_coma(rowFields[2]));
            } else {
              max = -1.0;
            }

            // obtains min tº and checks if its empty
            if (strlen(rowFields[3]) != 0) {
                min = atof(point_for_coma(rowFields[3]));
            } else {
                min = -1.0;
            }

            // obtains precipitation and checks if its empty
            if (strlen(rowFields[4]) != 0) {
                prec = atof(point_for_coma(rowFields[4]));
            } else {
                prec = -1.0;
            }

            // obtains nubosity and checks if its empty
            if (strlen(rowFields[5]) != 0) {
                nub = atof(point_for_coma(rowFields[5]));
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

int main() {

    load_data("data/data.csv");

    char key[100];

    while(strcmp(key, "exit")) {

        weather_info *wi;

        printf("Enter key: ");
        fgets (key, 100, stdin);
        printf("\n");

        strtok(key, "\n");

        wi = find_weather(key);

        if (wi) {
            char buffer[1024];
            weather_info_to_json(buffer, 1024, 1, wi);
            printf("%s\n", buffer);
        } else {
            printf("Weather report not found.\n");
        }
    }

    destroy_all_weather_info();

    return EXIT_SUCCESS;
}
