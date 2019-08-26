/* 
 * File: lexical_analyzer.c -> FINAL CODE
 * Implement a lexical analyzer for programming language PL/0
 * Author: Derek McCrae
 */

#include "lexical_analyzer.h"
#include "data.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Declares isalpa, isdigit, isalnum
#include <math.h>

/* ************************************************************************** */
/* Enumarations, Typename Aliases, Helpers Structs ************************** */
/* ************************************************************************** */

typedef enum {
    ALPHA,   // a, b, .. , z, A, B, .. Z
    DIGIT, // 0, 1, .. , 9
    SPECIAL, // '>', '=', , .. , ';', ':'
    INVALID  // Invalid symbol
} SymbolType;

/**
 * Following struct is recommended to use to keep track of the current state
 * .. of the lexer, and modify the state in other functions by passing pointer
 * .. to the state as argument.
 * */
typedef struct {
    int lineNum;         // the line number currently being processed
    int charInd;         // the index of the character currently being processed
    char* sourceCode;    // null-terminated source code string
    LexErr lexerError;   // LexErr to be filled when Lexer faces an error
    TokenList tokenList; // list of tokens
} LexerState;

/* ************************************************************************** */
/* Declarations ************************************************************* */
/* ************************************************************************** */

/**
 * Initializes the LexerState with the given null-terminated source code string.
 * Sets the other fields of the LexerState to their inital values.
 * Shallow copying is done for the source code field.
 * */
void initLexerState(LexerState*, char* sourceCode);

/**
 * Returns 1 if the given character is valid.
 * Returns 0 otherwise.
 * */
int isCharacterValid(char);

/**
 * Returns 1 if the given character is one of the special symbols of PL/0,
 * .. such as '/', '=', ':' or ';'.
 * Returns 0 otherwise.
 * */
int isSpecialSymbol(char);

/**
 * Returns the symbol type of the given character.
 * */
SymbolType getSymbolType(char);

/**
 * Checks if the given symbol is one of the reserved token.
 * If yes, returns the numerical value assigned to the corresponding token.
 * If not, returns -1.
 * For example, calling the function with symbol "const" returns 28.
 * */
int checkReservedTokens(char* symbol);

/**
 * Deterministic-finite-automaton to be entered when an alpha character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Alpha(LexerState*);

/**
 * Deterministic-finite-automaton to be entered when a digit character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Digit(LexerState*);

/**
 * Deterministic-finite-automaton to be entered when a special character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Special(LexerState*);

/* ************************************************************************** */
/* Definitions ************************************************************** */
/* ************************************************************************** */

void initLexerState(LexerState* lexerState, char* sourceCode){
    lexerState->lineNum = 0;
    lexerState->charInd = 0;
    lexerState->sourceCode = sourceCode;
    lexerState->lexerError = NONE;
    
    initTokenList(&lexerState->tokenList);
}

int isCharacterValid(char c){
    return isalnum(c) || isspace(c) || isSpecialSymbol(c);
}

int isSpecialSymbol(char c){
    return c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '(' || c == ')' || c == '=' || c == ',' ||
           c == '.' || c == '<' || c == '>' || c == ';' ||
           c == ':';
}

SymbolType getSymbolType(char c){
         if(isalpha(c))         return ALPHA;
    else if(isdigit(c))         return DIGIT;
    else if(isSpecialSymbol(c)) return SPECIAL;
    else                        return INVALID;
}

int checkReservedTokens(char* symbol){
    for(int i = firstReservedToken; i <= lastReservedToken; i++)
    {
        if( !strcmp(symbol, tokens[i]) )
        {
            // Symbol is the reserved token at index i.
            return i;
        }
    }

    // Symbol is not found among the reserved tokens
    return -1;
}


