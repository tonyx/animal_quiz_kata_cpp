typedef struct str_list {
    const char *element;
    struct str_list* next;
} Str_list;

void add_element_to_list(Str_list** list, const char* element);
char* head_element_of_list(Str_list** list); 
int has_elements(Str_list** list);
void free_str_list(Str_list** list);  
int list_size(Str_list** list,int *current_counter);
struct str_list* tail_of_list(struct str_list* list);
char * concatenate_strings(int num, ...);
void get_user_input(char* user_input);