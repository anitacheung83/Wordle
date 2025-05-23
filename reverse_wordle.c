#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordle.h"
#include "constraints.h"

/* Read the wordle grid and solution from fp. 
 * Return a pointer to a wordle struct.
 * See sample files for the format. Assume the input file has the correct
 * format.  In other words, the word on each is the correct length, the 
 * words are lower-case letters, and the line ending is either '\n' (Linux,
 * Mac, WSL) or '\r\n' (Windows)
 */
struct wordle *create_wordle(FILE *fp) {
    struct wordle *w = malloc(sizeof(struct wordle));
    char line[MAXLINE];
    w->num_rows = 0;

    while(fgets(line, MAXLINE, fp ) != NULL) {

        // remove the newline character(s)
        char *ptr;
        if(((ptr = strchr(line, '\r')) != NULL) ||
           ((ptr = strchr(line, '\n')) != NULL)) {
            *ptr = '\0';
        }

        strncpy(w->grid[w->num_rows], line, SIZE);
        w->grid[w->num_rows][SIZE - 1] = '\0';
        w->num_rows++;
    }
    return w;
}


/* Create a solver_node and return it.
 * If con is not NULL, copy con into dynamically allocated space in the struct
 * If con is NULL set the new solver_node con field to NULL.
 * Tip: struct assignment makes copying con a one-line statements
 */
struct solver_node *create_solver_node(struct constraints *con, char *word) {

    // TODO - replace return statement when function is implemented
    struct solver_node *solver = malloc(sizeof(struct solver_node));
    if (con == NULL){
        solver->con = NULL;
    }
    else {
        solver->con = con;
    }
    strcpy(solver->word, word);
    solver->next_sibling = NULL;
    solver->child_list = NULL;
    return solver;
}

/* Return 1 if "word" matches the constraints in "con" for the wordle "w".
 * Return 0 if it does not match
 */
int match_constraints(char *word, struct constraints *con, 
struct wordle *w, int row) {

    // TODO
    // Loop over word to check the conditions
    for (int i = 0; i < WORDLEN; i++){
        //Condition 1: if con->must_be is not empty and word[index] is not in the must_be[index] string, return 0.
        if (con->must_be[i][0] != '\0'){
            if (strchr(con->must_be[i], word[i]) == NULL){
                return 0;
            }
        }
        // Check Condition 2: if con->must_be is empty and word[index] is in the cannot_be set, return 0.
        else if (con->must_be[i][0] == '\0'){
            if (con->cannot_be[word[i] - 'a'] == 1){
                return 0;
            }
        }
        // Check Condition 3: if there are duplicate solution letters in word, return 0.
        if (strchr(w->grid[0], word[i]) != NULL){
            for (int j = i; j < WORDLEN; j++){
                if (word[i] == word[j]){
                    return 0;
                }
            }
        }
    }
    return 1;
}
/* remove "letter" from "word"
 * "word" remains the same if "letter" is not in "word"
 */
void remove_char(char *word, char letter) {
    char *ptr = strchr(word, letter);
    if(ptr != NULL) {
        *ptr = word[strlen(word) - 1];
        word[strlen(word) - 1] = '\0';
    }
}

/* Build a tree starting at "row" in the wordle "w". 
 * Use the "parent" constraints to set up the constraints for this node
 * of the tree
 * For each word in "dict", 
 *    - if a word matches the constraints, then 
 *        - create a copy of the constraints for the child node and update
 *          the constraints with the new information.
 *        - add the word to the child_list of the current solver node
 *        - call solve_subtree on newly created subtree
 */

