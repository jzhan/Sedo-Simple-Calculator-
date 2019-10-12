#include "lexer.h"
#include <vector>

#ifndef PARSER_H
#define PARSER_H

enum ErrorID {ILLEGAL_CHAR = 1, UNCOMPLETED_EXPR, UNDECLARE_ID};

struct Error {
  ErrorID state;
  bool is_error;
};

class Parser: public Lexer {
  private:
    struct Num result;
    Error error;
    bool is_eof;
    struct Identifier {
        char name[27];
        struct Num value;
    } identifier;
    std::vector<struct Identifier> list_of_identifier;

  public:
    Parser() {}

    struct Num factor() {
        struct Num temp;

        if(current_token.type == INT) {
            temp.value.ivalue = current_token.info.int_num;
            temp.active_state = INT;
        } else if(current_token.type == FLOAT) {
            temp.value.fvalue = current_token.info.float_num;
            temp.active_state = FLOAT;
        } else if(current_token.type == ID){
            int n = list_of_identifier.size();
            bool flag_in_list = false;

            for(int i = 0; i < n; ++i) {
                if(strcmp(list_of_identifier[i].name, current_token.info.id) == 0) {
                    temp = list_of_identifier[i].value;
                    flag_in_list = true;
                }
            }

            if(!flag_in_list) {
                error.is_error = true;
                error.state = UNDECLARE_ID;
                return temp;
            }
        } else if(current_token.type == LPAREN) {
            is_eof = !getNextToken();

            if(!is_eof) {
                temp = expresion();

                if(current_token.type != RPAREN) {
                    if(!error.is_error) {
                        error.is_error = true;
                        error.state = UNCOMPLETED_EXPR;
                    }

                    return temp;
                } else 
                    error.is_error = false;
            }
        } else if(current_token.type == RPAREN){
            temp.value.ivalue = 0;
            temp.active_state = INT;
        } else {
            error.is_error = true;
            error.state = ILLEGAL_CHAR;
            return temp;
        }

        error.is_error = false;

        is_eof = !getNextToken();

        return temp;
    }

    struct Num term() {
        struct Num temp = factor();

        if(error.is_error and error.state != UNCOMPLETED_EXPR) {
            return temp;
        }

        while(!is_eof and (current_token.type == MUL or current_token.type == DIV)) {
            struct Num temp2;
            
            switch(current_token.type) {
                case MUL:
                    is_eof = !getNextToken();

                    if(!is_eof) {
                        temp2 = factor();

                        if(temp.active_state == FLOAT or temp2.active_state == FLOAT) {
                            if(temp.active_state == INT) {
                                temp.value.fvalue = temp.value.ivalue;

                                temp.active_state = FLOAT;
                            }

                            if(temp2.active_state == INT) {
                                temp2.value.fvalue = temp2.value.ivalue;
                                
                                temp2.active_state = FLOAT;
                            }

                            temp.value.fvalue *= temp2.value.fvalue;
                        } else {
                            temp.value.ivalue *= temp2.value.ivalue;
                        }
                    } else {
                        error.is_error = true;
                        error.state = UNCOMPLETED_EXPR;

                        return temp;
                    }

                    break;
                case DIV:
                    is_eof = !getNextToken();

                    if(!is_eof) {
                        temp2 = factor();
                        
                        if(temp.active_state == INT) {
                            temp.value.fvalue = temp.value.ivalue;

                            temp.active_state = FLOAT;
                        }

                        if(temp2.active_state == INT) {
                            temp2.value.fvalue = temp2.value.ivalue;
                            
                            temp2.active_state = FLOAT;
                        }

                        temp.value.fvalue /= temp2.value.fvalue;
                    } else {
                        error.is_error = true;
                        error.state = UNCOMPLETED_EXPR;

                        return temp;
                    }

                    break;
            }
        }

        return temp;
    }

    struct Num expresion() {
        struct Num temp = term();

        if(error.is_error and error.state != UNCOMPLETED_EXPR) {
            return temp;
        }

