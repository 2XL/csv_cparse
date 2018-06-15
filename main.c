#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define CSV_HEADER_ENTRIES 6
#define CSV_MAX_ENTRIES 1000

/*csv head line
    Fecha;
    Ciudad;
    Temperatura Máxima (C);
    Temperatura Mínima (C);
    Precipitación (mm);
    Nubosidad (%)
 */

typedef struct _weather_forecast { // structure name

    // key
    char *key_string; // fecha && ciudad
    char *fecha;
    char *ciudad;

    // values
    float temp_max;
    float temp_min;
    float precipitacion;
    short nubosidad;

} WeatherForecast;

// binary tree structure
typedef struct _tree {
    struct _weather_forecast *data;
    struct _tree *parent;
    struct _tree *left;
    struct _tree *right;
} TreeNode;

typedef int(*COMPARE_LOOKUP)(struct _weather_forecast *, char *);

int compareWeatherString(struct _weather_forecast *wf, char *search_key) {
    return strcmp(wf->key_string, search_key);
}

TreeNode *insertNode(TreeNode **root, COMPARE_LOOKUP compare, struct _weather_forecast *data) {
    /*
     * Returns the root node
     */
    // initialize new node
    TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    // if its bootstrap node
    if (*root == NULL) {
        *root = node;
        return *root;
    }

    int cmp; // comparator variable, for debug...
    while (1) {
        cmp = compare((*root)->data, data->key_string);
        if (cmp == 0) {
            printf("= Already exist!?¿?");
        } else if (cmp > 0) {
            if ((*root)->left != NULL) {
                *root = (*root)->left;
            } else {
                (*root)->left = node;
                node->parent = (*root);
                break;
            }
        } else if (cmp < 0) {
            if ((*root)->right != NULL) {
                *root = (*root)->right;
            } else {
                (*root)->right = node;
                node->parent = (*root);
                break;
            }
        } else {
            printf("ALERT: Found an Unicorn!");
        }
    }

    // TODO: binary tree without self balance (lineal O(N) ... O(log))
    while ((*root)->parent != NULL) *root = (*root)->parent;
    return *root;
}

TreeNode *forecastLookup(TreeNode *root, COMPARE_LOOKUP compare, char *search_key) {
    /*
     *
     */
    if (root != NULL) {
        int cmp = compare(root->data, search_key);
        if (cmp == 0) {
            return root;
        } else if (cmp > 0) {
            if (root->left != NULL)
                return forecastLookup(root->left, compare, search_key);
        } else if (cmp < 0) {
            if (root->right != NULL)
                return forecastLookup(root->right, compare, search_key);
        } else {
            printf("Warning: Unicorn invasion..");
            return NULL;
        }
    }
    return NULL;
}


char *concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);//+1 to copy the null-terminator
    return result;
}

void deallocateWeatherForcast(struct _weather_forecast *wf) {
    free(wf->ciudad);
    free(wf->fecha);
    free(wf->key_string);
}

void displayJSON(struct _weather_forecast *wf, char unit) {
    /*
     */
    if (unit == 'C')
        printf("{ \n\t\"Fecha\": %s, \n\t\"Ciudad\": %s, \n\t\"Temperatura Máxima (C)\": %f, \n\t\"Temperatura Mínima (C)\": %f, \n\t\"Precipitación\": %f, \n\t\"Nubosidad\": %d \n}",
               wf->fecha, wf->ciudad, wf->temp_max, wf->temp_min, wf->precipitacion, wf->nubosidad);
    else { // if not celsius print fahrenheit (default fallback)
        printf("{ \n\t\"Fecha\": %s, \n\t\"Ciudad\": %s, \n\t\"Temperatura Máxima (F)\": %f, \n\t\"Temperatura Mínima (F)\": %f, \n\t\"Precipitación\": %f, \n\t\"Nubosidad\": %d \n}",
               wf->fecha, wf->ciudad, wf->temp_max * 1.8 + 32, wf->temp_min * 1.8 + 32, wf->precipitacion,
               wf->nubosidad);
    }
}




/*functions and actions && helpers
 */
char *replace_char(char *str, char find, char replace);

// normalize csv entries
char *get_next_token(char *buff_start,
                     size_t offset_ini,
                     short token_entry,
                     char **normal_entries) {
    /*
     * buff start :
     * offset_ini :
     * token_entry:
     * normal_entries:
     *
     */
    char *curr_ptr = buff_start; // = *(buff_start + (offset));
    size_t offset_fin = offset_ini;


    while (offset_fin <= strlen(buff_start)) {
        if (curr_ptr[offset_fin] == '\000' || curr_ptr[offset_fin] == ';') {

            // strip trail & initial white space
            size_t offset_trail = offset_fin;
            while (curr_ptr[offset_ini] == ' ' ||
                   curr_ptr[offset_ini] == '\n' ||
                   curr_ptr[offset_ini] == '\r' ||
                   curr_ptr[offset_ini] == '\t') {
                offset_ini++;
            };
            while (curr_ptr[offset_trail - 1] == ' ' ||
                   curr_ptr[offset_trail - 1] == '\n' ||
                   curr_ptr[offset_trail - 1] == '\r' ||
                   curr_ptr[offset_trail - 1] == '\t') {
                offset_trail--;
            };

            normal_entries[token_entry] = realloc(normal_entries[token_entry],
                                                  sizeof(char) * (offset_trail - offset_ini) +
                                                  1); // Jagged Arrays and Pointers
            strncpy(normal_entries[token_entry], buff_start + offset_ini, offset_trail - offset_ini);

            // replace all , with .

            if (token_entry == 2 ||
                token_entry == 3 ||
                token_entry == 4) {

                char *found_comma = strchr(normal_entries[token_entry], ','); // return @ of comma if exist
                if (found_comma != NULL) {
                    *found_comma = '.';
                }
            }


            if (curr_ptr[offset_fin] == '\000') { // exit condition
                break;
            }
            token_entry++;
            offset_fin++;
            return get_next_token(buff_start, offset_fin, token_entry, normal_entries); // continue recursion
        }
        offset_fin++;
    }

    // TODO: Balance the tree of query


    return buff_start; // return the original buff
}


