#ifndef HEADER_H
#define HEADER_H

#include <string>

#include "Element.hpp"

class Header : public Element
{
public:

    Header(std::string const& name);

    int count;
    std::string name;   

};

#endif // !defined(HEADER_H)