#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// An enum to store all the type of token we're receiving
enum TypeToken {
  IDENTIFIER,
  NUMBER,
  STRING,
  PRINT,
  IF,
  ELSE,
  PLUS,
  MINUS,
  STAR,
  SLASH,
  EQUAL_EQUAL,
  BANG_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,
  EQUAL,
  LEFT_PAREN,
  RIGHT_PAREN,
  COMMA,
  EOF_TOKEN,
  INDENT,
  End_of_line,
};

//Struct of token, to store its type, text, value and position
struct Token {
  TypeToken type; 
  string lexeme; 
  int literal;   
  int line;    
};

// Scanner class to tokenize the source code
class Scanner {
public:
  // Constructor taking the source code as input
  Scanner(const string &source_code) : source_code(source_code) {}
  int line = 1; //starting line

  // Function to tokenize the source code and return the tokens
  vector<Token> scanTokens() {
    while (!atEnd()) {
      start = current;
      scanToken();
    }
    tokens.push_back({EOF_TOKEN, "", 0, line}); // Add EOF token
    return tokens;
  }

private:
  const string &source_code;
  vector<Token> tokens; // Vector to store the tokens
  size_t start = 0;
  size_t current = 0;
  unordered_map<string, TypeToken> keywords = { // Map for keyword tokens
      {"print", PRINT}, {"if", IF}, {"else", ELSE}};

  // Function to check if the scanner has reached the end of the source code
  bool atEnd() const { return current >= source_code.size(); }

  // Functions to peek at the current and next characters in the source code
  char peek() const { return atEnd() ? '\0' : source_code[current]; }
  char peekNext() const { return current + 1 >= source_code.size() ? '\0' : source_code[current + 1]; }

  // Function to advance to the next character in the source code
  char advance() {
    current++;
    return source_code[current - 1];
  }

  // Function to add a token to the token list
  void addToken(TypeToken type, int literal = 0) {
    string lexeme = source_code.substr(start, current - start);
    tokens.push_back({type, lexeme, literal, line});
  }

    bool match(char check){
        //Function to check if the current char with our match 
        if (atEnd())
            return false;
        if (source_code[current] != check)
            return false;
        current++;
        return true;
    }
  // Function to scan the current token in the source code
  void scanToken() {
    char c = advance();
    switch (c) {
    
    case '+': addToken(PLUS); break;
    case ',': addToken(COMMA); break;
    case '-': addToken(MINUS); break;
    case '*': addToken(STAR); break;
    case '/': addToken(SLASH); break;
    case '#': while (peek() != '\n' && !atEnd()) advance(); break;
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '\n': addToken(End_of_line);line++; break;
    case ' ': case '\r': break;
    case '\t': addToken(INDENT); break; //doesn't work for now
    case '"': scanString(); break;
    case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
    case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;
    case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
    default: if (isdigit(c)) { scanNumber(); } else if (isalpha(c)) { scanIdentifier(); } break;
    }
  }

  // Function to scan string literals
  void scanString() {
    while (peek() != '"' && !atEnd()) {
      if (peek() == '\n') line++;
      advance();
    }
    if (atEnd()) return;
    advance();
    string lexeme = source_code.substr(start + 1, current - start - 2);
    addToken(STRING, 0);
  }

  // Function to scan numeric literals
  void scanNumber() {
    while (isdigit(peek())) advance();
    addToken(NUMBER, stoi(source_code.substr(start, current - start)));
  }

  // Function to scan identifiers and keywords
  void scanIdentifier() {
    while (isalnum(peek())) advance();
    string lexeme = source_code.substr(start, current - start);
    TypeToken type = keywords.count(lexeme) ? keywords.at(lexeme) : IDENTIFIER;
    addToken(type);
  }
};

// Interpreter class to execute the tokenized code
class Interpreter {
public:
  Interpreter(const vector<Token> &tokens) : tokens(tokens), current(0) {}

  // Function to start interpreting the code
  void interpret() {
    while (!isAtEnd()) {
      statement();
    }
  }

private:
  const vector<Token> &tokens;
  size_t current;
  unordered_map<string, int> variables;
  int line = 1;

  // Function to check if the interpreter has reached the end of the token list
  bool isAtEnd() const { return current >= tokens.size(); }

  // Function to advance to the next token in the list
  void advance() {
    if (tokens[current].type == End_of_line) line++;
    if (!isAtEnd()) current++;
  }

  // Function to execute a statement
  void statement() {
    if (match(PRINT)) {
      printStatement();
    } else if (match(IDENTIFIER)) {
      assignmentStatement();
    } 
    else if(match(IF)){
        ifStatement();
    }
    else if(match(ELSE)){
        statement();
    }
    else {
      advance();
    }
  }
  
