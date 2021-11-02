#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/ioctl.h>


typedef struct str_array_s {
    int size;
    char** array;
} str_array;



int* xtrct_params(int argc, char** argv);
str_array* xtrct_paths(int argc, char** argv);
int check_d_f_exists(char* path);
int my_strlen(char* text);
long int get_date(char* path);
void print_ls(str_array* fnames);
void    cp_str(char* dest, char* src);
str_array* get_fnames_fr_dir(char* path);
void sort_struct (str_array* fnames, int a, int t);
void my_ls(char* path, int a, int t);