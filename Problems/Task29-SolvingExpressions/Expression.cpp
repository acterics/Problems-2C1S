#include "stdafx.h"
#include "Expression.h"



void Expression::parseString(std::string inputString)
{
	std::string tempString;
	std::stack<std::string> tempStack;
	clearExpressionFromSpaces(inputString);

	for (unsigned int iterator = 0; iterator < inputString.length();)
	{
		if (isSymbol(inputString[iterator]))
		{
			tempString.clear();
			while (isSymbol(inputString[iterator]) || isNumber(inputString[iterator]))
			{
				tempString.push_back(inputString[iterator]);
				iterator++;
			}
			this->push_back(tempString);
		}
		if (isNumber(inputString[iterator]))
		{
			tempString.clear();
			while (isNumber(inputString[iterator]) || inputString[iterator] == '.')
			{
				tempString.push_back(inputString[iterator]);
				iterator++;
			}
			this->push_back(tempString);
		}
		if (inputString[iterator] == '(')
		{
			tempStack.push("(");
			iterator++;
		}
		if (inputString[iterator] == ')')
		{
			while (tempStack.top() != "(")
			{
				this->push_back(tempStack.top());
				tempStack.pop();
			}
			tempStack.pop();
			iterator++;
		}
		if (!scanOperator(inputString, iterator).empty())
		{
			while (!tempStack.empty())
			{
				if (operatorPriority(scanOperator(inputString, iterator)) <= operatorPriority(tempStack.top()))
				{
					this->push_back(tempStack.top());
					tempStack.pop();
				}
				else
					break;
			}
			tempStack.push(scanOperator(inputString, iterator));
			iterator++;
		}
	}
	while (!tempStack.empty())
	{
		this->push_back(tempStack.top());
		tempStack.pop();
	}
}

Expression::Expression(std::string filePath)
{
	std::string inputString;
	std::ifstream inputStream(filePath, std::ios::in);
	getline(inputStream, inputString);
	this->parseString(inputString);
	inputStream.close();
	
}


Expression::~Expression()
{
}

std::string Expression::calculateExpression()
{
	std::stack<std::string> tempStack;
	std::string firstOperand, secondOperand;
	for (unsigned int iterator = 0;iterator < this->size();iterator++)
	{

		if (isOperator((*this)[iterator]))
		{
			firstOperand = tempStack.top();
			tempStack.pop();
			secondOperand = tempStack.top();
			tempStack.pop();
			tempStack.push(calculateBinaryOperation(secondOperand, firstOperand, (*this)[iterator]));
		}
		else
			tempStack.push((*this)[iterator]);
	}
	return tempStack.top();
}

void Expression::clearExpressionFromSpaces(std::string &inputString)
{
	for (unsigned int iterator = 0; iterator < inputString.length(); iterator++)
	{
		if (inputString[iterator] == ' ')
		{
			inputString = inputString.substr(0, iterator) + inputString.substr(iterator + 1);
			iterator--;
		}
	}
}

bool Expression::isSymbol(char inputChar)
{
	if (((inputChar >= 'a') && (inputChar <= 'z')) ||
		((inputChar >= 'A') && (inputChar <= 'Z')))
		return true;
	return false;
}

bool Expression::isNumber(char inputChar)
{
	if ((inputChar >= '0') && (inputChar <= '9'))
		return true;
	return false;
}

std::string Expression::scanOperator(std::string inputString, int currentPosition)
{
	if (inputString[currentPosition] == '^')
		return "^";
	if (inputString[currentPosition] == '+')
		return "+";
	if (inputString[currentPosition] == '-')
		return "-";
	if (inputString[currentPosition] == '*')
		return "*";
	return "";
}

int Expression::operatorPriority(std::string inputOperator)
{
	if (inputOperator == "+" || inputOperator == "-")
		return 1;
	if (inputOperator == "*")
		return 2;
	if (inputOperator == "^")
		return 3;
	return -1;
}

void Expression::print()
{
	for (unsigned int iterator = 0; iterator < this->size(); iterator++)
	{
		std::cout << (*this)[iterator] << " ";
	}
	std::cout << std::endl;
}

bool Expression::isOperator(std::string inputExpression)
{

	if (inputExpression == "*") 
		return true; 
	if (inputExpression == "+")
		return true;
	if (inputExpression == "-")
		return true;
	if (inputExpression == "^")
		return true;
	return false;
}

std::string Expression::calculateBinaryOperation(std::string firstStringOperand, std::string secondStringOperand, std::string inputOperator)
{
	int   firstNumericalOperand = stringToNumber(firstStringOperand),
		secondNumericalOperand = stringToNumber(secondStringOperand),
		operationResult = 0;
	if (inputOperator == "+")
		operationResult = firstNumericalOperand + secondNumericalOperand;
	if (inputOperator == "-")
		operationResult = firstNumericalOperand - secondNumericalOperand;
	if (inputOperator == "*")
		operationResult = (firstNumericalOperand*secondNumericalOperand);
	if (inputOperator == "^")
		operationResult = pow(firstNumericalOperand, secondNumericalOperand);
	return numberToString(operationResult);
}

int Expression::stringToNumber(std::string inputString)
{
	int resultNumber;
	std::istringstream convertingStream(inputString);
	convertingStream >> resultNumber;
	return resultNumber;
}

std::string Expression::numberToString(int inputNumber)
{
	std::stringstream convertingStream;
	convertingStream << inputNumber;
	return convertingStream.str();
}



