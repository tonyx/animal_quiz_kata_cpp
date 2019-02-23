#define UNIT_TESTING

#ifndef STDIO_INCLUDED 

#include "minunit.h"
#include "animal_quiz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#include <stdexcept>
using namespace std;
#endif 
#define STDIO_INCLUDED

int tests_run = 0;

static char * test_foo() {
    mu_assert((char*)"one is not one",1==1);
    return 0;
}

static char * test_knowledge_tree_class() {
    Knowledge_tree_ref* knowledge_tree_class = new Knowledge_tree_ref_leaf((char*)"elephant");
    mu_assert((char*)"one is not one",1==1);
    // mu_assert((char*)"shold be leaf",knowledge_tree_class->leaf_or_not_leaf==IS_LEAF);
    mu_assert((char*)"shold containt elephant",(strcmp((char*)"elephant",knowledge_tree_class->animal)==0));
    return 0;
}

static char * when_guess_is_correct_then_the_message_will_be_yeah() {
    Model_ref* model = new Model_ref();
    model->state = GUESSING_STATE;
    model->update((char*)"");
    mu_assert((char*)"next state is not CHECKING_IF_GUESS_IS_CORRECT",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);
    model->update((char*)"yes");
    mu_assert((char*)"yeah",strcmp(model->message_from_engine_ref,(char*)"yeah")==0);
    return 0;
}



static char * when_guess_is_not_correct_then_the_state_will_be_ask_what_animal_was() {
    Model_ref* model = new Model_ref();
    model->state = GUESSING_STATE;
    model->update((char*)"");
    mu_assert((char*)"next state is not CHECKING_IF_GUESS_IS_CORRECT",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);
    model->update((char*)"no");
    mu_assert((char*)"state",model->state == GETTING_ANIMAL_NAME_STATE);
    mu_assert((char*)"yeah",strcmp(model->message_from_engine_ref,(char*)"what animal was?")==0);
    return 0;
}

static char * after_asking_what_animal_was_will_get_ask_the_question_to_distinguish() {
    Model_ref* model = new Model_ref();
    model->state = GETTING_ANIMAL_NAME_STATE;
    model->update((char*)"cat");
    mu_assert((char*)"the animal to be learned should be a cat",strcmp((char*)"cat",model->animal_to_be_learned)==0);
    mu_assert((char*)"the question is not as expected",
        strcmp(model->message_from_engine_ref,(char*)"what is the question to distinguish a cat from a elephant?")==0);
    return 0;
}

static char * after_giving_the_question_to_distinguish_animals_will_as_what_is_the_answer() {
    Model_ref* model = new Model_ref();
    model->state = GETTING_ANIMAL_NAME_STATE;
    model->update((char*)"cat");
    mu_assert((char*)"state",model->state==GETTING_DISCRIMINATING_QUESTION);
    mu_assert((char*)"the animal to be learned should be a cat",strcmp((char*)"cat",model->animal_to_be_learned)==0);
    mu_assert((char*)"the question is not as expected",
        strcmp(model->message_from_engine_ref,(char*)"what is the question to distinguish a cat from a elephant?")==0);
    model->update((char*)"is it big?");
    mu_assert(model->message_from_engine_ref,(strcmp((char*)"what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",model->message_from_engine_ref)==0));
    return 0;
}

