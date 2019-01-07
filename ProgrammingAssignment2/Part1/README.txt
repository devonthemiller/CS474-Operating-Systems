Simple program that uses two processes and two pipes to
flip the cases of all the characters in a given string.
Parent process sends inputted string to child process.
Child process receives string, flips the cases, and then
sends the flipped string back to the parent process. Parent
process then prints out the flipped string.

Instructions:
Compile using 'make'
Run using './pipe'
Input string when prompted
String must not be longer than 100 characters
