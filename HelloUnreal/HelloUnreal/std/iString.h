#pragma once

struct iString
{
    char* s;

    bool operator==(const iString& s);
    bool operator==(const char* s);

    iString& operator=(const iString& s);
    iString& operator=(const char* s);

    iString& operator+=(const iString& s);
    iString& operator+=(const char* s);
};

