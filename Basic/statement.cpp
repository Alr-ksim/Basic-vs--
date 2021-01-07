/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <cstring>
#include "statement.h"
#include "parser.h"
#include "../StanfordCPPLib/error.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement()
{
   /* Empty */
}

Statement::~Statement()
{
   /* Empty */
}

PRINTS::PRINTS(TokenScanner &scanner)
{
   exp = readE(scanner, 0);
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

PRINTS::~PRINTS()
{
   delete exp;
}

void PRINTS::execute(EvalState &state)
{
   cout << exp->eval(state) << endl;
}

LETS::LETS(TokenScanner &scanner)
{
   string commands[12] = {"REM", "LET", "PRINT", "INPUT", "GOTO", "IF", "END", "RUN", "CLEAR", "LIST", "QUIT", "HELP"};
   var = scanner.nextToken();
   for (int i = 0;i < 12;i++)
      if (var == commands[i])
         error("SYNTAX ERROR");   
   if (scanner.nextToken() != "=")
      error("SYNTAX ERROR");
   exp = readE(scanner, 0);
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

LETS::~LETS()
{
   delete exp;
}

void LETS::execute(EvalState &state)
{
   state.setValue(var, exp->eval(state));
}

REMS::REMS(TokenScanner &scanner)
{
   while (scanner.hasMoreTokens())
   {
      remark += scanner.nextToken();
      remark += " ";
   }
}

REMS::~REMS() {}

void REMS::execute(EvalState &state) {}

INPUTS::INPUTS(TokenScanner &scanner)
{
   var = scanner.nextToken();
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

INPUTS::~INPUTS()
{
   delete constantexp;
}

void INPUTS::execute(EvalState &state)
{
   string str;
   int value;
   while (true)
   {
      cout << " ? ";
      getline(cin, str);
      try
      {
         value = stringToInteger(str);
      }
      catch (ErrorException &ex)
      {
         cout << "INVALID NUMBER" << endl;
         continue;
      }
      constantexp = new ConstantExp(value);
      state.setValue(var, constantexp->eval(state));
      break;
   }
}

ENDS::ENDS(TokenScanner &scanner)
{
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

ENDS::~ENDS() {}

void ENDS::execute(EvalState &state)
{
   state.setLine(-1);
}

GOTOS::GOTOS(TokenScanner &scanner)
{
   string str = scanner.nextToken();
   try
   {
      nextline = stringToInteger(str);
   }
   catch (ErrorException &ex)
   {
      cout << "SYNTAX ERROR" << endl;
   }
   if (scanner.hasMoreTokens())
      error("Extraneous token " + scanner.nextToken());
}

GOTOS::~GOTOS() {}

void GOTOS::execute(EvalState &state)
{
   state.setLine(nextline);
}

IFS::IFS(TokenScanner &scanner)
{
   lexp = readE(scanner, 1);
   op = scanner.nextToken();
   rexp = readE(scanner, 1);
   string then = scanner.nextToken();
   if (then != "THEN")
      error("SYNTAX ERROR");
   nextline = stringToInteger(scanner.nextToken());
   if (scanner.hasMoreTokens())
      error("SYNTAX ERROR");
}

IFS::~IFS()
{
   delete lexp, rexp;
}

void IFS::execute(EvalState &state)
{
   int lval = lexp->eval(state);
   int rval = rexp->eval(state);
   if ((op == "<" && lval < rval) || (op == "=" && lval == rval) || (op == ">" && lval > rval))
      state.setLine(nextline);
}