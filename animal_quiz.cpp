#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "animal_quiz.h"
#include <iostream>
#include <stdexcept>
using namespace std;
#define STDIO_INCLUDED
#define NO_MAIN

const char* THINK_ABOUT_AN_ANIMAL_MESSAGE = "think about an animal";
const char* WELCOME_MESSAGE = "welcome";

Knowledge_tree::Knowledge_tree(char* animal) {
    this->leaf_or_not_leaf=IS_LEAF;
    this->animal=concatenate_strings(1,animal);
}

Knowledge_tree::Knowledge_tree(char* discriminating_question,Knowledge_tree* yes_branch,Knowledge_tree* no_branch ) {
    this->leaf_or_not_leaf=IS_NON_LEAF;
    this->discriminating_question=concatenate_strings(1,discriminating_question);
    this->yes_branch = yes_branch;
    this->no_branch = no_branch;
}

void Knowledge_tree::rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name) 
{
    if (!(strcmp(answer_to_new_discriminating_question,"no")==0)&&!(strcmp(answer_to_new_discriminating_question,"yes")==0))
    {
        std::cerr << concatenate_strings(2,"the string passed must be 'yes' or 'no' while it's value is ",answer_to_new_discriminating_question) << endl;
        throw std::runtime_error("parmeter error");
    }

    if (!has_elements(&yes_no_list)) {
        this->leaf_or_not_leaf=IS_NON_LEAF;
        this->discriminating_question = concatenate_strings(1,new_discriminating_question);
        if (strcmp("no",answer_to_new_discriminating_question)==0) {
            this->yes_branch = new Knowledge_tree(animal);
            this->no_branch = new Knowledge_tree(new_animal_name);
        } else {
            this->yes_branch = new Knowledge_tree(new_animal_name);
            this->no_branch = new Knowledge_tree(animal);
        }
    } else {
        if (strcmp(yes_no_list->element,"no")==0) {
            this->no_branch->rearrange_knowledge_tree(yes_no_list->next,new_discriminating_question,
                answer_to_new_discriminating_question,new_animal_name);
        } else {
            this->yes_branch->rearrange_knowledge_tree(yes_no_list->next,new_discriminating_question,
                answer_to_new_discriminating_question,new_animal_name);
        }
    }
}


Knowledge_tree::~Knowledge_tree() {
    if (leaf_or_not_leaf==IS_LEAF) {
        free(animal);
    }
    if (leaf_or_not_leaf==IS_NON_LEAF) {
        free(discriminating_question);
        delete(yes_branch);
        delete(no_branch);
    }
}


void update_model(Model* model,char * user_input);


// spike;
void huge_set_model(Model* model); 

#ifndef NO_MAIN
int main(int argc, char **argv) {
    char user_input[97];
    Model * model = get_initial_model();
    while(1) {
        std::cout << model->message_from_engine_ref << endl;
        get_user_input(user_input);

        update_model(model,user_input);
    }
    return 0;
}
#endif 



