// DemoSimpleInterpreter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include "lex_specs.h"

extern "C" FILE *yyin;  // yyin must be declared as extern "C".
extern "C" int yylex();
extern "C" int yylineno;
extern "C" char* yytext;
extern "C" void SkipAllTokensInLine();

class NameValue
{
public:
	NameValue()
	{
	}

	NameValue(const char* lpszName, int iValue) :
		strName(lpszName),
		value(iValue)
	{
	}

	std::string strName;
	int			value;
};

std::vector<NameValue> vecNVPairs;

std::vector<NameValue>::iterator GetNameValue(const std::string strName)
{
	std::vector<NameValue>::iterator theIterator;

	for (theIterator = vecNVPairs.begin(); theIterator != vecNVPairs.end(); theIterator++)
	{
		if ((*theIterator).strName == strName)
		{
			return theIterator;
		}
	}

	return vecNVPairs.end();
}

void SetNameValue(const NameValue nameValue)
{
	std::vector<NameValue>::iterator theIterator;
	int iValueTemp = 0;

	theIterator = GetNameValue(nameValue.strName);

	if (theIterator == vecNVPairs.end())
	{
		vecNVPairs.push_back(nameValue);
	}
	else
	{
		*theIterator = NameValue(nameValue);
	}
}

int main(int argc, char** argv)
{
	++argv, --argc;  /* skip over program name */
	if (argc > 0)
		fopen_s(&yyin, argv[0], "r");
	else
		yyin = stdin;

	int token;

	while (token = yylex())
	{
		switch (token)
		{
			case SET:
			{
				NameValue nameValue;

				token = yylex();
				if (token != IDENTIFIER)
				{
					printf("Syntax error in Line [%d]. Expecting an IDENTIFIER but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				nameValue.strName = yytext;

				token = yylex();
				if (token != ASSIGN)
				{
					printf("Syntax error in Line [%d]. Expecting an ASSIGN but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				token = yylex();
				if (token == IDENTIFIER)
				{
					// Get the NameValue associated with the next IDENTIFIER.
					std::vector<NameValue>::iterator theIterator = GetNameValue(std::string(yytext));

					if (theIterator == vecNVPairs.end())
					{
						printf("Identifier [%s] Not Found.\n", yytext);
						SkipAllTokensInLine();
						continue;
					}
					else
					{
						printf("Identifier [%s] Value [%d]\n", (*theIterator).strName.c_str(), (*theIterator).value);
						// Do the SET.
						nameValue.value = (*theIterator).value;
						SetNameValue(nameValue);
					}
				}
				else if (token == INTEGER)
				{
					// Do the SET.
					nameValue.value = atoi(yytext);
					SetNameValue(nameValue);
				}
				else
				{
					printf("Syntax error in Line [%d]. Expecting an IDENTIFIER or an INTEGER but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				printf("Identifier [%s] assigned to Value [%d]\n", nameValue.strName.c_str(), nameValue.value);

				break;
			}

			case GET:
			{
				//NameValue nameValue;
				token = yylex();
				if (token != IDENTIFIER)
				{
					printf("Syntax error in Line [%d]. Expecting an IDENTIFIER but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				//nameValue.strName = yytext;

				std::vector<NameValue>::iterator theIterator = GetNameValue(std::string(yytext));

				if (theIterator == vecNVPairs.end())
				{
					printf("Identifier [%s] Not Found.\n", yytext);
					SkipAllTokensInLine();
				}
				else
				{
					printf("Identifier [%s] Value [%d]\n", (*theIterator).strName.c_str(), (*theIterator).value);
				}

				break;
			}

			case ADDITION :
			{
				// Get the next token and make sure it is an IDENTIFIER.
				token = yylex();
				if (token != IDENTIFIER)
				{
					printf("Syntax error in Line [%d]. Expecting an IDENTIFIER but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				// Get the NameValue associated with IDENTIFIER.
				std::vector<NameValue>::iterator theIterator = GetNameValue(std::string(yytext));

				if (theIterator == vecNVPairs.end())
				{
					printf("Identifier [%s] Not Found.\n", yytext);
					SkipAllTokensInLine();
					continue;
				}
				else
				{
					printf("Identifier [%s] Value [%d]\n", (*theIterator).strName.c_str(), (*theIterator).value);
				}

				// Set nameValue to the discovered NameValue pair found in vecNVPairs.
				NameValue nameValue = (*theIterator);

				// Get the next token and make sure it is a COMMA.
				token = yylex();
				if (token != COMMA)
				{
					printf("Syntax error in Line [%d]. Expecting a COMMA but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				// Get the next token and make sure it is either an IDENTIFIER or a literal INTEGER value.
				token = yylex();
				if (token == IDENTIFIER)
				{
					// Get the NameValue associated with the next IDENTIFIER.
					std::vector<NameValue>::iterator theIterator = GetNameValue(std::string(yytext));

					if (theIterator == vecNVPairs.end())
					{
						printf("Identifier [%s] Not Found.\n", yytext);
						SkipAllTokensInLine();
						continue;
					}
					else
					{
						printf("Identifier [%s] Value [%d]\n", (*theIterator).strName.c_str(), (*theIterator).value);
						// Do the ADDITION.
						nameValue.value += (*theIterator).value;
						SetNameValue(nameValue);
					}
				}
				else if (token == INTEGER)
				{
					// Do the ADDITION.
					nameValue.value += atoi(yytext);
					SetNameValue(nameValue);
				}
				else 
				{
					printf("Syntax error in Line [%d]. Expecting an IDENTIFIER or an INTEGER but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				printf("Identifier [%s] Value [%d] after ADDITION.\n", nameValue.strName.c_str(), nameValue.value);

				break;
			}

			case SUBTRACTION :
			{
				// Get the next token and make sure it is an IDENTIFIER.
				token = yylex();
				if (token != IDENTIFIER)
				{
					printf("Syntax error in Line [%d]. Expecting an IDENTIFIER but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				// Get the NameValue associated with IDENTIFIER.
				std::vector<NameValue>::iterator theIterator = GetNameValue(std::string(yytext));

				if (theIterator == vecNVPairs.end())
				{
					printf("Identifier [%s] Not Found.\n", yytext);
					SkipAllTokensInLine();
					continue;
				}
				else
				{
					printf("Identifier [%s] Value [%d]\n", (*theIterator).strName.c_str(), (*theIterator).value);
				}

				// Set nameValue to the discovered NameValue pair found in vecNVPairs.
				NameValue nameValue = (*theIterator);

				// Get the next token and make sure it is a COMMA.
				token = yylex();
				if (token != COMMA)
				{
					printf("Syntax error in Line [%d]. Expecting a COMMA but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				// Get the next token and make sure it is either an IDENTIFIER or a literal INTEGER value.
				token = yylex();
				if (token == IDENTIFIER)
				{
					// Get the NameValue associated with the next IDENTIFIER.
					std::vector<NameValue>::iterator theIterator = GetNameValue(std::string(yytext));

					if (theIterator == vecNVPairs.end())
					{
						printf("Identifier [%s] Not Found.\n", yytext);
						SkipAllTokensInLine();
						continue;
					}
					else
					{
						printf("Identifier [%s] Value [%d]\n", (*theIterator).strName.c_str(), (*theIterator).value);
						// Do the ADDITION.
						nameValue.value -= (*theIterator).value;
						SetNameValue(nameValue);
					}
				}
				else if (token == INTEGER)
				{
					// Do the ADDITION.
					nameValue.value -= atoi(yytext);
					SetNameValue(nameValue);
				}
				else
				{
					printf("Syntax error in Line [%d]. Expecting an IDENTIFIER or an INTEGER but found [%s]\n", yylineno, yytext);
					SkipAllTokensInLine();
					continue;
				}

				printf("Identifier [%s] Value [%d] after SUBTRACTION.\n", nameValue.strName.c_str(), nameValue.value);

				break;
			}

			case EXIT:
			{
				return 0;
			}

			case NEWLINE:
			{
				break;
			}

			default:
			{
				printf("Syntax Error. Unrecognized item : [%s]\n", yytext);
				SkipAllTokensInLine();
				break;
			}
		}
	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
