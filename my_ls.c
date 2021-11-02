#include "my_ls.h"

/*
get params and path
from path get files list in set
param a effects how it is printed (with dot b4 or not)
param t effects print sequence (sort by date), by default sort by name

1- get params


1.5 - fix params in my_ls

2 - get path
3 - from path xtract all filenames
4 - sort filenames by name or time depending on param t

5 - get screen size and depending on it print on screen
5 - print to screen
*/

// OK
int* xtrct_params(int argc, char** argv) {
    int* params = malloc(sizeof(int) * 2);
    params[0] = 0;
    params[1] = 0;
    int i = 1;
    int k = 0;
    while (i < argc && argv[i][0] == '-') {
        k = 1;
        while (argv[i][k]){
            if (argv[i][k] == 'a')
                params[0] = 1;
            else if (argv[i][k] == 't')
                params[1] = 1;
            k++;
        }
        i++;
    }
    return (params);
}

// OK
str_array* xtrct_paths(int argc, char** argv) {
    char** paths = malloc(sizeof(char*) * 30);
    int i = 1;
    int j = 0;
    while (i < argc && argv[i][0] == '-')
        i++;
    while (i < argc) {
        paths[j] = argv[i];
        j++;
        i++;
    }
    str_array* rslt = malloc(sizeof(str_array));
    rslt->size = j;
    rslt->array = paths;
    return (rslt);
}

//OK
int check_d_f_exists(char* path) {
    struct stat ls_data;
    if (lstat(path, &ls_data) == -1)
        return (0);
    return (1);
}

//OK
int my_strlen(char* text) {
    int i = 0;
    while (text[i])
        i++;
    return(i);
}

//OK
long int get_date(char* path) {
    struct stat sb;
    lstat(path, &sb);
    return (sb.st_mtime);
}

// fix when filename is longer than 16 chars
void print_ls(str_array* fnames) {
    // get screensize to know how many files to print
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    // int files_in_line = w.ws_col / 16;
    int files_in_line = 5;


    int i = 0;
    int j;
    int k = 0;
    int rows = fnames->size / files_in_line;
    if (fnames->size % files_in_line > 0)
        rows++;
    while (i < rows) {
        j = 0;
        while (j < files_in_line) {
            if (k < fnames->size) {
                printf("%s", fnames->array[k]);
                if (my_strlen(fnames->array[k]) < 8)
                    printf("\t\t");
                else
                    printf("\t");
            }
            k += rows;
            j++;
        }
        printf("\n");
        i++;
        k = i;
    }
}

void    cp_str(char* dest, char* src) {
    int i = 0;

    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = src[i];
}

// OK
str_array* get_fnames_fr_dir(char* path) {
    DIR* dirp;
    struct dirent *dp;
    
    int size = 0;

    dirp = opendir(path);
    while((dp = readdir(dirp)) != NULL)
        size++;
    closedir(dirp);

    char** array = malloc(sizeof(char*) * size);

    int i = 0;
    dirp = opendir(path);
    while((dp = readdir(dirp)) != NULL) {
        array[i] = (char*) malloc(sizeof(char) * (my_strlen(dp->d_name) + 1));
        cp_str(array[i], dp->d_name);
        i++;
    }
    closedir(dirp);

    str_array* fnames = malloc(sizeof(str_array));
    fnames->size = size;
    fnames->array = array;

    return(fnames);
}

// TODO fix part where i drop . and .. ##OK
void sort_struct (str_array* fnames, int a, int t) {
    int i = 0;
    int j;
    int swapped = 1;
    char* temp;

    if (!a) {
        int size = 0;
        while (i < fnames->size) {
            if (fnames->array[i][0] != '.')
                size++;
            i++;
        }
        char** array = malloc(sizeof(char*) * size);
        i = 0;
        j = 0;
        while (i < fnames->size) {
            if (fnames->array[i][0] != '.') {
                array[j] = malloc(sizeof(char) * (my_strlen(fnames->array[i])+1));
                cp_str(array[j], fnames->array[i]);
                j++;
            }
            i++;
        }
        i = 0;
        while (i < fnames->size) {
            free(fnames->array[i]);
            i++;
        }
        free(fnames->array);
        fnames->array = array;
        fnames->size = size;
    }

    if (!t) {
        while (swapped) {
            swapped = 0;
            i = 1;
            while (i < fnames->size) {
                j = 0;
                while (fnames->array[i][j] && fnames->array[i-1][j] && fnames->array[i][j] == fnames->array[i-1][j])
                    j++;
                if (fnames->array[i][j] < fnames->array[i-1][j]) {
                    temp = fnames->array[i];
                    fnames->array[i] = fnames->array[i-1];
                    fnames->array[i-1] =temp;
                    swapped = 1;
                }
                i++;
            }
        }
    }

    else {
        swapped = 1;
        while (swapped) {
            swapped = 0;
            i = 1;
            while (i < fnames->size) {
                if (get_date(fnames->array[i]) > get_date(fnames->array[i-1])) {
                    temp = fnames->array[i];
                    fnames->array[i] = fnames->array[i-1];
                    fnames->array[i-1] =temp;
                    swapped = 1;
                }
                else if (get_date(fnames->array[i]) == get_date(fnames->array[i-1])) {
                    j = 0;
                    while (fnames->array[i][j] && fnames->array[i-1][j] && fnames->array[i][j] == fnames->array[i-1][j])
                        j++;
                    if (fnames->array[i][j] < fnames->array[i-1][j]) {
                        temp = fnames->array[i];
                        fnames->array[i] = fnames->array[i-1];
                        fnames->array[i-1] =temp;
                        swapped = 1;
                    }
                }
                i++;
            }
        }
    }
}

// OK
void my_ls(char* path, int a, int t) {
    if (!check_d_f_exists(path)) {
        printf("my_ls: %s: No such file or directory\n", path);
        return;
    }

    str_array* fnames = get_fnames_fr_dir(path);
    sort_struct(fnames, a, t);

    print_ls(fnames);

    int i = 0;
    while (i < fnames->size) {
        free(fnames->array[i]);
        i++;
    }
    free(fnames->array);
    free(fnames);
}



int main(int argc, char** argv) {
    if (argc == 1)
        my_ls(".", 0, 0);

    else {
        int* params = xtrct_params(argc, argv);
        int a = params[0];
        int t = params[1];
        free(params);

        str_array* paths = xtrct_paths(argc, argv);
        int i = 0;
        if (paths->size == 0)
            my_ls(".", a, t);
        else if (paths->size == 1)
            my_ls(paths->array[0], a, t);
        else {
            while (i < paths->size) {
                printf("%s:\n", paths->array[i]);
                my_ls(paths->array[i], a, t);
                printf("\n");
                i++;
            }
        }
        free(paths->array);
        free(paths);
    }

    return (0);
}