#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "animal_quiz.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <list>
using namespace std;
#define STDIO_INCLUDED
#define NO_MAIN

const char* THINK_ABOUT_AN_ANIMAL_MESSAGE = "think about an animal";
const char* WELCOME_MESSAGE = "welcome";

bool answer_for_state_is_binary(State state) {
    switch (state) {
        case THINK_ABOUT_AN_ANIMAL_STATE:
            return false;
        case GUESSING_STATE:
            return false;
        case CHECKING_GUESS_IN_LEAF_NODE_STATE:
            return true;
        case CHECKING_GUESS_IN_NON_LEAF_NODE_STATE:
            return true;
        case GETTING_ANIMAL_NAME_STATE:
            return false;
        case GETTING_DISCRIMINATING_QUESTION:
            return false;
        case GETTING_ANSWER_TO_DISCRIMINATING_QUESTION:
            return true;
    }
}

Knowledge_tree_ref_leaf::Knowledge_tree_ref_leaf(char* animal_in) {
    this->animal = concatenate_strings(1,animal_in);
}

Knowledge_tree_ref_non_leaf::Knowledge_tree_ref_non_leaf(char* discriminating_question,Knowledge_tree_ref* yes_branch, Knowledge_tree_ref* no_branch) 
{
    this->discriminating_question = concatenate_strings(1,discriminating_question);
    this->yes_branch=yes_branch;
    this->no_branch=no_branch;
}


// deprecated: 
Knowledge_tree_ref* Knowledge_tree_ref_leaf::rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name) 
{
    if (!(strcmp(answer_to_new_discriminating_question,"no")==0)&&!(strcmp(answer_to_new_discriminating_question,"yes")==0))
    {
        std::cerr << concatenate_strings(2,"the string passed must be 'yes' or 'no' while it's value is ",answer_to_new_discriminating_question) << endl;
        throw std::runtime_error("parmeter error");
    }

    char* old_animal = concatenate_strings(1,this->animal);
    if (strcmp("no",answer_to_new_discriminating_question)==0) {
        delete(this);
        return new Knowledge_tree_ref_non_leaf(new_discriminating_question,new Knowledge_tree_ref_leaf(old_animal),new Knowledge_tree_ref_leaf(new_animal_name));
    } else {
        delete(this);
        return new Knowledge_tree_ref_non_leaf(new_discriminating_question,new Knowledge_tree_ref_leaf(new_animal_name),new Knowledge_tree_ref_leaf(old_animal));
    }
}

void Knowledge_tree_ref::rearrange_knowledge_tree_ref(Str_list* yes_no_list,
        char* new_discriminating_question,
        char* answer_to_new_discriminating_question, char* new_animal_name,Knowledge_tree_ref** outTree) {
            Knowledge_tree_ref_leaf* down_casted_tree;
            if ((down_casted_tree=dynamic_cast<Knowledge_tree_ref_leaf*>(*outTree))!=NULL) {
                char* old_animal = concatenate_strings(1,(*outTree)->get_animal());
                if (strcmp("no",answer_to_new_discriminating_question)==0) {
                    Knowledge_tree_ref_non_leaf* new_node = new Knowledge_tree_ref_non_leaf(new_discriminating_question,new Knowledge_tree_ref_leaf(old_animal),new Knowledge_tree_ref_leaf(new_animal_name));
                    delete(*outTree);
                    *outTree = new_node;
                }
                else if (strcmp("yes",answer_to_new_discriminating_question)==0) {
                    Knowledge_tree_ref_non_leaf* new_node = new Knowledge_tree_ref_non_leaf(new_discriminating_question,new Knowledge_tree_ref_leaf(new_animal_name),new Knowledge_tree_ref_leaf(old_animal));
                    delete(*outTree);
                    *outTree = new_node;
                } else {
                    std::cerr << concatenate_strings(2,"the string passed must be 'yes' or 'no' while it's value is ",answer_to_new_discriminating_question) << endl;
                    throw std::runtime_error("parmeter error");
                }
            } else
            {
                if (strcmp("no",yes_no_list->element)==0) {
                    rearrange_knowledge_tree_ref(yes_no_list->next,new_discriminating_question,answer_to_new_discriminating_question,new_animal_name,(&(*outTree)->no_branch));
                } else if (strcmp("yes",yes_no_list->element)==0) {
                    rearrange_knowledge_tree_ref(yes_no_list->next,new_discriminating_question,answer_to_new_discriminating_question,new_animal_name,(&(*outTree)->yes_branch));
                } else {
                    std::cerr << concatenate_strings(2,"the string passed must be 'yes' or 'no' while it's value is ",answer_to_new_discriminating_question) << endl;
                    throw std::runtime_error("parmeter error");
                }
            }
        }


