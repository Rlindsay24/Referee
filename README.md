# Referee
A simple compiler from 'Shootout', (an invented BASIC-like language based on the game of football) to Python 2.7.
The language is created to be simple, with the aim of being interesting to children who have never programmed before.

Referee is the compiler used to translate Shootout into Python. It performs lexical analysis, syntax checking and code generation
on a Shootout file. Referee uses statements written in Backus-Naur form to define the allowable syntax of Shootout.
A GUI is also provided, written in Python using Pygame and TkInter.

A language reference is provided in the Doc folder, along with a brief user guide for Referee.
Example programs are provided in Doc\Examples, these can be compiled quickly to observe the compiler in action.

Usage: ref.exe 'Path\To\Shootout_code.txt' 'Name\Of\Output_code.py'
