#ifndef CELL_H
#define CELL_H

#include <sstream>
#include <string>

class Cell
{
public:

    int rOffset;
    int cOffset;

    bool operator<(Cell const& rhs) const
    {
        if (rOffset < rhs.rOffset) return true;
        if (rOffset > rhs.rOffset) return false;
        if (cOffset < rhs.cOffset) return true;
        if (cOffset > rhs.cOffset) return false;
        return false;
    }

    std::string Name() const
    {
        std::ostringstream str;
        str << rOffset << cOffset;
        return str.str();
    }

};

#endif // !defined(CELL_H)
