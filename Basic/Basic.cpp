/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);

void linecommand(TokenScanner &scanner, Program &program, EvalState &state, string line, int lineNumber);

void singlecommand(TokenScanner &scanner, Program &program, EvalState &state, string line, string commands);

/* Main program */

int main()
{
   EvalState state;
   Program program;

   while (true)
   {
      try
      {
         processLine(getLine(), program, state);
      }
      catch (ErrorException &ex)
      {
         cout << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */
void linecommand(TokenScanner &scanner, Program &program, EvalState &state, string line, int lineNumber){
      string commandz = scanner.nextToken();

      if (commandz == "")
         program.removeSourceLine(lineNumber);

      else if (commandz == "PRINT")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new PRINTS(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (commandz == "LET")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new LETS(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (commandz == "REM")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new REMS(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (commandz == "INPUT")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new INPUTS(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (commandz == "END")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new ENDS(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (commandz == "GOTO")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new GOTOS(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else if (commandz == "IF")
      {
         program.addSourceLine(lineNumber, line);
         Statement *stmt = new IFS(scanner);
         program.setParsedStatement(lineNumber, stmt);
      }
      else
         error("SYNTAX ERROR");
}

void singlecommand(TokenScanner &scanner, Program &program, EvalState &state, string line, string commands){
      if (commands == "LIST")
      {
         int linenumber = program.getFirstLineNumber();
         while (linenumber != -1)
         {
            cout << program.getSourceLine(linenumber) << endl;
            linenumber = program.getNextLineNumber(linenumber);
         }
      }
      else if (commands == "RUN")
      {
         int linenumber = program.getFirstLineNumber();
         while (linenumber != -1)
         {
            if (program.getSourceLine(linenumber) == "")
               error("LINE NUMBER ERROR");
            state.setLine(program.getNextLineNumber(linenumber));
            Statement *stmt = program.getParsedStatement(linenumber);
            stmt->execute(state);
            linenumber = state.getLine();
         }
      }
      else if (commands == "QUIT")
         exit(0);
      else if (commands == "CLEAR")
      {
         program.clear();
         state.clear();
      }
      else if (commands == "PRINT")
      {
         Statement *stmt = new PRINTS(scanner);
         stmt->execute(state);
         delete stmt;
      }
      else if (commands == "LET")
      {
         Statement *stmt = new LETS(scanner);
         stmt->execute(state);
         delete stmt;
      }
      else if (commands == "INPUT")
      {
         Statement *stmt = new INPUTS(scanner);
         stmt->execute(state);
         delete stmt;
      }
      else if (commands == "HELP")
         cout << "Stub implementation of BASIC" << endl;
      else
         error("SYNTAX ERROR");
}

void processLine(string line, Program &program, EvalState &state)
{
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);

   string commands = scanner.nextToken();

   if (commands[0] >= '0' && commands[0] <= '9')
   {
      int lineNumber = stringToInteger(commands);
      linecommand(scanner, program, state, line, lineNumber);
   }
   else
   {
      singlecommand(scanner, program, state, line, commands);
   }
}
