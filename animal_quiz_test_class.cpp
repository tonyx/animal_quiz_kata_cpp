#define UNIT_TESTING

#include "minunit.h"
#include "animal_quiz.cpp"

#ifndef STDIO_INCLUDED 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "myutils.c"
#endif 
#define STDIO_INCLUDED

int tests_run = 0;

static char * test_foo() {
    mu_assert((char*)"one is not one",1==1);
    return 0;
}

static char * test_knowledge_tree_class() {
    Knowledge_tree_class* knowledge_tree_class = new Knowledge_tree_class((char*)"elephant");
    mu_assert((char*)"one is not one",1==1);
    mu_assert((char*)"shold be leaf",knowledge_tree_class->leaf_or_not_leaf==IS_LEAF);
    mu_assert((char*)"shold containt elephant",(strcmp((char*)"elephant",knowledge_tree_class->animal)==0));
    return 0;
}

static char * when_guess_is_correct_then_the_message_will_be_yeah() {
    Model* model = get_initial_model();

    model->state = GUESSING_STATE;
    update_model(model,"");
    mu_assert("next state is not CHECKING_IF_GUESS_IS_CORRECT",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);
    update_model(model,"yes");
    mu_assert("yeah",strcmp(model->message_from_engine_ref,"yeah")==0);
    return 0;
}


static char * test_add_element_to_list() {
    Str_list* list; 
    list = NULL;
    add_element_to_list(&list,"hello");
    mu_assert("asdf",strcmp("hello",list->element)==0);
    add_element_to_list(&list,"hellohello");
    mu_assert("asdfasfd",strcmp("hellohello",list->next->element)==0);
    char* exphead = head_element_of_list(&list);
    mu_assert("head",strcmp(exphead,"hello")==0);

    Str_list* tail = tail_of_list(list);
    mu_assert("tail",strcmp(tail->element,"hellohello")==0);

    return 0;
}


static char * when_guess_is_not_correct_then_the_state_will_be_ask_what_animal_was() {
    Model* model = get_initial_model();

    model->state = GUESSING_STATE;
    update_model(model,"");
    mu_assert("next state is not CHECKING_IF_GUESS_IS_CORRECT",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);
    update_model(model,"no");
    mu_assert("state",model->state == GETTING_ANIMAL_NAME_STATE);
    mu_assert("yeah",strcmp(model->message_from_engine_ref,"what animal was?")==0);
    return 0;
}

static char * after_asking_what_animal_was_will_get_ask_the_question_to_distinguish() {
    Model* model = get_initial_model();
    model->state = GETTING_ANIMAL_NAME_STATE;
    update_model(model,"cat");
    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine_ref,"what is the question to distinguish a cat from a elephant?")==0);
    return 0;
}

static char * after_giving_the_question_to_distinguish_animals_will_as_what_is_the_answer() {
    Model* model = get_initial_model();
    model->state = GETTING_ANIMAL_NAME_STATE;
    update_model(model,"cat");
    mu_assert("state",model->state==GETTING_DISCRIMINATING_QUESTION);
    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine_ref,"what is the question to distinguish a cat from a elephant?")==0);
    update_model(model,"is it big?");
    mu_assert(model->message_from_engine_ref,(strcmp("what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",model->message_from_engine_ref)==0));
    return 0;
}

static char * after_asking_what_is_the_answer_to_the_discriinating_will_receive_the_answer() {
    Model* model = get_initial_model();

    model->state = GETTING_ANIMAL_NAME_STATE; 
    update_model(model,"cat");

    model->animal_to_be_learned = concatenate_strings(1,"cat");

    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine_ref,"what is the question to distinguish a cat from a elephant?")==0);
    update_model(model,"is it big?");
    mu_assert(model->message_from_engine_ref,(strcmp("what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",
        model->message_from_engine_ref)==0));
    mu_assert("state should be GETTING_ANSWER_TO_DISCRIMINATING_QUESTION",model->state==GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    update_model(model,"no");
    mu_assert("new root",model->knowledge_tree_class->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("new root2",(strcmp(model->knowledge_tree_class->discriminating_question,"is it big?")==0));
    mu_assert("new root3",(strcmp(model->knowledge_tree_class->yes_branch->animal,"elephant")==0));

    return 0;
}

static char * will_create_a_new_tree_structure_after_learning() {
    Model* model = get_initial_model();

    model->state = GETTING_ANIMAL_NAME_STATE; 
    update_model(model,"cat");

    model->animal_to_be_learned = concatenate_strings(1,"cat");

    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine_ref,"what is the question to distinguish a cat from a elephant?")==0);
    update_model(model,"is it big?");
    mu_assert(model->message_from_engine_ref,(strcmp("what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",
        model->message_from_engine_ref)==0));
    mu_assert("state should be GETTING_ANSWER_TO_DISCRIMINATING_QUESTION",model->state==GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    update_model(model,"no");
    mu_assert("the new type of knowledge tree is non leaf",model->knowledge_tree_class->leaf_or_not_leaf == IS_NON_LEAF);
    return 0;
}

