#include "iString.h"

#include <stdio.h>
#include <string.h>

iString::iString()
{
	//printf("iString()\n");
	s = NULL;
}

iString::iString(const iString& s)
{
	//printf("iString(const string& s)\n");
	int len = strlen(s.s);
	char* t = new char[len + 1];
	strcpy(t, s.s);

	this->s = t;
}

iString::iString(const char* s)
{
	//printf("iString(const char* s)\n");
	int len = strlen(s);
	char* t = new char[len + 1];
	strcpy(t, s);

	this->s = t;
}

iString::~iString()
{
	//printf("~iString() %s\n", s);
	if (s)
		delete s;
}

iString iString::operator+(const char* s)
{
	int len0 = strlen(this->s);
	int len1 = strlen(s);
	char* t = new char[len0 + len1 + 1];
	strcpy(t, this->s); // 원래 문자열 복사
	strcpy(&t[len0], s); // 원래 문자열 뒤에 파라미터로 받은 문자열 복사

	iString r(t);

	return r;
}

iString& iString::operator+=(const char* s)
{
	int len0 = strlen(this->s);
	int len1 = strlen(s);
	char* t = new char[len0 + len1 + 1];
	strcpy(t, this->s); // 원래 문자열 복사
	strcpy(&t[len0], s); // 원래 문자열 뒤에 파라미터로 받은 문자열 복사

	if (this->s)
		delete this->s;
	this->s = t;

	return *this;
}

iString& iString::operator+=(const iString& s)
{
	int len0 = strlen(this->s);
	int len1 = strlen(s.s);
	char* t = new char[len0 + len1 + 1];
	strcpy(t, this->s); // 원래 문자열 복사
	strcpy(&t[len0], s.s); // 원래 문자열 뒤에 파라미터로 받은 문자열 복사

	if (this->s)
		delete this->s;
	this->s = t;

	return *this;
}

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
	int len = strlen(s.s);
	char* t = new char[len + 1];
	strcpy(t, s.s);

	if (this->s)
		delete this->s;
	this->s = t;
	return *this;
}

iString& iString::operator=(const char* s)
{
	int len = strlen(s);
	char* t = new char[len + 1];
	strcpy(t, s);

	if (this->s)
		delete this->s;
	this->s = t;
	return *this;
}