/**
 * Deterministic-finite-automaton to be entered when an alpha character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Alpha(LexerState* lexerState){
    //declare variables
    int index = lexerState->charInd, i = 0;
    char single;
    char string[13];
    Token new;
    
    //loop until not a number or letter
    while(isalnum(lexerState->sourceCode[index])){
        if(i == 0)
            single = lexerState->sourceCode[index];
        else if(i == 1){
            string[0] = single;
            string[1] = lexerState->sourceCode[index];
        }
        else
            string[i] = lexerState->sourceCode[index];
        
        if((i+ 1) > 11){
            lexerState->lexerError = NAME_TOO_LONG;
            return;
        }
        i++;
        index++;
    }
    lexerState->charInd = index;
    string[i] = '\0';
    
    //once string obtained, pass to token list
    if(i == 1){
        //Token 2
        new.id = 2;
        new.lexeme[0] = single;
        new.lexeme[1] = '\0';
        addToken(&lexerState->tokenList, new);
        return;
    }
    if(strcmp("odd", string) == 0){
        //Token 8
        new.id = 8;
        strcpy(new.lexeme, tokens[8]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("begin", string) == 0){
        //Token 21
        new.id = 21;
        strcpy(new.lexeme, tokens[21]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("end", string) == 0){
        //Token 22
        new.id = 22;
        strcpy(new.lexeme, tokens[22]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("if", string) == 0){
        //Token 23
        new.id = 23;
        strcpy(new.lexeme, tokens[23]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("then", string) == 0){
        //Token 24
        new.id = 24;
        strcpy(new.lexeme, tokens[24]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("while", string) == 0){
        //Token 25
        new.id = 25;
        strcpy(new.lexeme, tokens[25]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("do", string) == 0){
        //Token 26
        new.id = 26;
        strcpy(new.lexeme, tokens[26]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("call", string) == 0){
        //Token 27
        new.id = 27;
        strcpy(new.lexeme, tokens[27]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("const", string) == 0){
        //Token 28
        new.id = 28;
        strcpy(new.lexeme, tokens[28]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("var", string) == 0){
        //Token 29
        new.id = 29;
        strcpy(new.lexeme, tokens[29]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("procedure", string) == 0){
        //Token 30
        new.id = 30;
        strcpy(new.lexeme, tokens[30]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("write", string) == 0){
        //Token 31
        new.id = 31;
        strcpy(new.lexeme, tokens[31]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("read", string) == 0){
        //Token 32
        new.id = 32;
        strcpy(new.lexeme, tokens[32]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("else", string) == 0){
        //Token 33
        new.id = 33;
        strcpy(new.lexeme, tokens[33]);
        addToken(&lexerState->tokenList, new);
    }
    else if(strcmp("null", string) == 0){
        //Token 1
        new.id = 1;
        strcpy(new.lexeme, string);
        addToken(&lexerState->tokenList, new);
    }
    else{
        //Token 2
        new.id = 2;
        strcpy(new.lexeme, string);
        addToken(&lexerState->tokenList, new);
    }
    
    return;
}


/**
 * Deterministic-finite-automaton to be entered when a digit character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Digit(LexerState* lexerState){
    //declare variables
    Token new;
    int index = 0, i;
    char digits[5];
    char c = lexerState->sourceCode[lexerState->charInd];
    
    //obtain digits until reach space, end of line, or special character
    lexerState->charInd++;
    digits[index] = c;
    index++;
    while(lexerState->sourceCode[lexerState->charInd] != ' ' && lexerState->sourceCode[lexerState->charInd] != '\n' && isSpecialSymbol(lexerState->sourceCode[lexerState->charInd]) == 0){
        c = lexerState->sourceCode[lexerState->charInd];
        if(isalpha(c) && index == 1){
            lexerState->lexerError = NONLETTER_VAR_INITIAL;
            return;
        }
        if(index > 4){
            lexerState->lexerError = NUM_TOO_LONG;
            return;
        }
        digits[index] = c;
        
        lexerState->charInd++;
        index++;
    }
    
    //assign digit array to token list
    //Token 3
    new.id = 3;
    for(i = 0; i < index; i++){
        new.lexeme[i] = digits[i];
    }
    new.lexeme[i] = '\0';
    addToken(&lexerState->tokenList, new);
    return;
}

void DFA_Special(LexerState* lexerState){
    //declare structure
    Token new;
    
    //Comment case
    if(lexerState->sourceCode[lexerState->charInd] == '/' && lexerState->sourceCode[lexerState->charInd + 1] == '*'){
        lexerState->charInd = lexerState->charInd + 2;
        while(!(lexerState->sourceCode[lexerState->charInd] == '*' && lexerState->sourceCode[lexerState->charInd + 1] == '/')){
            lexerState->charInd ++;
        }
        lexerState->charInd = lexerState->charInd + 2;
        return;
    }
    
    //2 symbol case - assign 2 symbol token to token list
    //if second character is not symbol, assign only first symbol to token list
    if(lexerState->sourceCode[lexerState->charInd] == '<'){
        if(lexerState->sourceCode[lexerState->charInd + 1] == '>'){
            //Token 10
            new.id = 10;
            strcpy(new.lexeme, tokens[10]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd = lexerState->charInd + 2;
            return;
        }
        else if(lexerState->sourceCode[lexerState->charInd + 1] == '='){
            //Token 12
            new.id = 12;
            strcpy(new.lexeme, tokens[12]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd = lexerState->charInd + 2;
            return;
        }
        else if(isSpecialSymbol(lexerState->sourceCode[lexerState->charInd + 1]){
            lexerState->lexerError = INV_SYM;
            return;
        }
        else{
            //Token 11
            new.id = 11;
            strcpy(new.lexeme, tokens[11]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            return;
        }
    }
    else if(lexerState->sourceCode[lexerState->charInd] == '>'){
        if(lexerState->sourceCode[lexerState->charInd + 1] == '='){
            //Token 14
            new.id = 14;
            strcpy(new.lexeme, tokens[14]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd = lexerState->charInd + 2;
            return;
        }
        else if(isSpecialSymbol(lexerState->sourceCode[lexerState->charInd + 1]){
            lexerState->lexerError = INV_SYM;
            return;
        }
        else{
            //Token 13
            new.id = 13;
            strcpy(new.lexeme, tokens[13]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            return;
        }
    }
    else if(lexerState->sourceCode[lexerState->charInd] == ':'){
        if(lexerState->sourceCode[lexerState->charInd + 1] == '='){
            //Token 20
            new.id = 20;
            strcpy(new.lexeme, tokens[20]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd = lexerState->charInd + 2;
            return;
        }
        else if(isSpecialSymbol(lexerState->sourceCode[lexerState->charInd + 1]){
            lexerState->lexerError = INV_SYM;
            return;
        }
        else{
            lexerState->lexerError = INV_SYM;
            return;
        }
    }
    
    //single comment case
    //once correct symbol obtained assign to token list
    //at end if symbol not defined, then it's INVALID
    char c = lexerState->sourceCode[lexerState->charInd];
    switch(c){
        case '+':
            //Token 4
            new.id = 4;
            strcpy(new.lexeme, tokens[4]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case '-':
            //Token 5
            new.id = 5;
            strcpy(new.lexeme, tokens[5]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case '(':
            //Token 15
            new.id = 15;
            strcpy(new.lexeme, tokens[15]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case ')':
            //Token 16
            new.id = 16;
            strcpy(new.lexeme, tokens[16]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case '*':
            //Token 6
            new.id = 6;
            strcpy(new.lexeme, tokens[6]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case '/':
            //Token 7
            new.id = 7;
            strcpy(new.lexeme, tokens[7]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case ';':
            //Token 18
            new.id = 18;
            strcpy(new.lexeme, tokens[18]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case '.':
            //Token 19
            new.id = 19;
            strcpy(new.lexeme, tokens[19]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case ',':
            //Token 17
            new.id = 17;
            strcpy(new.lexeme, tokens[17]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        case '=':
            //Token 9
            new.id = 9;
            strcpy(new.lexeme, tokens[9]);
            addToken(&lexerState->tokenList, new);
            lexerState->charInd ++;
            break;
        default:
            lexerState->lexerError = INV_SYM;
            break;
    }
    
    return;
}

LexerOut lexicalAnalyzer(char* sourceCode){
    if(!sourceCode){
        fprintf(stderr, "ERROR: Null source code string passed to lexicalAnalyzer()\n");
        
        LexerOut lexerOut;
        lexerOut.lexerError = NO_SOURCE_CODE;
        lexerOut.errorLine = -1;

        return lexerOut;
    }

    // Create & init lexer state
    LexerState lexerState;
    initLexerState(&lexerState, sourceCode);

    // While not end of file, and, there is no lexer error
    // .. continue lexing
    while( lexerState.sourceCode[lexerState.charInd] != '\0' && lexerState.lexerError == NONE ){
        char currentSymbol = lexerState.sourceCode[lexerState.charInd];

        // Skip spaces or new lines until an effective character is seen
        while(currentSymbol == ' ' || currentSymbol == '\n')
        {
            // Advance line number if required
            if(currentSymbol == '\n')
                lexerState.lineNum++;

            // Advance to the following character
            currentSymbol = lexerState.sourceCode[++lexerState.charInd];
        }

        // After recognizing spaces or new lines, make sure that the EOF was
        // .. not reached. If it was, break the loop.
        if(lexerState.sourceCode[lexerState.charInd] == '\0'){
            break;
        }

        // Take action depending on the current symbol's type
        switch(getSymbolType(currentSymbol)){
            case ALPHA:
                DFA_Alpha(&lexerState);
                break;
            case DIGIT:
                DFA_Digit(&lexerState);
                break;
            case SPECIAL:
                DFA_Special(&lexerState);
                break;
            case INVALID:
                lexerState.lexerError = INV_SYM;
                break;
        }
    }

    // Prepare LexerOut to be returned
    LexerOut lexerOut;

    if(lexerState.lexerError != NONE){
        // Set LexErr
        lexerOut.lexerError = lexerState.lexerError;

        // Set the number of line the error encountered
        lexerOut.errorLine = lexerState.lineNum;

        lexerOut.tokenList = lexerState.tokenList;
    }
    else{
        // No error!
        lexerOut.lexerError = NONE;
        lexerOut.errorLine = -1;
        
        // Copy the token list

        // The scope of LexerState ends here. The ownership of the tokenlist
        // .. is being passed to LexerOut. Therefore, neither deletion of the
        // .. tokenlist nor deep copying of the tokenlist is required.
        lexerOut.tokenList = lexerState.tokenList;
    }

    return lexerOut;
}