// deprecated: can't purge the old nodes in the heap memory, so this funciton is replaced by the static rearrange_knowledge_tree_ref
Knowledge_tree_ref* Knowledge_tree_ref_non_leaf::rearrange_knowledge_tree(Str_list* yes_no_list,char* new_discriminating_question,char* answer_to_new_discriminating_question, char* new_animal_name) 
{
    if (!(strcmp(answer_to_new_discriminating_question,"no")==0)&&!(strcmp(answer_to_new_discriminating_question,"yes")==0))
    {
        std::cerr << concatenate_strings(2,"the string passed must be 'yes' or 'no' while it's value is ",answer_to_new_discriminating_question) << endl;
        throw std::runtime_error("parmeter error");
    }

    if (strcmp(yes_no_list->element,"no")==0) {
        return new Knowledge_tree_ref_non_leaf(this->discriminating_question,
        this->yes_branch,
        this->no_branch->rearrange_knowledge_tree(yes_no_list->next,
            new_discriminating_question,answer_to_new_discriminating_question,new_animal_name));

    } else {
        return new Knowledge_tree_ref_non_leaf(this->discriminating_question,
        this->yes_branch->rearrange_knowledge_tree(yes_no_list->next,
            new_discriminating_question,answer_to_new_discriminating_question,new_animal_name),
        this->no_branch);
    }
}


char* Knowledge_tree_ref_leaf::get_question() {
    return concatenate_strings(3,"is it a ",animal,"?");
}

char* Knowledge_tree_ref_non_leaf::get_question() {
    return discriminating_question;
}

char* Knowledge_tree_ref_leaf::get_animal() {
    return animal;
}

char* Knowledge_tree_ref_non_leaf::get_animal() {
    throw std::runtime_error("unable to determine animal name in a non leaf node node");
}

void Knowledge_tree_ref_non_leaf::set_animal(char* animal) {
    throw std::runtime_error("unable to set animal name in a non leaf node node");
}

void Knowledge_tree_ref_leaf::set_animal(char* animal_in) {
    this->animal = concatenate_strings(1,animal_in);
}




State Knowledge_tree_ref_non_leaf::select_specific_checking_guess_state() {
    return CHECKING_GUESS_IN_NON_LEAF_NODE_STATE;
}

State Knowledge_tree_ref_leaf::select_specific_checking_guess_state() {
    return CHECKING_GUESS_IN_LEAF_NODE_STATE;
}

char* Knowledge_tree_ref_non_leaf::get_info() {
    return get_question();
}

char* Knowledge_tree_ref_leaf::get_info() {
    return get_animal();
}

Knowledge_tree_ref::~Knowledge_tree_ref() {
}

Knowledge_tree_ref_leaf::~Knowledge_tree_ref_leaf() {
    free(animal);
}

Knowledge_tree_ref_non_leaf::~Knowledge_tree_ref_non_leaf() {
    free(discriminating_question);
    delete(yes_branch);
    delete(no_branch);
}

Model_ref::Model_ref() {
    answer_to_discrimated_question = concatenate_strings(1,"");
    animal_to_be_learned = concatenate_strings(1,"");
    discriminating_question_for_learning = concatenate_strings(1,WELCOME_MESSAGE);
    state = THINK_ABOUT_AN_ANIMAL_STATE; 
    message_from_engine_ref = concatenate_strings(1,WELCOME_MESSAGE);
    yes_no_list=NULL;
    messages = std::list<string>();
    knowledge_tree_ref = new Knowledge_tree_ref_leaf((char*)"elephant");
    current_node_ref = knowledge_tree_ref;
}

void Model_ref::rewind_n_times(int n) {
    // reinit
    delete(this->knowledge_tree_ref);
    this->knowledge_tree_ref = new Knowledge_tree_ref_leaf((char*)"elephant");
    this->current_node_ref = this->knowledge_tree_ref;
    state = THINK_ABOUT_AN_ANIMAL_STATE; 

    std:list<string> previous_messages; 
    previous_messages = this->messages;
    this->messages.clear();
    free_str_list(&yes_no_list);
    this->yes_no_list=NULL;
    int i=0;
    for (auto it = previous_messages.begin();i<n;it++,i++) {
        update((char*)(*it).c_str());
    }
    previous_messages.clear();
}

void Model_ref::undo() {
    int size = this->messages.size()-1; 
        if (size>2) { 
        this->rewind_n_times(size);
    }
    return;
}