static char * after_asking_what_is_the_answer_to_the_discriinating_will_receive_the_answer() {
    Model_ref* model = new Model_ref();

    model->state = GETTING_ANIMAL_NAME_STATE; 
    model->update((char*)"cat");

    mu_assert((char*)"the animal to be learned should be a cat",strcmp((char*)"cat",model->animal_to_be_learned)==0);
    mu_assert((char*)"the question is not as expected",
        strcmp(model->message_from_engine_ref,(char*)"what is the question to distinguish a cat from a elephant?")==0);
    model->update((char*)"is it big?");
    mu_assert(model->message_from_engine_ref,(strcmp((char*)"what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",
        model->message_from_engine_ref)==0));
    mu_assert((char*)"state should be GETTING_ANSWER_TO_DISCRIMINATING_QUESTION",model->state==GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    model->update((char*)"no");
    // mu_assert((char*)"new root",model->knowledge_tree_ref->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert((char*)"new root2",(strcmp(model->knowledge_tree_ref->discriminating_question,(char*)"is it big?")==0));
    mu_assert((char*)"new root3",(strcmp(model->knowledge_tree_ref->yes_branch->animal,(char*)"elephant")==0));

    return 0;
}

static char * will_create_a_new_tree_structure_after_learning() {
    Model_ref* model = new Model_ref();

    model->state = GETTING_ANIMAL_NAME_STATE; 
    model->update((char*)"cat");

    model->animal_to_be_learned = concatenate_strings(1,(char*)"cat");

    mu_assert((char*)"the animal to be learned should be a cat",strcmp((char*)"cat",model->animal_to_be_learned)==0);
    mu_assert((char*)"the question is not as expected",
        strcmp(model->message_from_engine_ref,(char*)"what is the question to distinguish a cat from a elephant?")==0);
    model->update((char*)"is it big?");
    mu_assert(model->message_from_engine_ref,(strcmp((char*)"what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",
        model->message_from_engine_ref)==0));
    mu_assert((char*)"state should be GETTING_ANSWER_TO_DISCRIMINATING_QUESTION",model->state==GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    model->update((char*)"no");
    // mu_assert((char*)"the new type of knowledge tree is non leaf",model->knowledge_tree_ref->leaf_or_not_leaf == IS_NON_LEAF);
    return 0;
}

static char * will_navigate_the_non_leaf_tree_by_questions() {
    Model_ref* model = new Model_ref();
    Knowledge_tree_ref* yes_node= new Knowledge_tree_ref_leaf((char*)"elephant");
    Knowledge_tree_ref* no_node=new Knowledge_tree_ref_leaf((char*)"cat");
    Knowledge_tree_ref* composed_tree = new Knowledge_tree_ref_non_leaf((char*)"is it big?",yes_node,no_node);
    model->knowledge_tree_ref = composed_tree;
    model->current_node_ref = model->knowledge_tree_ref;
    model->update((char*)"");
    mu_assert((char*)"next state is not GUESSING",model->state == GUESSING_STATE);
    model->update((char*)"");
    mu_assert(concatenate_strings(2,(char*)"expecting is it big? but getting ",model->message_from_engine_ref),
        strcmp(model->message_from_engine_ref,(char*)"is it big?")==0);
    mu_assert((char*)"state",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    model->update((char*)"no");
    mu_assert((char*)"animal",strcmp((model->current_node_ref)->animal,(char*)"cat")==0);
    return 0;
}

static char * test_list() {
    Str_list* str_list = NULL;
    int size_calc = 0;
    mu_assert((char*)"test size 0",list_size(&str_list,&size_calc)==0);
    add_element_to_list(&str_list,"ciao");
    mu_assert((char*)"test size 0",list_size(&str_list,&size_calc)==1);
    mu_assert((char*)"intege should be 1",size_calc==1);
    return 0;
}


static char *reproducing_bug() {
    Model_ref* model = new Model_ref();
    model->update((char*)"");
    model->update((char*)"");
    mu_assert((char*)"non elephant",strcmp(model->message_from_engine_ref,(char*)"is it a elephant?")==0);
    model->update((char*)"no");
    model->update((char*)"cat");
    model->update((char*)"is it big?");
    model->update((char*)"no"); 
    // mu_assert((char*)"should be non leaf",model->knowledge_tree_ref->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert((char*)"root shuld be question is it big",(strcmp(model->knowledge_tree_ref->discriminating_question,(char*)"is it big?")==0));
    mu_assert((char*)"yes branch should be elephant",strcmp(model->knowledge_tree_ref->yes_branch->animal,(char*)"elephant")==0);
    mu_assert((char*)"no branch should be cat",strcmp(model->knowledge_tree_ref->no_branch->animal,(char*)"cat")==0);

    // starts again
    model->update((char*)""); 

    model->update((char*)"no"); // big
    model->update((char*)"no"); // cat
    model->update((char*)"mouse"); 
    model->update((char*)"cheese?"); 
    model->update((char*)"yes");  

    // mu_assert((char*)"should be non leaf",model->knowledge_tree_ref->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert((char*)"root shuld be question is it big",(strcmp(model->knowledge_tree_ref->discriminating_question,(char*)"is it big?")==0));
    mu_assert((char*)"yes branch should be elephant",strcmp(model->knowledge_tree_ref->yes_branch->animal,(char*)"elephant")==0);
    // mu_assert((char*)"no branch should be non leaf",(model->knowledge_tree_ref->no_branch->leaf_or_not_leaf == IS_NON_LEAF));
    mu_assert((char*)"no branch question should bshould be cheese",strcmp(model->knowledge_tree_ref->no_branch->discriminating_question,(char*)"cheese?")==0);
    mu_assert((char*)"mouse check",strcmp(model->knowledge_tree_ref->no_branch->yes_branch->animal,(char*)"mouse")==0);
    mu_assert((char*)"cat check",strcmp(model->knowledge_tree_ref->no_branch->no_branch->animal,(char*)"cat")==0);

    model->update((char*)""); 
    
    // starts again
    model->update((char*)"yes");  
    model->update((char*)"no");  
    model->update((char*)"pterodaptil");  
    model->update((char*)"estinct?");  
    model->update((char*)"yes");  

    // mu_assert((char*)"should be non leaf",model->knowledge_tree_ref->leaf_or_not_leaf==IS_NON_LEAF); 
    mu_assert((char*)"root shuld be question is it big",(strcmp(model->knowledge_tree_ref->discriminating_question,(char*)"is it big?")==0));
    // mu_assert((char*)"yes branch should be non leafXXXX",model->knowledge_tree_ref->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert((char*)"question is estinct" ,strcmp(model->knowledge_tree_ref->yes_branch->discriminating_question,(char*)"estinct?")==0);
    // mu_assert((char*)"yes branch of yes branch should be leaf",model->knowledge_tree_ref->yes_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    // mu_assert((char*)"no branch of yes branch should be leaf",model->knowledge_tree_ref->yes_branch->no_branch->leaf_or_not_leaf == IS_LEAF);

    mu_assert((char*)"pterodaptil",strcmp(model->knowledge_tree_ref->yes_branch->yes_branch->animal,(char*)"pterodaptil")==0);
    mu_assert((char*)"elephant",strcmp(model->knowledge_tree_ref->yes_branch->no_branch->animal,(char*)"elephant")==0);

    // starts again
    
    model->update((char*)"");  
    model->update((char*)"yes");  
    model->update((char*)"no");  
    model->update((char*)"no");  
    model->update((char*)"cow");  
    model->update((char*)"deliver milk?");  
    model->update((char*)"yes");  
    model->update((char*)"");  
    char *outmessage = concatenate_strings(2,(char*)"initial state issue ",model->message_from_engine_ref);
    mu_assert(outmessage,strcmp(model->message_from_engine_ref,(char*)"is it big?")==0);
    
    // mu_assert((char*)"should be non leaf",model->knowledge_tree_ref->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert((char*)"root shuld be question is it big",(strcmp(model->knowledge_tree_ref->discriminating_question,(char*)"is it big?")==0));
    // mu_assert((char*)"yes branch should be non leaf",model->knowledge_tree_ref->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert((char*)"question is estinct" ,strcmp(model->knowledge_tree_ref->yes_branch->discriminating_question,(char*)"estinct?")==0);
    // mu_assert((char*)"yes branch of yes branch should be leaf",model->knowledge_tree_ref->yes_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    // mu_assert((char*)"no branch of yes branch should be non leaf",model->knowledge_tree_ref->yes_branch->no_branch->leaf_or_not_leaf == IS_NON_LEAF);
    mu_assert((char*)"deliver milk question",strcmp(model->knowledge_tree_ref->yes_branch->no_branch->discriminating_question,(char*)"deliver milk?")==0);
    // mu_assert((char*)"yes branch of no branch of yes branch should be leaf",model->knowledge_tree_ref->yes_branch->no_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    // mu_assert((char*)"no branch of no branch of yes branch should be leaf",model->knowledge_tree_ref->yes_branch->no_branch->no_branch->leaf_or_not_leaf == IS_LEAF);

    mu_assert((char*)"yes branch of no branch of yes branch should be cow",strcmp(model->knowledge_tree_ref->yes_branch->no_branch->yes_branch->animal,(char*)"cow") == 0);
    mu_assert((char*)"no branch of no branch of yes branch should be elephant",strcmp(model->knowledge_tree_ref->yes_branch->no_branch->no_branch->animal,(char*)"elephant") == 0);

    model->update((char*)"");  
    mu_assert((char*)"ask if it is big",strcmp(model->message_from_engine_ref,(char*)"is it big?")==0);
    model->update((char*)"yes");  
    mu_assert((char*)"ask if it is estinct",strcmp(model->message_from_engine_ref,(char*)"estinct?")==0);
    model->update((char*)"no");  
    mu_assert((char*)"ask if it deliver milks",strcmp(model->message_from_engine_ref,(char*)"deliver milk?")==0);
    model->update((char*)"yes");  
    mu_assert((char*)"ask if it is an elephant",strcmp(model->message_from_engine_ref,(char*)"is it a cow?")==0);
    model->update((char*)"no");  

    return 0;
}


static char * stdLibListTest() {
    std::list<char*> myList;
    
    myList.clear();
    myList.push_back((char*)"pipo");
    mu_assert((char*)"list size",myList.size()==1);
    myList.push_back((char*)"pipoX");
    mu_assert((char*)"list size",myList.size()==2);
    myList.clear();
    mu_assert((char*)"list size",myList.size()==0);

    std::string myString = std::string("ciao");
    std::string secondString = std::string("ciao");
    mu_assert((char*)"strings",myString==secondString);

    Model_ref* model_ref = new Model_ref();
    // model_ref->yes_no_list.push_back("hello");

    // mu_assert((char*)"due",model_ref->yes_no_list.size()==1);

    return 0;
}




static char * all_tests() {
    mu_run_test(test_foo);
    mu_run_test(test_knowledge_tree_class);
    mu_run_test(when_guess_is_correct_then_the_message_will_be_yeah);
    mu_run_test(when_guess_is_not_correct_then_the_state_will_be_ask_what_animal_was);
    mu_run_test(after_asking_what_animal_was_will_get_ask_the_question_to_distinguish);
    mu_run_test(after_giving_the_question_to_distinguish_animals_will_as_what_is_the_answer);
    mu_run_test(after_asking_what_is_the_answer_to_the_discriinating_will_receive_the_answer);
    mu_run_test(will_create_a_new_tree_structure_after_learning);
    mu_run_test(will_navigate_the_non_leaf_tree_by_questions);
    mu_run_test(reproducing_bug);
    mu_run_test(stdLibListTest);
    // mu_run_test(test_list);
    return 0;
}

int main() {
     char *result = all_tests();
     if (result != 0) {
         printf((char*)"%s\n", result);
     }
     else {
         printf((char*)"ALL TESTS PASSED\n");
     }
     printf((char*)"Tests run: %d\n", tests_run);
 
     return result != 0;
}