void update_model(Model* model,char *user_input) {
    switch (model->state) {
        case THINK_ABOUT_AN_ANIMAL_STATE:
            free(model->message_from_engine_ref);
            model->message_from_engine_ref = concatenate_strings(1,THINK_ABOUT_AN_ANIMAL_MESSAGE);
            model->current_node = model->knowledge_tree;
            model->state=GUESSING_STATE;
            break;
        case GUESSING_STATE:
            if (((model->current_node))->leaf_or_not_leaf==IS_LEAF) {
                free(model->message_from_engine_ref);
                model->message_from_engine_ref = concatenate_strings(3,"is it a ",(model->current_node)->animal,"?");
                model->state = CHECKING_GUESS_IN_LEAF_NODE_STATE;
            } else {
                free(model->message_from_engine_ref);
                model->message_from_engine_ref = concatenate_strings(1,model->current_node->discriminating_question);
                model->state = CHECKING_GUESS_IN_NON_LEAF_NODE_STATE;
            }
            break;

        case CHECKING_GUESS_IN_LEAF_NODE_STATE:
            if (strcmp("yes",user_input)==0) {
                free(model->message_from_engine_ref);
                model->message_from_engine_ref = concatenate_strings(1,"yeah");
                model->state=THINK_ABOUT_AN_ANIMAL_STATE;
            } else if (strcmp("no",user_input)==0) {
                model->message_from_engine_ref = concatenate_strings(1,"what animal was?");
                model->state = GETTING_ANIMAL_NAME_STATE;
            }
            break;

        case CHECKING_GUESS_IN_NON_LEAF_NODE_STATE:
            if (strcmp("yes",user_input)==0) {
                model->current_node= (model->current_node)->yes_branch;
                add_element_to_list(&model->yes_no_list,"yes");
            } else if (strcmp("no",user_input)==0) {
                model->current_node= (model->current_node)->no_branch;
                add_element_to_list(&model->yes_no_list,"no");
            } else 
                break;

           if (((model->current_node))->leaf_or_not_leaf==IS_LEAF) {
                free(model->message_from_engine_ref);
                model->message_from_engine_ref = concatenate_strings(3,"is it a ",(model->current_node)->animal,"?");
                model->state = CHECKING_GUESS_IN_LEAF_NODE_STATE;
            } else {
                free(model->message_from_engine_ref);
                model->message_from_engine_ref = concatenate_strings(1,model->current_node->discriminating_question);
                model->state = CHECKING_GUESS_IN_NON_LEAF_NODE_STATE;
            } 
            break;

        case GETTING_ANIMAL_NAME_STATE:
            if (strcmp("",user_input)==0) {
                break;
            }
            free(model->animal_to_be_learned);
            model->animal_to_be_learned = concatenate_strings(1,user_input);
            free(model->message_from_engine_ref);
            model->message_from_engine_ref= concatenate_strings(5,"what is the question to distinguish a ",
                   model->animal_to_be_learned," from a ",
                   (model->current_node)->animal,"?");

            model->state = GETTING_DISCRIMINATING_QUESTION;
            break;


        case GETTING_ANSWER_TO_DISCRIMINATING_QUESTION:
            if ((strcmp("no",user_input)!=0)&&(strcmp("yes",user_input)!=0)) {
                break;
            }
            model->knowledge_tree->rearrange_knowledge_tree (model->yes_no_list,model->discriminating_question_for_learning,user_input,model->animal_to_be_learned);
            model->current_node = model->knowledge_tree;
            free(model->message_from_engine_ref);
            model->message_from_engine_ref=concatenate_strings(1,"let's start again. Think about an animal");
            free_str_list(&model->yes_no_list);
            model->yes_no_list=NULL;
            model->state = GUESSING_STATE;
            break;


        case GETTING_DISCRIMINATING_QUESTION:
            if (user_input==NULL||strcmp("",user_input)==0) {
                break;
            }
            char* discriminating_question_for_learning = concatenate_strings(1,user_input);
            free(model->discriminating_question_for_learning);
            model->discriminating_question_for_learning = discriminating_question_for_learning;
            model->message_from_engine_ref=concatenate_strings(7,"what is the answer to the question '",
                discriminating_question_for_learning,
                "' to distinguish a ",
                model->animal_to_be_learned,
                " from a ",
                (model->current_node)->animal,"?"  );
            model->state = GETTING_ANSWER_TO_DISCRIMINATING_QUESTION;
            break;

    }
}

Model* get_initial_model() {
    Model* to_return = (Model*)malloc(sizeof(Model));
    to_return->answer_to_discrimated_question=concatenate_strings(1,"");
    to_return->animal_to_be_learned= concatenate_strings(1,"");
    to_return->discriminating_question_for_learning = concatenate_strings(1,WELCOME_MESSAGE);
    to_return->state=THINK_ABOUT_AN_ANIMAL_STATE;
    to_return->animal_to_be_learned=concatenate_strings(1,"");
    to_return->message_from_engine_ref=concatenate_strings(1,WELCOME_MESSAGE);
    to_return->knowledge_tree = new Knowledge_tree((char*)"elephant");
    to_return->current_node = to_return->knowledge_tree;
    to_return->yes_no_list=NULL;
    return to_return;
}



// spikes: preload a huge tree, only for a test purpose
void huge_set_model(Model* model) {
    update_model(model,(char*)"");
    update_model(model,(char*)"");
    update_model(model,(char*)"no");
    update_model(model,(char*)"cat");
    update_model(model,(char*)"is it big?");
    update_model(model,(char*)"no"); 

    update_model(model,(char*)""); 
    update_model(model,(char*)"no"); // big
    update_model(model,(char*)"no"); // cat
    update_model(model,(char*)"mouse"); 
    update_model(model,(char*)"cheese?"); 
    update_model(model,(char*)"yes");  

    update_model(model,(char*)""); 
    
    // starts again
    update_model(model,(char*)"yes");  
    update_model(model,(char*)"no");  
    update_model(model,(char*)"pterodaptil");  
    update_model(model,(char*)"estinct?");  

    update_model(model,(char*)"yes");  
    update_model(model,(char*)"");  
    update_model(model,(char*)"yes");  
    update_model(model,(char*)"no");  
    update_model(model,(char*)"no");  
    update_model(model,(char*)"cow");  
    update_model(model,(char*)"deliver milk?");  
    update_model(model,(char*)"yes");  
    update_model(model,(char*)""); 

}