void solve_subtree(int row, struct wordle *w,  struct node *dict, 
                   struct solver_node *parent) {
    if(verbose) {
        printf("Running solve_subtree: %d, %s\n", row, parent->word);
    }
    //base case
    if (row == w->num_rows){
        return;
    }
    //recursive step
    else{
        //update parent's constraint
        for (int i = 0; i <WORDLEN; i++){
            if (w->grid[row][i] == 'g'){
                set_green(parent->word[i], i, parent->con);
            }
            else if (w->grid[row][i] == 'y'){
                set_yellow(i, w->grid[row], w->grid[row-1], parent->word, parent->con);
            }
            else if (w->grid[row][i] == '-'){
                add_to_cannot_be(&(parent->word[i]), parent->con);
            }
        }
        // Loop over dictionary to find words that match constraints
        while (dict->next != NULL){
            if (match_constraints(dict->word, parent->con, w, row) == 1){
                struct constraints *new_constraints = init_constraints();
                struct solver_node *new_solver_node = create_solver_node(new_constraints, dict->word);
                if (parent->child_list == NULL){
                    parent->child_list = new_solver_node;
                }
                else{
                    struct solver_node *curr = parent->child_list;
                    while (curr->next_sibling!= NULL){
                       curr = curr->next_sibling;
                    }
                    curr->next_sibling = new_solver_node;
                }
                solve_subtree(row+1, w, dict, new_solver_node);
            }
        }
        // Check if the last dict->word matches constraints
        if (match_constraints(dict->word, parent->con, w, row) == 1){
            struct constraints *new_constraints = init_constraints();
            struct solver_node *new_solver_node = create_solver_node(new_constraints, dict->word);
            if (parent->child_list == NULL){
                parent->child_list = new_solver_node;
            }
            else{
                struct solver_node *curr = parent->child_list;
                while (curr->next_sibling != NULL){
                    curr = curr->next_sibling;
                }
                curr->next_sibling = new_solver_node;
            }
        }
    }
    // TODO

    // debugging suggestion, but you can choose how to use the verbose option
    /*if(verbose) {
        print_constraints(c);
    } */

    // TODO

}

/* Print to standard output all paths that are num_rows in length.
 * - node is the current node for processing
 * - path is used to hold the words on the path while traversing the tree.
 * - level is the current length of the path so far.
 * - num_rows is the full length of the paths to print
 */

void print_paths(struct solver_node *node, char **path, 
                 int level, int num_rows) {

    //Case 1: No child, No sibling
    if (node->child_list == NULL && node->next_sibling == NULL){
        path[level-1] = node->word;
        for (int i = 0; i < num_rows; i++){
            printf("%s ", path[i]);
        }
        return;
    }
    //Case 2: No child, Yes sibling
    else if (node->child_list == NULL && node->next_sibling != NULL){
        for (int i = 0; i < level; i ++){
            printf("%s ", path[i]);
        }
        printf("%s\n", node->word);
        print_paths(node->next_sibling, path, level, num_rows);
        return;
    }
    //Case 3: Yes child, No sibling
    else if (node->child_list != NULL && node->next_sibling == NULL){
        path[level-1] = node->word;
        print_paths(node->child_list, path, level + 1, num_rows);
        return;
    }
    //Case 4: Yes child, Yes sibling
    else{
        print_paths(node->next_sibling, path, level, num_rows);
        print_paths(node->child_list, path, level + 1, num_rows);
        return;
    }
} 

/* Free all dynamically allocated memory pointed to from w.
 */ 
void free_wordle(struct wordle *w){
    // TODO
    free(&(w->num_rows));
    for (int i = 0; i < 6; i++){
        free(w->grid[i]);
    }
    free(w);
}

/* Free all dynamically allocated pointed to from node
 */
void free_tree(struct solver_node *node){
    // TODO
    //Case 1: No Child, No Sibling
    if (node->child_list == NULL && node->next_sibling == NULL){
        free_constraints(node->con);
        free(node->word);
        free(node);
        return;
    }
    //Case 2: No child, Yes Sibling
    else if (node->child_list == NULL && node->next_sibling != NULL){
        free_constraints(node->con);
        free(node->word);
        free_tree(node->next_sibling);
        free(node);
        return;
    }
    //Case 3: Yes child, No sibling
    else if (node->child_list != NULL && node->next_sibling == NULL){
        free_constraints(node->con);
        free(node->word);
        free_tree(node->child_list);
        free_tree(node);
        return;
    }
    //Case 4: Yes child, Yes sibling
    else{
        free_constraints(node->con);
        free(node->word);
        free_tree(node->child_list);
        free_tree(node->next_sibling);
        free(node);
        return;
    }

}
