- Course: CSC173
- Assignment: Project 1
- Name: Zhijie Wang
- Email: zwang179@u.rochester.edu
- Collaborator: Aijia Yu(Only Part 1 & Part 2)
- Collaborator's email: ayu27@u.rochester.edu
- My program has a clean report from valgrind.
(There is no error reported from valgrind except memory leaks. 
The instructions say memory leaks will not be punished)

- To build and run my program:
    -Step 1: type code below on terminal:
    gcc -std=c99 -Wall -Werror -o EXECUTABLE *.c

    -Step 2: To run my code using valgrind, type code below on terminal:
    valgrind ./EXECUATBLE

    OR to run without valgrind, type code below on terminal:
    ./EXECUTABLE