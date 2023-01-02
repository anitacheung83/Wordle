#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordlist.h"


/* Read the words from a filename and return a linked list of the words.
 *   - The newline character at the end of the line must be removed from
 *     the word stored in the node.
 *   - You an assume you are working with Linux line endings ("\n").  You are
 *     welcome to also handle Window line endings ("\r\n"), but you are not
 *     required to, and we will test your code on files with Linux line endings.
 *   - The time complexity of adding one word to this list must be O(1)
 *     which means the linked list will have the words in reverse order
 *     compared to the order of the words in the file.
 *   - Do proper error checking of fopen, fclose, fgets
 */
int main(){
    char filename[] = "testfiles/small_words5.txt";
    struct node *dictionary = read_list(filename);
    print_dictionary(dictionary);
    return 0;
}



struct node *read_list(char *filename) {
    // TODO - Remember to update return statement
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL){
        printf("Error!");
        exit(1);
    }
    char temp[6];
    struct node *front = NULL;
    while (fgets(temp, 6, fptr) != NULL){
        struct node *new_node = malloc(sizeof(struct node));
        strcpy(new_node->word, temp);
        new_node->next = front;
        front = new_node;
    }
    fclose(fptr);

    return front;
}

/* Print the words in the linked-list list one per line
 */
void print_dictionary(struct node *list) {
    // TODO
    struct node *curr = list;
    while (curr != NULL){
        printf("%s\n", curr->word);
        curr = curr->next;
    }
}
/* Free all of the dynamically allocated memory in the dictionary list 
 */
void free_dictionary(struct node *list) {
    // TODO
    struct node *curr = list;
    while (curr != NULL) {
        struct node *temp = curr->next;
        free(curr);
        curr = temp;
    }
}

