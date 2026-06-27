#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
 
class RPN
{ 
private:
	std::stack<int> _stack;

public:
	RPN();
	RPN(RPN const &other);
	RPN &operator=(RPN const &other);
	~RPN();

	int evaluate(std::string const &expression);
};

#endif