    void ifStatement(){
        //cout << "current token: " << tokens[current].lexeme << endl;
        bool condition = compare_expression();
        //cout << condition << endl;
        if (condition){
            statement();
        }
        else{
            int first_indent = tokens[current].line;
            //cout << "First_indent: " << first_indent << endl;
            advance();
            while (tokens[current].type != End_of_line ){
                //cout << "current line in while " << tokens[current].line << endl;
                advance();
            }
            statement();
        }
    }
    bool compare_expression(){
        Token left = expression();
        //cout << "left token and value is:  " << left.lexeme << "value is: " <<left.literal << endl;
        while (match(GREATER, GREATER_EQUAL) || match(LESS, LESS_EQUAL) || match(EQUAL_EQUAL)){
            Token op = previous();
            //cout << "op token and value is:  " << op.lexeme << endl;
            Token right = expression();
            //cout << "right token and value is:  " << right.literal << endl;
            if (op.type == GREATER){
                //cout << left.literal << " " << right.literal << endl;
                //cout << (left.literal > right.literal) << endl;
                return left.literal > right.literal;
            }
            else if (op.type == GREATER_EQUAL){
                return left.literal >= right.literal;
            }
            else if (op.type == LESS){
                return left.literal < right.literal;
            }
            else if (op.type == LESS_EQUAL){
                return left.literal <= right.literal;
            }
            else if (op.type == EQUAL_EQUAL){
                //cout << left.literal << " " << right.literal << endl;
                //cout << (left.literal == right.literal) << endl;
                return left.literal == right.literal;
            }
        }
        return false;
    }
  // Function to execute a print statement
  void printStatement() {
    consume(LEFT_PAREN, ""); // Consume left parenthesis
    bool firstOutput = true;
    while (true) {
      Token nextToken = tokens[current];
      if (nextToken.type == STRING) {
        if (!firstOutput) cout << " ";
        cout << nextToken.lexeme.substr(1, nextToken.lexeme.size() - 2);
        advance();
      } else {
        Token value = expression();
        if (!firstOutput) cout << " ";
        if (value.type == IDENTIFIER) {
          if (variables.count(value.lexeme)) {
            cout << variables[value.lexeme];
          } else {
            exit(1);
          }
        } else {
          cout << value.literal;
        }
      }
      firstOutput = false;
      if (!match(COMMA)) break;
    }
    consume(RIGHT_PAREN, ""); // Consume right parenthesis
    cout << endl; // Print newline after completing the print statement
  }

  // Function to execute an assignment statement
  void assignmentStatement() {
    Token name = previous();
    consume(EQUAL, ""); // Consume equal sign
    Token value = expression();
    variables[name.lexeme] = value.literal;
  }

  // Function to evaluate an expression
  Token expression() {
    Token left = comparison();
    while (match(PLUS, MINUS)) {
      Token op = previous();
      Token right = comparison();
      if (op.type == PLUS) {
        left.literal += right.literal;
      } else if (op.type == MINUS) {
        left.literal -= right.literal;
      }
    }
    return left;
  }

  // Function to evaluate a comparison
  Token comparison() {
    Token left = addition();
    return left;
  }

  // Function to evaluate an addition or subtraction
  Token addition() {
    Token left = multiplication();
    while (match(PLUS, MINUS)) {
      Token op = previous();
      Token right = multiplication();
      if (op.type == PLUS) {
        left.literal += right.literal;
      } else if (op.type == MINUS) {
        left.literal -= right.literal;
      }
    }
    return left;
  }

  // Function to evaluate a multiplication or division
  Token multiplication() {
    Token left = unary();
    while (match(STAR, SLASH)) {
      Token op = previous();
      Token right = unary();
      if (op.type == STAR) {
        left.literal *= right.literal;
      } else if (op.type == SLASH) {
        left.literal /= right.literal;
      }
    }
    return left;
  }

  // Function to evaluate unary operations (e.g., negation)
  Token unary() {
    if (match(MINUS)) {
      Token op = previous();
      Token right = unary();
      return {NUMBER, "", -right.literal, op.line};
    }
    return primary();
  }

  // Function to evaluate primary expressions (e.g., literals, identifiers, parentheses)
  Token primary() {
    if (match(NUMBER)) {
      return previous();
    } else if (match(IDENTIFIER)) {
      if (variables.count(previous().lexeme)) {
        return {NUMBER, previous().lexeme, variables[previous().lexeme], previous().line};
      }
    } else if (match(LEFT_PAREN)) {
      Token expr = expression();
      consume(RIGHT_PAREN, ""); // Consume right parenthesis
      return expr;
    }
    exit(1);
  }

  // Function to check if the current token matches the expected token type
  bool match(TypeToken expected) {
    if (isAtEnd()) return false;
    if (tokens[current].type != expected)
        return false;
    advance();
    return true;
  }

  // Function to check if the current token matches one of two expected token types
  bool match(TypeToken t1, TypeToken t2) {
    if (isAtEnd()) return false;
    if (tokens[current].type != t1 && tokens[current].type != t2) return false;
    advance();
    return true;
  }

  // Function to consume the current token if it matches the expected token type
  Token consume(TypeToken type, const string &message) {
    if (tokens[current].type == type) {
      return tokens[current++];
    } else {
      exit(1);
    }
  }

  // Function to get the previous token
  Token previous() const { return tokens[current - 1]; }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
       return 1;
    }
    ifstream inputFile(argv[1]);

    // Check if the file is opened
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the input file." << std::endl;
        return 1; 
    }

    // Read file into a string
    string source_code;
    string line;
    while (getline(inputFile, line)) {
        source_code += line + "\n";
    }
    inputFile.close();


    Scanner scanner(source_code);
    vector<Token> tokens = scanner.scanTokens();

    Interpreter interpreter(tokens);
    interpreter.interpret();

    return 0;
}
