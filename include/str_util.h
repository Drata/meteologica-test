//str_util.h
// headers of functions to operate and manipulate strings
#include <string.h>
#include <ctype.h>

/*
 * Function: remove_white_space
 * --------------------
 *  Removes all white spaces from a given string
 *
 *  dest: string to store the string without white spaces
 *  src: original string
 */
void remove_white_space(char * dest, char * src);

/*
 * Function:  point_for_coma
 * --------------------
 *  Changes all the comas of a string for points
 *
 *  dest: string to store the string with points
 *  src: original string
 */
void point_for_coma(char * dest, char * src);

/*
 * Function: to_lower_case
 * --------------------
 *  Converts all characters in a string to lower case
 *
 *  dest: string to store the lower case string
 *  src: original string
 */
void to_lower_case(char * dest, char * src);

/*
 * Function:  point_for_coma
 * --------------------
 *  Removes all accents of a string
 *
 *  dest: string to store the string without accents
 *  src: original string
 */
void remove_accents(char * dest, char * src);

/*
 * Function: remove_dash
 * --------------------
 *  CRemoves all dashes of a string
 *
 *  dest: string to store the string without dashes
 *  src: original string
 */
void remove_dash(char *dest, char *src);
