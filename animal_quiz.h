#include "myutils.h"

#define IS_LEAF 1
#define IS_NON_LEAF 0


class Knowledge_tree_ref {
    public:
    int leaf_or_not_leaf;
    char * animal;
    char * discriminating_question;
    Knowledge_tree_ref* yes_branch;
    Knowledge_tree_ref* no_branch;
    // virtual ~Knowledge_tree_ref() = 0;
    virtual Knowledge_tree_ref* rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name) =0;
};


class Knowledge_tree_ref_non_leaf: public Knowledge_tree_ref {
    public:
    Knowledge_tree_ref* rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name);
    Knowledge_tree_ref_non_leaf(char* discriminating_question,Knowledge_tree_ref* yes_branch, Knowledge_tree_ref* no_branch);
};


class Knowledge_tree_ref_leaf: public Knowledge_tree_ref {
    public:
    Knowledge_tree_ref* rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name);
    Knowledge_tree_ref_leaf(char* animal);
    // ~Knowledge_tree_ref_leaf();
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
    Knowledge_tree_ref* knowledge_tree_ref;
    Knowledge_tree_ref* current_node_ref;
    Str_list* messages_list;

} Model;

Model* get_initial_model();
void update_model(Model* model,char * user_input);