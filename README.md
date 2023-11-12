# POKER
A simple poker game programmed in C

## Description
1. This program assembles a deck out of character arrays and orders it in the order of a freshly opened pack of cards
2. It shuffles the cards using a simple iterate and swap algortihm
3. It makes use of strcpy to deal the cards
4. It creates a new array with numeric values for sorting and assessment 
5. It uses a (admittedly unnecessary) decrimental insertion sort algorithm to sort the cards for assessment 
6. The assess function nests another of other functions: a duplicate checker, a straight checker and a flush checker. These functions work in tandem to see which of the possible poker hands the player has, with a decimal value being added according to the highest card e.g. if the player has a pair of threes and the cpu has a pair of fours, the player's hand value will be 1.3 and the dealer's 1.4 

## Background
I did this project to improve my skills with pointers and data-structures in C. 

