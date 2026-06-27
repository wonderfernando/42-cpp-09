#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

RPN::RPN() {}

RPN::RPN(RPN const &other) : _stack(other._stack) {}

RPN &RPN::operator=(RPN const &other)
{
	if (this != &other)
		_stack = other._stack;
	return *this;
}

RPN::~RPN() {}

static bool isOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

static int applyOp(int a, int b, char op)
{
	switch (op)
	{
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/':
		if (b == 0)
			throw std::runtime_error("Error: division by zero");
		return a / b;
	}
	return 0;
}

int RPN::evaluate(std::string const &expression)
{
	while (!_stack.empty())
		_stack.pop();

	std::istringstream iss(expression);
	std::string token;

	while (iss >> token)
	{
		if (token.length() == 1 && isOperator(token[0]))
		{
			if (_stack.size() < 2)
				throw std::runtime_error("Error: insufficient operands");
			int b = _stack.top();
			 _stack.pop();
			int a = _stack.top(); 
			_stack.pop();
			_stack.push(applyOp(a, b, token[0]));
		}
		else if (token.length() == 1 && std::isdigit(token[0]))
		{ 
			_stack.push(token[0] - '0');
		}
		else 
		{
			throw std::runtime_error("Error: invalid token");
		}
	}

	if (_stack.size() != 1)
		throw std::runtime_error("Error: invalid expression");

	return _stack.top();
}
