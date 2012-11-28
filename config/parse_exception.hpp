#ifndef __PARSE_EXCEPTION_HPP__
#define __PARSE_EXCEPTION_HPP__

#include <stdexcept>

class ParseException: public std::exception
{
	public:
		explicit ParseException(char* msg): message(msg) {};
		explicit ParseException(std::string msg = ""): message(msg) {};
		virtual const char* what() const throw() 
		{
			return message.c_str();
		}
		virtual ~ParseException() throw() 
		{
		}
	private:
		std::string message;
};


#endif // __PARSE_EXCEPTION_HPP__
