#include "myutils.h"
#include <list>

#define IS_LEAF 1
#define IS_NON_LEAF 0

typedef enum {THINK_ABOUT_AN_ANIMAL_STATE,
    GUESSING_STATE,
    CHECKING_GUESS_IN_LEAF_NODE_STATE,
    CHECKING_GUESS_IN_NON_LEAF_NODE_STATE,
    GETTING_ANIMAL_NAME_STATE,
    GETTING_DISCRIMINATING_QUESTION,
    GETTING_ANSWER_TO_DISCRIMINATING_QUESTION,
    } 
State;

bool answer_for_state_is_binary(State state); 

using Tree_viewer = void();

using Command_function = void(*)(char*);

class Command_item {
    public:
    Command_function command;
    char argument_buffer[97];
    Command_item(Command_function command, char* buffer_in);
};

class Knowledge_tree_ref {
    public:
    Knowledge_tree_ref* yes_branch;
    Knowledge_tree_ref* no_branch;
    static void set_tree_viewer(Tree_viewer* fun);
    virtual char* get_info() = 0;
    virtual char* get_animal() = 0;
    virtual void set_animal(char* animal) =0;
    virtual State select_specific_checking_guess_state() = 0;
    virtual char* get_question() = 0;
    virtual ~Knowledge_tree_ref() = 0;

    void set_buffer(char* pre_buffer, char* buffer_in);
    char* get_buffer();

    // deprecated:
    virtual Knowledge_tree_ref* rearrange_knowledge_tree(Str_list* yes_no_list,
        char* new_discriminating_question,
        char* answer_to_new_discriminating_question, char* new_animal_name) =0;
    
    static void rearrange_knowledge_tree_ref(Str_list* yes_no_list,
        char* new_discriminating_question,
        char* answer_to_new_discriminating_question, char* new_animal_name,Knowledge_tree_ref** outTree);

    protected: 
    static Tree_viewer* tree_viewer;
    char buffer[97];
};

class Knowledge_tree_ref_non_leaf: public Knowledge_tree_ref {
    public:

    // deprecated:
    Knowledge_tree_ref* rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name);

    char* get_question();
    char* get_animal();
    void set_animal(char* animal);
    char* get_info();
    State select_specific_checking_guess_state(); 
    Knowledge_tree_ref_non_leaf(char* discriminating_question,Knowledge_tree_ref* yes_branch, Knowledge_tree_ref* no_branch);
    ~Knowledge_tree_ref_non_leaf();
    private:
    char* discriminating_question;
};

class Knowledge_tree_ref_leaf: public Knowledge_tree_ref {
    public:

    // deprecated:
    Knowledge_tree_ref* rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name);

    char* get_animal();
    void set_animal(char* animal);
    char* get_question();
    char* get_info();
    State select_specific_checking_guess_state(); 
    Knowledge_tree_ref_leaf(char* animal);
    ~Knowledge_tree_ref_leaf();
    private: 
    char* animal;
};

class Model_ref {
    public:
    char* get_message_from_engine();
    void set_message_from_engine(int num, ...);
    void set_message_from_engine(char* message);
    State state;
    char* answer_to_discrimated_question;
    char* discriminating_question_for_learning;
    Str_list* yes_no_list;
    std::list<std::string> yes_no_list_ref;
    std::list<std::string> messages;

    std::list<Command_item*> command_list;

    Knowledge_tree_ref* knowledge_tree_ref;
    Knowledge_tree_ref* current_node_ref;
    void update(char* user_input);
    void undo();
    Model_ref();
    void rewind_n_times(int n);

    private:
    char message_from_engine_buffer_version[97];
    char* animal_to_be_learned;
};
