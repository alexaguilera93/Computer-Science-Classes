CS211 Assignment 1 Alex Aguilera
How to use this program:
To run the program, you must run wordstat<arguement> in the terminal.

Data Structure:

I used a Binary Search Tree in this assignment that holds sibling nodes as well to keep track of each case sensitive 
occurance of a word. The average run time of the tree is O(logN) for search and O(N) to print.

RunTime:
The Average Runtime of the program to build the tree is O(NlogN) then the time to print the tree is O(N). When we
combine these two we get that the runtime of the program is O(N^2logN).