static char * will_navigate_the_non_leaf_tree_by_questions() {
    Model* model = get_initial_model();
    Knowledge_tree_class* yes_node= new Knowledge_tree_class("elephant");
    Knowledge_tree_class* no_node=new Knowledge_tree_class("cat");
    Knowledge_tree_class* composed_tree = new Knowledge_tree_class("is it big?",yes_node,no_node);
    model->knowledge_tree_class = composed_tree;
    model->current_node_class = composed_tree;
    update_model(model,"");
    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine_ref),
        strcmp(model->message_from_engine_ref,"is it big?")==0);
    mu_assert("state",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"no");
    mu_assert("animal",strcmp((model->current_node_class)->animal,"cat")==0);
    return 0;
}

static char * will_navigate_the_non_leaf_tree_by_questions_2() {
    Model* model = get_initial_model();
    Knowledge_tree_class* yes_node=new Knowledge_tree_class("elephant");
    Knowledge_tree_class* no_node=new Knowledge_tree_class("cat");
    Knowledge_tree_class* composed_tree = new Knowledge_tree_class("is it big?",yes_node,no_node);
    model->knowledge_tree_class = composed_tree;
    model->current_node_class = composed_tree;
    update_model(model,"");
    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine_ref),
        strcmp(model->message_from_engine_ref,"is it big?")==0);
    mu_assert("state",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"yes");
    mu_assert("animal",strcmp(((model->current_node_class))->animal,"elephant")==0);
    return 0;
}

