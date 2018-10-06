#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <signal.h>

#include "../include/str_util.h"
#include "../include/csvparser.h"
#include "../include/weather_info.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "../include/nuklear.h"
#include "../include/nuklear_glfw_gl3.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define MAX_LENGTH 64

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

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

int main(void)
{
    static GLFWwindow *win;
    int width = 0, height = 0, query_success = 0;
    struct nk_context *ctx;
    struct nk_colorf bg;

    // glfw
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }

    // creates window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Reporte de clima", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwGetWindowSize(win, &width, &height);

    // openGL
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    // initialize context
    ctx = nk_glfw3_init(win, NK_GLFW3_INSTALL_CALLBACKS);

    // loads font
    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&atlas);
    nk_glfw3_font_stash_end();}

    //loads data from csv
    load_data("data/data.csv");

    // main loop
    while (!glfwWindowShouldClose(win))
    {
        // handles input
        glfwPollEvents();
        nk_glfw3_new_frame();

        enum {C, F};
        static int op = C;

        // starts GUI
        if (nk_begin(ctx, "Reporte de clima", nk_rect(0, 0, 800, 400),
            NK_WINDOW_TITLE))
        {

            float row_layout[2];
            char json[MAX_ELEMENT_BUFFER];
            char key[MAX_ELEMENT_BUFFER];
            char box_buffer[MAX_ELEMENT_BUFFER];
            int box_len, last_op;

            row_layout[0] = 770 * 0.3;
            row_layout[1] = 770 * 0.7;

            // creates row layout
            nk_layout_row(ctx, NK_STATIC, 350, 2, row_layout);

            // creates group of widgets
            if (nk_group_begin(ctx, "Group_Without_Border", 0)) {

                char buffer[MAX_LENGTH];
                static char city[MAX_LENGTH];
                static char date[MAX_LENGTH];
                static int text_len[2];
                weather_info *wi;

                // creates row layout
                nk_layout_row_static(ctx, 25, 150, 1);

                // widget input for city
                nk_label(ctx, "Ciudad (sin acentos):", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_FIELD, city, &text_len[0], 64, nk_filter_default);

                // widget input for city
                nk_label(ctx, "Fecha (yyyy/mm/dd):", NK_TEXT_LEFT);
                nk_edit_string(ctx, NK_EDIT_FIELD, date, &text_len[1], 64, nk_filter_default);

                city[text_len[0]] = '\0';
                date[text_len[1]] = '\0';

                // widget for selecting measure unit
                if (nk_option_label(ctx, "Celsius", op == C)) op = C;
                if (nk_option_label(ctx, "Fahrenheit", op == F)) op = F;

                // widget button for initiating search
                if (nk_button_label(ctx, "Buscar")) {
                    // saves measure unit selection
                    last_op = op;
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
                        // gets json from weather info
                        weather_info_to_json(json, MAX_ELEMENT_BUFFER, op, wi);

                        // copies json to text box
                        box_len = strlen(json);
                        strcpy(box_buffer, json);
                        query_success = 1;
                    } else {
                        // prints error msg in text boz
                        sprintf(json, "Información del clima no encontrada.\nTu busqueda ha sido: \nCiudad: %s \nFecha: %s", city, date);
                        box_len = strlen(json);
                        strcpy(box_buffer, json);
                        query_success = 0;
                    }
                }

                // widget button for saving json to file
                if (nk_button_label(ctx, "Guardar")) {
                    // if search has succeded
                    printf("Has pulsado guardar: %d", query_success);
                    if(query_success == 1) {
                        // save json to a file
                        char path[MAX_LENGTH];
                        char key_dashless[MAX_LENGTH];

                        // removes dashes from key
                        remove_dash(key_dashless, key);

                        // creates path
                        sprintf(path, "data/%s%d.json", key_dashless, last_op);
                        printf("Saving in %s", path);
                        FILE *fp = fopen(path, "w");
                        if (fp != NULL) {
                            char *save_success =  "La informacion del clima se ha guardado en la carpeta data.";
                            fputs(json, fp);
                            fclose(fp);

                            // prints success msg on text box
                            strcpy(box_buffer, save_success);
                            box_len = strlen(save_success);
                        }
                    }
                }

                nk_group_end(ctx);
            }
            // creates group of widgets
            if (nk_group_begin(ctx, "Group_With_Border", 0)) {
                //Widget text
                nk_layout_row_static(ctx, 310, 510, 1);
                //nk_label_wrap(ctx, json);
                nk_edit_string(ctx, NK_EDIT_BOX, box_buffer, &box_len, 512, nk_filter_default);

                nk_group_end(ctx);
            }
        }
        nk_end(ctx);

        // draw
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }

    // frees memory allocated for hash
    destroy_all_weather_info();

    // terminates glfw context
    nk_glfw3_shutdown();
    glfwTerminate();
    return 0;
}
