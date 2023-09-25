#ifndef _TINY_STRING_
#define _TINY_STRING_

#include <iostream>
#include <string.h>

namespace tinyInfra {

class string
{
public:
    //Default ctor
    string(const char* c_str = NULL);
    //Copy ctor
    string(const string& str);
    //Assignment operator
    string& operator=(const string& str);
    //dtor
    ~string();
    //Get string size
    size_t size() const;
    size_t length() const;
    //Empty
    bool empty() const;
private:
    char* c_str_ {nullptr};
};

string::string(const char* c_str)
{
    if (nullptr != c_str)
    {
        size_t size = strlen(c_str);
        c_str_ = new char[size+1]();
        strncpy(c_str_, c_str, size);
    }
}

string::string(const string& str)
{
    if (nullptr != str.c_str_)
    {
        size_t size = strlen(str.c_str_);
        c_str_ = new char[size+1]();
        strncpy(c_str_, str.c_str_, size);
    }
}

string& string::operator=(const string& str)
{
    if (this == &str)
        return *this;

    delete[] c_str_;
    c_str_ = nullptr;
    size_t size = strlen(str.c_str_);
    c_str_ = new char[size+1]();
    strncpy(c_str_, str.c_str_, size);
    return *this;
}

string::~string()
{
    delete[] c_str_;
    c_str_ = nullptr;
}

size_t string::size() const
{
    if (nullptr != c_str_)
        return strlen(c_str_);
    return 0;
}

size_t string::length() const
{
    if (nullptr != c_str_)
        return strlen(c_str_);
    return 0;
}

bool string::empty() const
{
    return (0 == size());
}

}
#endif //_TINY_STRING_