static char * leaning_on_a_non_leaf_tree() {
    Model* model = get_initial_model();
    model->knowledge_tree_class = new Knowledge_tree_class("is it big?",new Knowledge_tree_class("elephant"),new Knowledge_tree_class("cat"));
    update_model(model,"");


    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine_ref),
        strcmp(model->message_from_engine_ref,"is it big?")==0);
    mu_assert("stateX",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"no");
    mu_assert("animal",strcmp(((model->current_node_class))->animal,"cat")==0);
    

    mu_assert("stateZ",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);

    mu_assert("engine",strcmp(model->message_from_engine_ref,"is it a cat?")==0);
    update_model(model,"no");
    mu_assert("getting animal",model->state == GETTING_ANIMAL_NAME_STATE);
    update_model(model,"mouse");
    mu_assert("getting animal",model->state == GETTING_DISCRIMINATING_QUESTION);
    update_model(model,"does it like cheese?");
    mu_assert("getting answer",model->state == GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    mu_assert("message",strncmp(model->message_from_engine_ref,"what is the answer to the question 'does it like cheese'",15)==0);
    update_model(model,"yes");
    mu_assert("think about an animal",model->state == GUESSING_STATE);
    mu_assert("non leaf",model->knowledge_tree_class->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("leaf",model->knowledge_tree_class->yes_branch->leaf_or_not_leaf==IS_LEAF);
    mu_assert("elephant",strcmp(model->knowledge_tree_class->yes_branch->animal,"elephant")==0);
    mu_assert("leaf",model->knowledge_tree_class->no_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("does it like cheese?",strcmp(model->knowledge_tree_class->no_branch->discriminating_question,"does it like cheese?")==0);
    mu_assert("leaf 2",model->knowledge_tree_class->no_branch->yes_branch->leaf_or_not_leaf=IS_LEAF);
    mu_assert("mouseX",strcmp(model->knowledge_tree_class->no_branch->yes_branch->animal,"mouse")==0);
    mu_assert("mouseX2",strcmp(model->knowledge_tree_class->no_branch->no_branch->animal,"cat")==0);
    mu_assert("guessing",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert("asf",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"");
    update_model(model,"");
    mu_assert("engine",strcmp(model->message_from_engine_ref,"is it big?")==0);
    update_model(model,"no");
    mu_assert("engine",strcmp(model->message_from_engine_ref,"does it like cheese?")==0);
    update_model(model,"no");



    return 0;
}


static char *reproducing_bug() {
    Model* model = get_initial_model();
    update_model(model,"");
    update_model(model,"");
    mu_assert("non elephant",strcmp(model->message_from_engine_ref,"is it a elephant?")==0);
    update_model(model,"no");
    update_model(model,"cat");
    update_model(model,"is it big?");
    update_model(model,"no"); 
    mu_assert("should be non leaf",model->knowledge_tree_class->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree_class->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be elephant",strcmp(model->knowledge_tree_class->yes_branch->animal,"elephant")==0);
    mu_assert("no branch should be cat",strcmp(model->knowledge_tree_class->no_branch->animal,"cat")==0);

    // starts again
    update_model(model,""); 

    update_model(model,"no"); // big
    update_model(model,"no"); // cat
    update_model(model,"mouse"); 
    update_model(model,"cheese?"); 
    update_model(model,"yes");  

    mu_assert("should be non leaf",model->knowledge_tree_class->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree_class->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be elephant",strcmp(model->knowledge_tree_class->yes_branch->animal,"elephant")==0);
    mu_assert("no branch should be non leaf",(model->knowledge_tree_class->no_branch->leaf_or_not_leaf == IS_NON_LEAF));
    mu_assert("no branch question should bshould be cheese",strcmp(model->knowledge_tree_class->no_branch->discriminating_question,"cheese?")==0);
    mu_assert("mouse check",strcmp(model->knowledge_tree_class->no_branch->yes_branch->animal,"mouse")==0);
    mu_assert("cat check",strcmp(model->knowledge_tree_class->no_branch->no_branch->animal,"cat")==0);

    update_model(model,""); 
    
    // starts again
    update_model(model,"yes");  
    update_model(model,"no");  
    update_model(model,"pterodaptil");  
    update_model(model,"estinct?");  
    update_model(model,"yes");  

    mu_assert("should be non leaf",model->knowledge_tree_class->leaf_or_not_leaf==IS_NON_LEAF); 
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree_class->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be non leafXXXX",model->knowledge_tree_class->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("question is estinct" ,strcmp(model->knowledge_tree_class->yes_branch->discriminating_question,"estinct?")==0);
    mu_assert("yes branch of yes branch should be leaf",model->knowledge_tree_class->yes_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    mu_assert("no branch of yes branch should be leaf",model->knowledge_tree_class->yes_branch->no_branch->leaf_or_not_leaf == IS_LEAF);

    mu_assert("pterodaptil",strcmp(model->knowledge_tree_class->yes_branch->yes_branch->animal,"pterodaptil")==0);
    mu_assert("elephant",strcmp(model->knowledge_tree_class->yes_branch->no_branch->animal,"elephant")==0);

    // starts again
    
    update_model(model,"");  
    update_model(model,"yes");  
    update_model(model,"no");  
    update_model(model,"no");  
    update_model(model,"cow");  
    update_model(model,"deliver milk?");  
    update_model(model,"yes");  
    update_model(model,"");  
    char *outmessage = concatenate_strings(2,"initial state issue ",model->message_from_engine_ref);
    mu_assert(outmessage,strcmp(model->message_from_engine_ref,"is it big?")==0);
    
    mu_assert("should be non leaf",model->knowledge_tree_class->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree_class->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be non leaf",model->knowledge_tree_class->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("question is estinct" ,strcmp(model->knowledge_tree_class->yes_branch->discriminating_question,"estinct?")==0);
    mu_assert("yes branch of yes branch should be leaf",model->knowledge_tree_class->yes_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    mu_assert("no branch of yes branch should be non leaf",model->knowledge_tree_class->yes_branch->no_branch->leaf_or_not_leaf == IS_NON_LEAF);
    mu_assert("deliver milk question",strcmp(model->knowledge_tree_class->yes_branch->no_branch->discriminating_question,"deliver milk?")==0);
    mu_assert("yes branch of no branch of yes branch should be leaf",model->knowledge_tree_class->yes_branch->no_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    mu_assert("no branch of no branch of yes branch should be leaf",model->knowledge_tree_class->yes_branch->no_branch->no_branch->leaf_or_not_leaf == IS_LEAF);

    mu_assert("yes branch of no branch of yes branch should be cow",strcmp(model->knowledge_tree_class->yes_branch->no_branch->yes_branch->animal,"cow") == 0);
    mu_assert("no branch of no branch of yes branch should be elephant",strcmp(model->knowledge_tree_class->yes_branch->no_branch->no_branch->animal,"elephant") == 0);

    update_model(model,"");  
    mu_assert("ask if it is big",strcmp(model->message_from_engine_ref,"is it big?")==0);
    update_model(model,"yes");  
    mu_assert("ask if it is estinct",strcmp(model->message_from_engine_ref,"estinct?")==0);
    update_model(model,"no");  
    mu_assert("ask if it deliver milks",strcmp(model->message_from_engine_ref,"deliver milk?")==0);
    update_model(model,"yes");  
    mu_assert("ask if it is an elephant",strcmp(model->message_from_engine_ref,"is it a cow?")==0);
    update_model(model,"no");  

    return 0;
}






static char * all_tests() {
    mu_run_test(test_foo);
    mu_run_test(test_knowledge_tree_class);
    mu_run_test(test_add_element_to_list);
    mu_run_test(when_guess_is_correct_then_the_message_will_be_yeah);
    mu_run_test(when_guess_is_not_correct_then_the_state_will_be_ask_what_animal_was);
    mu_run_test(after_asking_what_animal_was_will_get_ask_the_question_to_distinguish);
    mu_run_test(after_giving_the_question_to_distinguish_animals_will_as_what_is_the_answer);
    mu_run_test(after_asking_what_is_the_answer_to_the_discriinating_will_receive_the_answer);
    mu_run_test(will_create_a_new_tree_structure_after_learning);
    mu_run_test(will_navigate_the_non_leaf_tree_by_questions);
    mu_run_test(will_navigate_the_non_leaf_tree_by_questions_2);
    mu_run_test(leaning_on_a_non_leaf_tree);
    return 0;
}

int main(int argc, char **argv) {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);
 
     return result != 0;
}
