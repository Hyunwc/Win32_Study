#include "iString.h"

#include <string.h>

bool iString::operator==(const iString& s)
{
	if (strcmp(this->s, s.s))
		return false;
	return true;
}

bool iString::operator==(const char* s)
{
	if (strcmp(this->s, s))
		return false;
	return true;
}

iString& iString::operator=(const iString& s)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

iString& iString::operator=(const char* s)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

iString& iString::operator+=(const iString& s)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

iString& iString::operator+=(const char* s)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}
