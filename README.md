This C++ program implements a simple interpreter for a Python-like language. The interpreter can tokenize source code, execute print statements, handle if statements, and evaluate mathematical expressions involving addition, subtraction, multiplication, division, and comparison operators.

How it Works?

  1.Tokenization: The Scanner class tokenizes the input source code, by identifying different types of tokens such as identifiers, numbers, strings, operators, and keywords like "print", "if", and "else".
  
  2. Interpreter Execution: The Interpreter class processes the tokenized code and executes it accordingly. It supports print statements, assignment statements, if statements, and arithmetic operations.
  
  3. Token Types:
    Identifier: Represents variable names
    Number: Represents numeric literals
    String: Represents string literals
    Print: Represents the print keyword
    If, Else: Represents the conditional statements
    Operators: PLUS, MINUS, STAR, SLASH,     
    EQUAL_EQUAL, BANG_EQUAL, GREATER, 
    GREATER_EQUAL, LESS, LESS_EQUAL, EQUAL, 
    LEFT_PAREN, RIGHT_PAREN, COMMA.
    
  4. Example Input:
    a = 100 + 2 * 3 * 4 * 5 + 12
    b = 10 * 2 + 3 * 5 + 4 * 6 + 1 * 2
    c = 10 + 20 + 30 + 40 + 50 + 1 * 60
    #print
    print("a =", a)
    print("b =", b)
    print("c =", c)
  This input calculates the values of variables a,   b, and c using mathematical expressions     
  involving addition, multiplication, and numeric 
  literals. It then prints the values of these 
  variables.
    Example Output:
    a = 232
    b = 61
    c = 210
  5.
  The syntax to run our program is following the requirement, so it can be:
    g++ -std=c++11 *.cpp -o mypython
  or
    g++ -o mypython myprogram.cpp

  After that, to execute our program, it would be:
    ./mypython <file.py>
  The above command will print the output to the console.
  And then to redirect the output to a text file, we simply add "> <outputfile.txt>" at the end, for example:
  ./mypython in01.py > out01.txt