# Wordle

## Important Update:
Based on questions in the last couple of days, it is clear that handling duplicate letters in the solution and in the guesses is more challenging that we originally realized.  To simplify the constraints, we are going to make the following change to the specifications:  the solution word will not contain any duplicate letters, and guesses may not contain duplicate solution word letters. 

For example, if the solution word is "light" and the row one is "gg--g" then "licit" is not a valid guess because it contains two i's, and the cannot_be set will be "g h i l t".  This means that the input1 example has no valid paths.

Note that we can still accept words that contain other duplicate letters as valid guesses.  For example, if the solution word is "perky" then a valid guess given "-yy---" could be "tress".  Since 's' is not a solution letter, it is fine for it to be duplicated.

If you have already implemented a solution that handles duplicate paths you do not need to change it.  Please document it clearly so that the TAs can see the design decisions that you made. 

Example input files and the corresponding list of paths are in samples.zip Download samples.zip  For the input file named "inputX", the corresponding list of paths is in a file called "inputX.out".  Note that the order of printing the paths in your solution could be different than in the example output.  Because the input in "input1" does not produce any valid paths, the file "input1.out" is empty.

Also note that the TAs will be focusing their grading on building and traversing the tree rather than the constraints.

## Update: 
Some of the example pdfs have been updated to fix typos.  The only significant change is illustrated in a new example: largo-example.pdf Download largo-example.pdf  You may also find this example useful because it has yellow tiles in the row closest to the solution and has a very small tree.

## Introduction
We just couldn't resist creating an assignment around wordle Links to an external site., the word game that has caught the attention of the world.  Please read the rules for wordle in the app, and try playing a couple of times if you haven't already.

This assignment is not about writing a program to solve the game.  Instead we are going to produce a tree that contains all of the possible paths to a solution given 1) the solution word, and 2) the grid of coloured tiles that will impose constraints on the potential paths, and then print all of the possible paths to a solution.

The goal of this assignment is to give you experience working with dynamic data structures and strings.

### Wordle rules for our version of the game
Your program will actually apply these rules in reverse order, but it is helpful to understand the rules of the game when someone is trying to guess the solution.

Each guess must be a valid WORDLEN-letter word.  After each guess, the colour of the tiles will change to show which, if any, letters are in the solution word.   (WORDLEN is defined as 5 in common.h, but you should not use hard-coded numbers in your code.)

* A green tile means that the letter is in the solution in the same position. 
* A yellow tile means that the letter is in the solution in a different position. 
* A grey tile means that the letter is not in the solution.

For our purposes, we are going to create an extra-hard mode: 

* If a letter was marked as grey, it cannot be used again.
* If a letter was marked as yellow, that letter must be used in the next guess in a different position that the previous guess.
* If a letter was marked as green, that letter must be used in the same position in the next guess.

### Reverse Wordle Constraints
In our solver, we will be identifying possible words in reverse order.  The first step will be to start with the solution word, and use the tile information in the second-to-last guess to create a list of possible words for that row.  So, working in reverse order the rules need to be expressed differently so that we can develop the set of constraints that we can use to test the words in the dictionary to see if they will match.

The constraints struct is the data structure used to identify which words in the dictionary could be used in a given row.  Please see the first example Download first exampleand the header file for more information about how the constraints are stored and constructed.

A second illustrated example  Download illustrated exampleis provided that shows one possible path from the solution to a starting word. (Update: I believe this example is still fine except that the solution word has a duplicate letter in it.  This doesn't change the program.)

Now let's consider the steps needed to build the tree structure of possible paths to the solution.

**Step 1**: Read the dictionary file into a linked list

The dictionary file contains a list of 5-letter words, one per line.  Read the file and construct a linked list of struct nodes.  This is a task in the extra lab, so you can just copy your code from that lab.

**Step 2** Read the wordle input and initialize the wordle struct. This function is given to you.

**Step 3** Initialize the root node of the tree.  The constraints struct for the root node initially is empty.

**Step 4** Build the solver tree

The solver tree is a general tree where a node can have an arbitrary number of children.  A parent node has a pointer to a linked list of child nodes.  The linked list of child nodes is connected through a next_sibling pointer.  See the solver tree  Download solver treediagram.

**Step 4 a)** Construct the constraints that will be applied to generate the list of children for this node.  Use the functions set_green, set_yellow, and add_to_cannot_be that you wrote in the extra lab.

**Step 4 b)** Iterate over the words in the dictionary, calling match_constraints on each word to see if it fits.  If a word matches,  create a new struct solver_node, add it to the list of children of this node, and call solve_subtree on it.  The base case of the recursion is when we reach the end of the rows in the wordle input.

### match_constraints() algorithm
There are several checks to make to see if a word matches the constraints in the struct constraints.

* For each index in word, check if the word[index] is in the must_be[index] string.  If it isn't, then this word does not match the constraints.
* If  must_be[index] is the empty string and word[index] is in the cannot_be set, this word does not match the constraints.
* If there are multiple yellow tiles in this row, ensure that we are not matching on words that contain duplicates of letters in the solution.  For example if a row had two yellow tiles and both of them could be either 'u' or 't', then we should only match on words where both yellow are 'u' or both yellow are 't' if there are two 'u's or two 't's in the solution.
The first few nodes of a tree including the constraints are shown in solver_tree.pdfDownload solver_tree.pdf

### print_paths
Prints all the the paths from the root to the leaves that have length equal to the number of rows in the wordle.

## Details
* Enough space is reserved in the data structures that all words are stored as strings including the grid.  The only character array that is not null-terminated in the array the "cannot_be" set in the struct constraints.  This means that you can use string functions when it is convenient.
* Do not include hard-coded numbers.  We should be able to change the constants in common.h to allow for different sizes of words. One difference from the extra lab is that we have added a MAXLINE constant that will be useful for creating an array to read input from the input files.  You can include this in your extra lab code.
* Input files will contain valid input data.  You don't have to handle poorly formatted or incomplete input data.
    * The first line will contain the solution word
    * The remaining lines will contain the grid used to get to the solution word
    * The grid is stored in reverse order to how you would solve a wordle problem.
    * The characters in this file will always be lower-case
* Do not change the header files.  We should be able to write additional programs with main functions to test functions declared in wordle.h, wordlist.h, and constraints.h independently.
* You are welcome to add to the Makefile if you create additional tests.
* Please add any additional helper functions to the appropriate source code file.  There isn't very much code to write for this assignment, so you are unlikely to need many helper functions.
## Submission
Commit all code required to compile and run your program.  You are welcome to include other C source files that you use for testing.  Do not include executable files, or any other auto-generated files. 
