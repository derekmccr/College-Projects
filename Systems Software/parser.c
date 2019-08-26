/* 
 * File:   parser.c
 * Author: Derek McCrae
 * Assignment: COP 3402 - Assignment 3: Parser
 * Created on October 29, 2018, 9:44 AM
 */

#include "token.h"
#include "data.h"
#include "symbol.h"
#include <string.h>
#include <stdlib.h>

/**
 * This pointer is set when by parser() func and used by printParsedToken() func.
 * 
 * You are not required to use it anywhere. The implemented part of the skeleton
 * handles the printing. Instead, You could use the supplied helper functions to
 * manipulate the output file.
 * */
FILE* _out;

/**
 * Token list iterator used by the parser. It will be set once entered to parser()
 * and reset before exiting parser().
 * 
 * It is better to use the given helper functions to make use of token list iterator.
 * */
TokenListIterator _token_list_it;

/**
 * Current level.
 * */
unsigned int currentLevel;

/**
 * Symbol table.
 * */
SymbolTable symbolTable;

/**
 * Returns the current token using the token list iterator.
 * If it is the end of tokens, returns token with id nulsym.
 * */
Token getCurrentToken();

/**
 * Returns the type of the current token. Returns nulsym if it is the end of tokens.
 * */
int getCurrentTokenType();

/**
 * Prints the given token on _out by applying required formatting.
 * */
void printCurrentToken();

/**
 * Advances the position of TokenListIterator by incrementing the current token
 * index by one.
 * */
void nextToken();

/**
 * Given an entry from non-terminal enumaration, prints it.
 * */
void printNonTerminal(NonTerminal nonTerminal);

/**
 * Functions used for non-terminals of the grammar
 * */
int program();
int block();
int const_declaration();
int var_declaration();
int proc_declaration();
int statement();
int condition();
int relop();
int expression();
int term();
int factor();

Token getCurrentToken()
{
    return getCurrentTokenFromIterator(_token_list_it);
}

int getCurrentTokenType()
{
    return getCurrentToken().id;
}

void printCurrentToken()
{
    fprintf(_out, "%8s <%s, '%s'>\n", "TOKEN  :", tokenNames[getCurrentToken().id], getCurrentToken().lexeme);
}

void nextToken()
{
    _token_list_it.currentTokenInd++;
}

void printNonTerminal(NonTerminal nonTerminal)
{
    fprintf(_out, "%8s %s\n", "NONTERM:", nonTerminalNames[nonTerminal]);
}

/**
 * Given the parser error code, prints error message on file by applying
 * required formatting.
 * */
void printParserErr(int errCode, FILE* fp)
{
    if(!fp) return;

    if(!errCode)
        fprintf(fp, "\nPARSING WAS SUCCESSFUL.\n");
    else
        fprintf(fp, "\nPARSING ERROR[%d]: %s.\n", errCode, parserErrorMsg[errCode]);
}

/**
 * Advertised parser function. Given token list, which is possibly the output of 
 * the lexer, parses the tokens. If encountered, return the error code.
 * 
 * Returning 0 signals successful parsing.
 * Otherwise, returns a non-zero parser error code.
 * */
int parser(TokenList tokenList, FILE* out)
{
    // Set output file pointer
    _out = out;

    /**
     * Create a token list iterator, which helps to keep track of the current
     * token being parsed.
     * */
    _token_list_it = getTokenListIterator(&tokenList);

    // Initialize current level to 0, which is the global level
    currentLevel = 0;

    // Initialize symbol table
    initSymbolTable(&symbolTable);

    // Write parsing history header
    fprintf(_out, "Parsing History\n===============\n");

    // Start parsing by parsing program as the grammar suggests.
    int err = program();

    // Print symbol table - if no error occured
    if(!err)
    {
        fprintf(_out, "\n\n");
        printSymbolTable(&symbolTable, _out);
    }

    // Reset output file pointer
    _out = NULL;

    // Reset the global TokenListIterator
    _token_list_it.currentTokenInd = 0;
    _token_list_it.tokenList = NULL;

    // Delete symbol table
    deleteSymbolTable(&symbolTable);

    // Return err code - which is 0 if parsing was successful
    return err;
}

int program(){
    //declare variable
    int err;
    
    printNonTerminal(PROGRAM);

    //continue with BLOCK
    err = block();
    if(err)
        return err;
    
    if(getCurrentTokenType() != periodsym)
        return 6;
    // Consume periodsym
    printCurrentToken(); // Printing the token is essential!
    nextToken(); // Go to the next token..

    return 0;
}

