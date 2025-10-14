#ifndef SMPPROUTEREXCEPTIONS
#define SMPPROUTEREXCEPTIONS


#include <exception>
#include <string>


class NetworkException : public std::exception{
protected:
	std::string e;
public :
	NetworkException (char* str){e.assign(str);};
	~NetworkException () throw (){};
	const char* what(){
		return e.c_str();
	}

};


class TcpSocketException : public std::exception{
protected:
	std::string e;
public :
	TcpSocketException(char* str){e.assign(str);};
	~TcpSocketException() throw (){};
	const char* what(){
		return e.c_str();
	}
};


#endif
