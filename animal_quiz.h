#include "myutils.h"

#define IS_LEAF 1
#define IS_NON_LEAF 0

class Knowledge_tree {
    public:
    int leaf_or_not_leaf;
    char * animal;
    char * discriminating_question;
    Knowledge_tree * yes_branch;
    Knowledge_tree * no_branch;
    Knowledge_tree(char* animal);
    Knowledge_tree(char* discriminating_question,Knowledge_tree* yes_branch, Knowledge_tree* no_branch);
    void rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name);

    ~Knowledge_tree();
};

typedef enum {THINK_ABOUT_AN_ANIMAL_STATE,
    GUESSING_STATE,
    CHECKING_GUESS_IN_LEAF_NODE_STATE,
    CHECKING_GUESS_IN_NON_LEAF_NODE_STATE,
    GETTING_ANIMAL_NAME_STATE,
    GETTING_DISCRIMINATING_QUESTION,
    GETTING_ANSWER_TO_DISCRIMINATING_QUESTION,
    } State;


typedef struct Model {
    char* animal_to_be_learned;
    char* answer_to_discrimated_question;
    char* discriminating_question_for_learning;
    State state;
    Str_list* yes_no_list;
    char* message_from_engine_ref;
    Knowledge_tree* knowledge_tree;
    Knowledge_tree* current_node;
} Model;

Model* get_initial_model();
void update_model(Model* model,char * user_input);