int block(){
    //declare variables
    int err, hold;
    
    printNonTerminal(BLOCK);

    //continue with CONSTANT
    err = const_declaration();
    if(err)
        return err;
    
    //continue with VAR
    err = var_declaration();
    if(err)
        return err;
    
    //continue with PROCEDURE
    err = proc_declaration();
    if(err)
        return err;
    
    //continue with STAETMENT
    err = statement();
    if(err)
        return err;

    return 0;
}

int const_declaration(){
    //declare structure
    Symbol current;
    
    printNonTerminal(CONST_DECLARATION);

    if(getCurrentTokenType() == constsym){
        // Consume constsym
        current.type = CONST;
        current.level = currentLevel;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //constsym must be followed by identsym
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != identsym)
            return 3;
        strcpy(current.name, getCurrentToken().lexeme);
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        if(getCurrentTokenType() != eqsym)
            return 2;
        //consume eqsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..

        //eqsym must be followed by identsym
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != numbersym)
            return 1;
        sscanf(getCurrentToken().lexeme, "%d", &current.value);
        addSymbol(&symbolTable, current);
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        while(getCurrentTokenType() == commasym){
            //consume commasym
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
            current.type = CONST;
            current.level = currentLevel;
            
            //commasym must be followed by identsym
            //if yes, consume
            //if no, return error
            if(getCurrentTokenType() != identsym)
                return 3;
            strcpy(current.name, getCurrentToken().lexeme);
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
            
            //identsym must be followed by eqsym
            //if yes, consume
            //if no, return error
            if(getCurrentTokenType() != eqsym)
                return 2;
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
            
            //eqsym must be followed by numbersym
            //if yes, consume
            //if no, return error
            if(getCurrentTokenType() != numbersym)
                return 1;
            sscanf(getCurrentToken().lexeme, "%d", &current.value);
            addSymbol(&symbolTable, current);
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
        }
        
        //need semicolon at end
        if(getCurrentTokenType() != semicolonsym)
            return 10;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }

    // Successful parsing.
    return 0;
}

int var_declaration(){
    //declare structure
    Symbol current;
    
    printNonTerminal(VAR_DECLARATION);

    if(getCurrentTokenType() == varsym){
        // Consume varsym
        current.type = VAR;
        current.level = currentLevel;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //varsym must be followed by identsym
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != identsym)
            return 3;
        strcpy(current.name, getCurrentToken().lexeme);
        addSymbol(&symbolTable, current);
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        while(getCurrentTokenType() == commasym){
            // Consume commasym
            current.type = VAR;
            current.level = currentLevel;
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
        
            //commasym must be followed by identsym
            //if yes, consume
            //if no, return error
            if(getCurrentTokenType() != identsym)
                return 3;
            strcpy(current.name, getCurrentToken().lexeme);
            addSymbol(&symbolTable, current);
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
        }
        
        //need semicolon at end
        if(getCurrentTokenType() != semicolonsym)
            return 4;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }
    return 0;
}

int proc_declaration(){
    //declare variables
    int err;
    Symbol current;
    
    printNonTerminal(PROC_DECLARATION);

    while(getCurrentTokenType() == procsym){
        // Consume procsym
        current.type = PROC;
        current.level = currentLevel;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //procsym must be followed by identsym
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != identsym)
            return 3;
        strcpy(current.name, getCurrentToken().lexeme);
        addSymbol(&symbolTable, current);
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //identsym must be followed by ;
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != semicolonsym)
            return 5;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue with BLOCK
        currentLevel++;
        err = block();
        if(err)
            return err;
        
        //BLOCK must be followed by ;
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != semicolonsym)
            return 5;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }
    return 0;
}

