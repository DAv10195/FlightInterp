created by David Abramov and Nikita Kogan.

We wanted to give a bit of information about our interpreter...

First of all, we support both command-line input and file input (including a command-line "run"
command which takes a file path as an argument).

The interpreter will run until "exit" command is inputed (via file or command-line).

Each argument passed via running the exexcutable file is read and executed command by command.

Make sure you run out Interpreter BEFORE running FlightGear!!!

Secondly, our interpreter lexes the input, and than parses it. upon parsing each command, in case
it is valid and it got valid paramaters, it will be executed on the spot.

each command is represented by a Command object that implements the Command interface (more
presicley abstract class). Upon their birth, commands are given a set of various tools, so they can handle their mission dealing with the various Expressions (also an interface - abstract class,
see UML file for more details).

during the entire running time of the interpreter (more presicley upon calling openDataServer command) another thread runs in the background updating the symbol table with current values read from FlightGear.

Have fun with FlightInterp 1.0!
