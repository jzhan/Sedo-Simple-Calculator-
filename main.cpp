#include "parser.h"

const char *interpreter_prefix = ">>> ";

int main() {
  char str[200];
  Parser parser;;

  while(true) {
    std::cout << interpreter_prefix;  
    gets(str);

    if(strcmp(str, "exit") == 0)
      return 0;

    std::cout << interpreter_prefix;
    
    parser.parse(str);
  }
  /* 
  while(lexer.getNextToken()) {
    switch(lexer.current_token.type) {
      case LPAREN:
        std::cout << "[" << lexer.current_token.info.op << ": LPAREN]";
        break;
      case RPAREN:
        std::cout << "[" << lexer.current_token.info.op << ": RPAREN]";
        break;
      case ASSIGN:
        std::cout << "[" << lexer.current_token.info.id << ": ASSIGN]";
        break;
      case ID:
        std::cout << "[" << lexer.current_token.info.id << ": ID]";
        break;
      case INT:
        std::cout << "[" << lexer.current_token.info.int_num << ": INT]";
        break;
      case FLOAT:
        std::cout << "[" << lexer.current_token.info.float_num << ": FLOAT]";
        break;
      case PLUS:
        std::cout << "[" << lexer.current_token.info.op << ": PLUS]";
        break;
      case MIN:
        std::cout << "[" << lexer.current_token.info.op << ": MIN]";
        break;
      case MUL:
        std::cout << "[" << lexer.current_token.info.op << ": MUL]";
        break;
      case DIV:
        std::cout << "[" << lexer.current_token.info.op << ": DIV]";
        break;
      case ILLEGAL:
        std::cout << "[" << lexer.current_token.info.ilchr << ": ILLEGAL]";
        break;
    }

    std::cout << " ";
  }
  */

  return 0;
}