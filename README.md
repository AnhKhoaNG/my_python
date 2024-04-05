# Simple Interpreter for Python-like Language

This C++ program implements a simple interpreter for a Python-like language. The interpreter can tokenize source code, execute print statements, handle if statements, and evaluate mathematical expressions involving addition, subtraction, multiplication, division, and comparison operators.

## How it Works

1. **Tokenization**: The `Scanner` class tokenizes the input source code, identifying different types of tokens such as identifiers, numbers, strings, operators, and keywords like "print", "if", and "else".

2. **Interpreter Execution**: The `Interpreter` class processes the tokenized code and executes it accordingly. It supports print statements, assignment statements, if statements, and arithmetic operations.

## Token Types

- **Identifier**: Represents variable names
- **Number**: Represents numeric literals
- **String**: Represents string literals
- **Print**: Represents the print keyword
- **If, Else**: Represents the conditional statements
- **Operators**: 
  - PLUS, MINUS, STAR, SLASH,
  - EQUAL_EQUAL, BANG_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, EQUAL,
  - LEFT_PAREN, RIGHT_PAREN, COMMA.

## Example Input 
```python
a = 100 + 2 * 3 * 4 * 5 + 12
b = 10 * 2 + 3 * 5 + 4 * 6 + 1 * 2
c = 10 + 20 + 30 + 40 + 50 + 1 * 60
# print
print("a =", a)
print("b =", b)
print("c =", c)
```
## Example Output
a = 232
b = 61
c = 210

## Running the Program
To compile the program, use the following syntax:
```bash
g++ -std=c++11 *.cpp -o mypython
```
or 
```bash
g++ -o mypython myprogram.cpp
```
Then to execute the program, the following syntax is:
```bash
./mypython <file.py>
```
