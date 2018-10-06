#include "../include/str_util.h"

void remove_white_space(char *dest, char* src) {

    // To keep track of non-space character count
    int count = 0;

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; src[i]; i++) {
        if (src[i] != ' ') {
            dest[count++] = src[i]; // here count is incremented
        }
    }

    // finish the string
    dest[count] = '\0';
}

void point_for_coma(char *dest, char* src) {

    // copies string, if theres a coma copy a point
    for (int i = 0; src[i]; i++) {
        if(src[i] != ',') {
            dest[i] = src[i];
        } else {
            dest[i] = '.';
        }
    }
}

void to_lower_case(char *dest, char* src) {
    for(int i = 0; src[i]; i++){
        dest[i] = tolower(src[i]);
    }
}

void remove_accents(char *dest, char *src) {

    int i = 0, j = 0;

    for(; src[i]; i++, j++) {

        // if there's a vowel with an accent copy the same vowel without it
        switch(src[i]) {
            case -61: j--; break;
            case -95: dest[j] = 'a'; break;
            case -87: dest[j] = 'e'; break;
            case -83: dest[j] = 'i'; break;
            case -77: dest[j] = 'o'; break;
            case -70: dest[j] = 'u'; break;
            default: dest[j] = src[i]; break;
        }
    }

    // finish string
    dest[j] = '\0';
}

void remove_dash(char *dest, char *src) {

    int i = 0, j = 0;

    for(; src[i]; i++, j++) {
        // if there's a vowel with an accent copy the same vowel without it
        switch(src[i]) {
            case '/': j--; break;
            default: dest[j] = src[i]; break;
        }
    }

    // finish string
    dest[j] = '\0';
}
