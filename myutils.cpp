#ifndef STDIO_INCLUDED 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "myutils.h"
#include <iostream>
#include <stdexcept>
using namespace std;
#endif 
#define STDIO_INCLUDED

void add_element_to_list(Str_list** list, const  char* element) {
    if (*list==NULL) {
        *list = (Str_list*) malloc(sizeof(Str_list));
        (*list)->element = element; 
        (*list)->next = NULL;
    } 
    else {
        add_element_to_list(&(*list)->next,element);
    }
}

char* head_element_of_list(Str_list** list) {
    if (*list==NULL) {
        return NULL;
    } else {
        return (char*)(*list)->element;
    }
}

int has_elements(Str_list** list) {
    return (*list!=NULL);
}

void free_str_list(Str_list** list) {
    if ((*list)==NULL) {
        return;
    } else {
        free_str_list(&(*list)->next);
        free(*list);
    }
}

int list_size(Str_list** list,int *current_counter) {
    if ((*list)==NULL) {
        return *current_counter;
    } else
    {
        (*current_counter)++;
        return list_size(&(*list)->next,current_counter);
    }
}

struct str_list* tail_of_list(struct str_list* list) {
    return list->next;
}

char * concatenate_strings(int num, ...) {
    char* to_return;
    va_list valist;
    int total_size=0;
    va_start(valist,num);
    
    for (int i=0;i<num;i++) {
        total_size+=strlen(va_arg(valist,char*));
    }
    va_end(valist);
    total_size+=20;
    to_return = (char*) malloc(total_size);
    strcpy(to_return,"");

    va_start(valist,num);
    for (int i=0;i<num;i++) {
        strcat(to_return,(va_arg(valist,char*)));
    }
    return to_return;
}


void multistrcpy(char* out_buffer, int num, ...) {
    strcpy(out_buffer,"");
    va_list valist;

    va_start(valist,num);
    for (int i = 0; i<num; i++) {
        strcat(out_buffer,(va_arg(valist,char*)));
    }
    va_end(valist);
}



void get_user_input(char* user_input) {
    fgets(user_input,97,stdin);
    if (user_input==NULL||strlen(user_input)==0) {
        user_input=concatenate_strings(1,"");
    } else {
        char * eof = strchr(user_input,'\n');
        if (eof!=0) {
            *eof = '\0'; 
        }
    }
}