        if(current_token.type == ASSIGN) {
            error.is_error = true;
            error.state = ILLEGAL_CHAR;

            return temp;
        }

        while(!is_eof and (current_token.type == PLUS or current_token.type == MIN)) {  
            struct Num temp2;
            
            switch(current_token.type) {
                case PLUS:
                    is_eof = !getNextToken();

                    if(!is_eof) {
                        temp2 = term();

                        if(temp.active_state == FLOAT or temp2.active_state == FLOAT) {
                            if(temp.active_state == INT) {
                                temp.value.fvalue = temp.value.ivalue;
                                
                                temp.active_state = FLOAT;
                            }

                            if(temp2.active_state == INT) {
                                temp2.value.fvalue = temp2.value.ivalue;
                                
                                temp2.active_state = FLOAT;
                            }

                            temp.value.fvalue += temp2.value.fvalue;
                        } else {
                            temp.value.ivalue += temp2.value.ivalue;
                        }
                    } else {
                        error.is_error = true;
                        error.state = UNCOMPLETED_EXPR;

                        return temp;
                    }

                    break;
                case MIN:
                    is_eof = !getNextToken();

                    if(!is_eof) {
                        temp2 = term();
                        
                        if(temp.active_state == FLOAT or temp2.active_state == FLOAT) {
                            if(temp.active_state == INT) {
                                temp.value.fvalue = temp.value.ivalue;

                                temp.active_state = FLOAT;
                            }

                            if(temp2.active_state == INT) {
                                temp2.value.fvalue = temp2.value.ivalue;
                                
                                temp2.active_state = FLOAT;
                            }

                            temp.value.fvalue -= temp2.value.fvalue;
                        } else {
                            temp.value.ivalue -= temp2.value.ivalue;
                        }
                    } else {
                        error.is_error = true;
                        error.state = ILLEGAL_CHAR;

                        return temp;
                    }

                    break;
            }
        }
        
        return temp;
    }

    void parse(char *param_str) {
        str = param_str;

        len = strlen(str);
        current_position = 0;

        result.active_state = INT;
        result.value.ivalue = 0;

        error.is_error = false;
        is_eof = !getNextToken();

        if(!is_eof) {
            struct Token peek_token;
            bool flag_peek_token = peek(peek_token);
            bool flag_assign_statement = false;
            char identifier_name[27];

            if(current_token.type == ID and flag_peek_token and peek_token.type == ASSIGN) {
                flag_assign_statement = true;
                strcpy(identifier_name, current_token.info.id);

                getNextToken();
                is_eof = !getNextToken();

                error.is_error = true;
                error.state = UNCOMPLETED_EXPR;
            }
            
            if(!is_eof)
                result = expresion();
            
            if(!error.is_error) {
                if(flag_assign_statement) {
                    int n = list_of_identifier.size();
                    bool flag_in_list = false;

                    for(int i = 0; i < n; ++i) {
                        if(strcmp(list_of_identifier[i].name, identifier_name) == 0) {
                            flag_in_list = true;

                            list_of_identifier[i].value = result;
                        }
                    }

                    if(!flag_in_list) {
                        struct Identifier new_identifier;

                        strcpy(new_identifier.name, identifier_name);
                        new_identifier.value = result;

                        list_of_identifier.push_back(new_identifier);
                    }
                }
                
                std::cout << (result.active_state == INT? result.value.ivalue: result.value.fvalue) 
                            << "\n";
            } else {
                if(error.state == ILLEGAL_CHAR) {
                    for(int i = 0; i < current_position - 1; ++i) {
                        std::cout << " ";
                    }

                    std::cout << "^\n";
                    std::cerr << "Error: terdapat karakter ilegal\n";
                } else if(error.state == UNCOMPLETED_EXPR) {
                    std::cerr << "Error: ekspresi tidak lengkap\n";
                } else if(error.state == UNDECLARE_ID) {
                    std::cerr << "Error: variabel belum terdefinisikan\n";
                }
            }      
        }
    }
};

#endif