int statement(){
    //declare variable
    int err;
    
    printNonTerminal(STATEMENT);

    if(getCurrentTokenType() == identsym){
        // Consume identsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //BECOMESSYM expected
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != becomessym){
            return 7;
        }
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue with EXPRESSION
        err = expression();
        if(err)
            return err;
    }
    else if(getCurrentTokenType() == callsym){
        // Consume callsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //callsym must be followed by identsym
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != identsym)
            return 8;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }
    else if(getCurrentTokenType() == beginsym){
        // Consume beginsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    
        //continue with STATEMENT
        err = statement();
        if(err)
            return err;
        
        while(getCurrentTokenType() == semicolonsym){
            // Consume semicolonsym
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
            
            //continue with STATEMENT
            err = statement();
            if(err)
                return err;
        }
        // Consume endsym if present or return error
        if(getCurrentTokenType() != endsym)
            return 10;
	currentLevel--;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
    }
    else if(getCurrentTokenType() == ifsym){
        // Consume ifsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue with CONDITION
        err = condition();
        if(err)
            return err;
        
        //THEN expected next
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != thensym)
            return 9;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue with STATEMENT
        err = statement();
        if(err)
            return err;
        
        if(getCurrentTokenType() == elsesym){
            //consume elsesym
            printCurrentToken(); // Printing the token is essential!
            nextToken(); // Go to the next token..
            
            //continue with STATEMENT
            err = statement();
            if(err)
                return err;
        }
    }
    else if(getCurrentTokenType() == whilesym){
        // Consume whilesym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue with CONDITION
        err = condition();
        if(err)
            return err;
        
        //DO expected next
        //if yes, consume
        //if not, return error
        if(getCurrentTokenType() != dosym)
            return 11;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue with STATEMENT
        err = statement();
        if(err)
            return err;
    }
    else if(getCurrentTokenType() == readsym){
        // Consume readsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //readsym must be followed by identsym
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != identsym)
            return 3;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }
    else if(getCurrentTokenType() == writesym){
        // Consume writesym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //writesym must be followed by identsym
        //if yes, consume
        //if no, return error
        if(getCurrentTokenType() != identsym)
            return 3;
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }
    return 0;
}

int condition(){
    //declare variable
    int err;
    
    printNonTerminal(CONDITION);

    if(getCurrentTokenType() == oddsym){
        // Consume oddsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue with EXPRESSION
        err = expression();
        if(err)
            return err;
    }
    else{
        //continue with EXPRESSION
        err = expression();
        if(err)
            return err;
        
        err =  relop();
        if(err)
            return err;
        
        //continue with EXPRESSION
        err = expression();
        if(err)
            return err;
    }
    return 0;
}

int relop(){
    printNonTerminal(REL_OP);

    if(getCurrentTokenType() == eqsym || getCurrentTokenType() == leqsym || getCurrentTokenType() == lessym || getCurrentTokenType() == gtrsym || getCurrentTokenType() == geqsym
            || getCurrentTokenType() == neqsym){
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        return 0;
    }
    else
        return 12;
}

int expression(){
    //declare variable
    int err;
    
    printNonTerminal(EXPRESSION);

    if(getCurrentTokenType() == plussym || getCurrentTokenType() == minussym){
        // Consume plussym or minussym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }
    
    //continue by parsing TERM
    err = term();
    if(err)
        return err;
    
    while(getCurrentTokenType() == plussym || getCurrentTokenType() == minussym){
        // Consume plussym or minussym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue by parsing TERM
        err = term();
        if(err)
            return err;
    }

    return 0;
}

int term(){
    int err;
    printNonTerminal(TERM);

    //continue by parsing FACTOR
    err = factor();
    if(err)
        return err;
    
    while(getCurrentTokenType() == multsym || getCurrentTokenType() == slashsym){
        // Consume multsym or slashsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
        
        //continue by parsing FACTOR
        err = factor();
        if(err)
            return err;
    }

    return 0;
}

/**
 * The below function is left fully-implemented as a hint.
 * */
int factor()
{
    printNonTerminal(FACTOR);

    /**
     * There are three possibilities for factor:
     * 1) ident
     * 2) number
     * 3) '(' expression ')'
     * */

    // Is the current token a identsym?
    if(getCurrentTokenType() == identsym)
    {
        // Consume identsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..

        // Success
        return 0;
    }
    // Is that a numbersym?
    else if(getCurrentTokenType() == numbersym)
    {
        // Consume numbersym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..

        // Success
        return 0;
    }
    // Is that a lparentsym?
    else if(getCurrentTokenType() == lparentsym)
    {
        // Consume lparentsym
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..

        // Continue by parsing expression.
        int err = expression();

        /**
         * If parsing of expression was not successful, immediately stop parsing
         * and propagate the same error code by returning it.
         * */
        
        if(err) return err;

        // After expression, right-parenthesis should come
        if(getCurrentTokenType() != rparentsym)
        {
            /**
             * Error code 13: Right parenthesis missing.
             * Stop parsing and return error code 13.
             * */
            return 13;
        }

        // It was a rparentsym. Consume rparentsym.
        printCurrentToken(); // Printing the token is essential!
        nextToken(); // Go to the next token..
    }
    else
    {
        /**
          * Error code 24: The preceding factor cannot begin with this symbol.
          * Stop parsing and return error code 24.
          * */
        return 14;
    }

    return 0;
}