void Model_ref::update(char* user_input) {
    if (strcmp(user_input,"*")==0) {
        undo();
        return;
    } 

    messages.push_back(user_input);
    switch (state) {
        case THINK_ABOUT_AN_ANIMAL_STATE:
            free(message_from_engine_ref);
            message_from_engine_ref = concatenate_strings(1,THINK_ABOUT_AN_ANIMAL_MESSAGE);
            current_node_ref = knowledge_tree_ref;
            state=GUESSING_STATE;
            break;

        case GUESSING_STATE:
            free(message_from_engine_ref);
            // ref:
            message_from_engine_ref = concatenate_strings(1,current_node_ref->get_question());
            state = current_node_ref->select_specific_checking_guess_state();
            break;

        case CHECKING_GUESS_IN_LEAF_NODE_STATE:
            if (strcmp("yes",user_input)==0) {
                free(message_from_engine_ref);
                message_from_engine_ref = concatenate_strings(1,"yeah");
                state=THINK_ABOUT_AN_ANIMAL_STATE;
            } else if (strcmp("no",user_input)==0) {
                free(message_from_engine_ref);
                message_from_engine_ref = concatenate_strings(1,"what animal was?");
                state = GETTING_ANIMAL_NAME_STATE;
            }
            break;

        case CHECKING_GUESS_IN_NON_LEAF_NODE_STATE:
            if (strcmp("yes",user_input)==0) {
                add_element_to_list(&yes_no_list,"yes");
                current_node_ref = current_node_ref->yes_branch;
            } else if (strcmp("no",user_input)==0) {
                add_element_to_list(&yes_no_list,"no");
                current_node_ref = current_node_ref->no_branch;
            } else 
                break;

            free(message_from_engine_ref);
            message_from_engine_ref = concatenate_strings(1,current_node_ref->get_question()); 

            state= current_node_ref->select_specific_checking_guess_state();
            break;

        case GETTING_ANIMAL_NAME_STATE:
            if (strcmp("",user_input)==0) {
                break;
            }
            free(animal_to_be_learned);
            animal_to_be_learned = concatenate_strings(1,user_input);
            free(message_from_engine_ref);
            message_from_engine_ref= concatenate_strings(5,"what is the question to distinguish a ",
                   animal_to_be_learned," from a ",
                   current_node_ref->get_animal(),"?");

            state = GETTING_DISCRIMINATING_QUESTION;
            break;

        case GETTING_ANSWER_TO_DISCRIMINATING_QUESTION:
            if ((strcmp("no",user_input)!=0)&&(strcmp("yes",user_input)!=0)) {
                break;
            }
            // TODO: refactor in a completely mutable way, as the old trees can't be freeed in this way
            // knowledge_tree_ref = knowledge_tree_ref->rearrange_knowledge_tree (yes_no_list,discriminating_question_for_learning,user_input,animal_to_be_learned);

            Knowledge_tree_ref::rearrange_knowledge_tree_ref(yes_no_list,discriminating_question_for_learning,user_input,animal_to_be_learned,&knowledge_tree_ref);


            current_node_ref = knowledge_tree_ref;

            free(message_from_engine_ref);
            message_from_engine_ref=concatenate_strings(1,"let's start again. Think about an animal");
            free_str_list(&yes_no_list);
            yes_no_list=NULL;
            state = GUESSING_STATE;
            break;

        case GETTING_DISCRIMINATING_QUESTION:
            if (user_input==NULL||strcmp("",user_input)==0) {
                break;
            }
            char* discriminating_question_for_learning = concatenate_strings(1,user_input);
            free(this->discriminating_question_for_learning);
            this->discriminating_question_for_learning = discriminating_question_for_learning;
            free(message_from_engine_ref);
            message_from_engine_ref=concatenate_strings(7,"what is the answer to the question '",
                discriminating_question_for_learning,
                "' to distinguish a ",
                animal_to_be_learned,
                " from a ",
                (current_node_ref)->get_animal(),"?"  );

            state = GETTING_ANSWER_TO_DISCRIMINATING_QUESTION;
            break;
    }
}

#ifndef NO_MAIN
int main(int argc, char **argv) {
    char user_input[97];
    Model_ref* model_ref = new Model_ref();

    while(1) {
        std::cout << model_ref->message_from_engine_ref << endl;
        get_user_input(user_input);
        model_ref->update(user_input);
    }
    return 0;
}
#endif 



// spikes: preload a huge tree, only for a test purpose
// void huge_set_model(Model* model) {
//     update_model(model,(char*)"");
//     update_model(model,(char*)"");
//     update_model(model,(char*)"no");
//     update_model(model,(char*)"cat");
//     update_model(model,(char*)"is it big?");
//     update_model(model,(char*)"no"); 

//     update_model(model,(char*)""); 
//     update_model(model,(char*)"no"); // big
//     update_model(model,(char*)"no"); // cat
//     update_model(model,(char*)"mouse"); 
//     update_model(model,(char*)"cheese?"); 
//     update_model(model,(char*)"yes");  

//     update_model(model,(char*)""); 
    
//     // starts again
//     update_model(model,(char*)"yes");  
//     update_model(model,(char*)"no");  
//     update_model(model,(char*)"pterodaptil");  
//     update_model(model,(char*)"estinct?");  

//     update_model(model,(char*)"yes");  
//     update_model(model,(char*)"");  
//     update_model(model,(char*)"yes");  
//     update_model(model,(char*)"no");  
//     update_model(model,(char*)"no");  
//     update_model(model,(char*)"cow");  
//     update_model(model,(char*)"deliver milk?");  
//     update_model(model,(char*)"yes");  
//     update_model(model,(char*)""); 

// }


