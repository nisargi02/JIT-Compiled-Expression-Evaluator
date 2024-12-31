JIT-Compiled-Expression-Evaluator:

Designed a program that accepts a mathematical expression (a string containing thesymbols: +, -, *, /, %, (, ), value). This program will dynamically generate a C programbased on the input string, invoke a C compiler to create an equivalent loadable module,and then load and execute the machine code equivalent of the C program to produce theevaluated result of the expression.
Also, have implemented the sigmoid(double x) function thatresides in your main program but is called from the generated C code to transform thefi nal value of the expression (i.e., sigmoid(expression-value)).
