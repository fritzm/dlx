#include "Header.hpp"


Header::Header(std::string const& name)
:
    count(0), name(name)
{
    col = this;
}
