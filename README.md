# Basic C

The program is a part of an assignment for the **CS F351 - Theory of Computation** course of **[BITS Pilani, Hyderabad Campus](https://www.bits-pilani.ac.in/hyderabad/)**.  
The objective of the assignment was to build a simulator and parse tree generator for a simpler version of the C language, which supports integer constants, variables, expressions, assignments, for loops, and read and write statements.

## Description of Basic C

1. A program consists of a sequence of statements separated by semi-colons.
2. A statement is either a declaration-statement, read-statement, write-statement, assignment-statement, or a for-loop.
3. A declaration-statement is of the form `int V1, V2, ..., Vk;` where V1, V2, ..., Vk are variables of integer type. This statement must be the first statement in the program if it is presented and not allowed in any other part of the program. Further, no variable which is not listed here cannot be used in the rest of the program.
4. A read-statement is of the form `read X` where X is a variable.
5. A write-statement is of the form `write Y`, where Y is either a variable or an integer constant.
6. An assignment statement is a statement of the form `V = E` where V is a variable and E is an expression.
7. Expressions are obtained by applying binary operations +, −, ∗, /, >, == over variables
and constants. Note that ∗, / have higher precedence than +, − and +, − have higher precedence than >, ==. All binary operators associate from left to right. Further, expressions can also be parenthesized using ().
8. A for-loop is a statement of the form `for(E1; E2; E3){S};` where E1, E3 are assignment statements and E2 is the expression and S is a sequence of statements separated by semi-colons. Note that for-loop is the same as in C language except all statements must be written between { and } even if S is a single statement and there is a semi-colon (;) after }.
9. Variable names are from {a−z}<sup>+</sup> but not from the set of keywords {for, int, read, write}.
10. In addition to the variables, the language also has support for integer constants of the
form {0−9}<sup>+</sup>.

## Grammar

- \<program> → \<statements> | \<declaration>;  \<statements> | \<declaration>
- \<statements> → \<statement>; | \<statements> \<statement>;
- \<statement> → \<read> | \<write> | \<for_loop> | \<assignment>
- \<read> → read \<var>
- \<write> → write \<var> | write \<integer_const>
- \<assignment> → \<var> = \<expression>
- \<for_loop> → for(\<assignment>;\<expression>;\<assignment>) { \<statements> }
- \<declaration> → int \<vars>
- \<vars> -> \<var> | \<vars>, \<var>
- \<expression> → \<expression> == \<term> | \<expression> > \<term> | \<term>
- \<term> → \<term> + \<factor> | \<term> - \<factor> | \<factor>
- \<factor> → \<factor> * \<operand> | \<factor> / \<operand> | \<operand>
- \<operand> → (\<expression>) | \<var> | \<integer_const>
- \<integer_const> → \<integer> | \<integer_const>\<integer>
- \<integer> → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
- \<var> → \<letter> | \<var>\<letter>
- \<letter> → a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z

## Running the Program

1. **Compile the main.c file:**
    - Windows: Run `gcc -o main.exe main.c` in terminal.
    - Ubuntu:  Run `gcc -o main.out main.c` in terminal.
2. **Run the code:** (Input text file should have the code to simulate)
    - Windows: Run `.\main.exe <input_filename>` in terminal.
    - Ubuntu:  Run `.\main.out <input_filename>` in terminal.  
    The parse tree is generated and stored in `parse_tree.txt`.
3. **Run tree.py:** (To visualize the generated parse tree)
    - Windows: Run `python tree.py` in terminal.
    - Ubuntu:  Run `python3 tree.py` in terminal.

## Group Members

- **[Ritvik](https://github.com/Code-R57)**
- **[Abhinav Tyagi](https://github.com/Abhiinv)**
- **[Darshan Chandak](https://github.com/DarshanChandak)**
- **[Sai Hemanth](https://github.com/SETRAZ)**
- **[Jai Sehgal](https://github.com/Jai-Sehgal-26)**
