#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <vector>

#include "Cell.hpp"

class Piece
{
public:

    std::string name;
    std::vector<Cell> cells;

    bool operator<(Piece const& rhs) const
    {
        if (name < rhs.name) return true;
        if (name > rhs.name) return false;
        if (cells < rhs.cells) return true;
        if (cells > rhs.cells) return false;
        return false;
    }

};

#endif // !defined(PIECE_H)