int main(int argc, char **argv) {


    for (int i = 0; i < argc; i++) {
        printf("argv[%d] %s \n", i, argv[i]);
    }


    printf("Testing code");

    /* constants, structs and vars
    */
    const char CSV_SKIP_FIRST_ROW = 1;
    static const char DATASET_PATH_CSV[] = "dataset/dataset.csv";


    // TODO: open csv file, read mode
    FILE *file_stream = fopen(DATASET_PATH_CSV, "r");


    WeatherForecast *dataset[CSV_MAX_ENTRIES];
    TreeNode *tree_dataset[CSV_MAX_ENTRIES];
    TreeNode *tree_root = NULL;
    for (int i = 0; i < CSV_MAX_ENTRIES; i++) {
        dataset[i] = NULL;
        tree_dataset[i] = NULL;
    }

    // TODO: parse csv file
    if (file_stream == NULL) {
        printf("Error reading");
        exit(1); // no content
    } else {

        // allocate memory for dataset
        char line_buffer[255]; //creating char array to store data of file && initialize memory with 0 :D

        if (CSV_SKIP_FIRST_ROW == TRUE) {
            fgets(line_buffer, sizeof(line_buffer), file_stream);
        }


        char *normal_entries[CSV_HEADER_ENTRIES]; // CSV_COL_COUNT pointers

        // allocate memory for normalized tokens
        for (int idx = 0; idx < CSV_HEADER_ENTRIES; idx++) {
            normal_entries[idx] = malloc(sizeof(char));
            if (normal_entries[idx] == NULL) {
                exit(1);
            }
        }

        int forecast_counter = 0;
        while (fgets(line_buffer, sizeof(line_buffer), file_stream)) {

            // normalize row
            get_next_token(line_buffer, 0, 0, normal_entries); // return modifications are stored at normal_entries

            WeatherForecast *ptrWeatherForecast;
            ptrWeatherForecast = (WeatherForecast *) malloc(sizeof(WeatherForecast));
            dataset[forecast_counter] = ptrWeatherForecast;
            ptrWeatherForecast->fecha = (char *) malloc(strlen(normal_entries[0] + 1));
            strcpy(ptrWeatherForecast->fecha, normal_entries[0]);

            ptrWeatherForecast->ciudad = (char *) malloc(strlen(normal_entries[1] + 1));
            strcpy(ptrWeatherForecast->ciudad, normal_entries[1]);

            ptrWeatherForecast->key_string = concat(ptrWeatherForecast->fecha, ptrWeatherForecast->ciudad);

            ptrWeatherForecast->temp_max = atof(normal_entries[2]);
            ptrWeatherForecast->temp_min = atof(normal_entries[3]);
            ptrWeatherForecast->precipitacion = atof(normal_entries[4]);

            // might overflow casting  bigger than short (suppose valid range [0-100])
            ptrWeatherForecast->nubosidad = atoi(normal_entries[5]);

            // insert forcast entry into binary tree && check if forecast entry exist
            insertNode(&tree_root, compareWeatherString, ptrWeatherForecast);
            TreeNode *node = forecastLookup(tree_root, compareWeatherString, ptrWeatherForecast->key_string);
            tree_dataset[forecast_counter] = node;
            // printf("\n\n%d -> [%s]\n", forcast_counter, node->data->key_string);
            forecast_counter++;
        }


        // free memory for normalized tokens
        for (int idx = 0; idx < CSV_HEADER_ENTRIES; idx++) {
            free(normal_entries[idx]);
        }

    }

    fclose(file_stream);

    /***************************************************
     * Interactive testing
     ***************************************************/

    while (1) {
        printf("\n-----------------------------------------------"
               "\nOptions: "
               "\nQuery:  \n\tyyyy/mm/dd\n\tlocation\n\tunit"
               "\nSample:   \n\t2018/06/02\n\tSevilla\n\tCelsius"
               "\nQuit:    \n\texit\n\n...:");

        char location[30];
        char date[20];
        char unit[15];

        printf("\nChoose location [Madrid]: \n");
        scanf("%s", location);
        if (strcmp(location, "exit") == 0) break;

        printf("Choose date [2018/06/06]: \n");
        scanf("%s", date);
        if (strcmp(date, "exit") == 0) break;

        printf("Choose unit [Celsius] or [Fahrenheit]: \n");
        scanf("%s", unit);
        if (strcmp(unit, "exit") == 0) break;

        TreeNode *node = forecastLookup(tree_root, compareWeatherString, concat(date, location));
        if (node == NULL) {
            printf("No result found!!!!!!!!!!!!!!\n");
        } else {
            printf("Found!\n\n");
            if (strcmp(unit, "Celsius") == 0) {
                displayJSON(node->data, 'C');
            } else {
                displayJSON(node->data, 'F');
            }
        }
    };


    /****************************************************
     * FREE MEMORY ALLOCATION
     ****************************************************/
    for (int idx = 0; idx < CSV_MAX_ENTRIES; idx++) {
        if (dataset[idx] != NULL) {
            deallocateWeatherForcast(dataset[idx]);
            free(dataset[idx]);
        }
        if (tree_dataset[idx] != NULL) {
            free(tree_dataset[idx]);
        }
    }

    return 0;
}

