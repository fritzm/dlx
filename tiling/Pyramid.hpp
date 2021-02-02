#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include <set>
#include <vector>

#include "Puzzle.hpp"

class PyrCell;
class Element;
class PyrPiece;


class Pyramid : public Puzzle
{
protected:

    virtual void init() override;
    virtual void print(std::vector<Element*>& solution) override;

private:

    void addPieceAspects(PyrPiece const& piece, std::set<PyrPiece>& aspects);

    void addAspectPlacements(
        PyrPiece const& aspect, 
        std::set<PyrCell> const& board, 
        std::vector<PyrPiece>& placements
    );

    void buildBoard(std::set<PyrCell>& board);
    bool aspectFilter(PyrPiece const& piece, int flip, int rot);
    bool placementFilter(PyrPiece const& aspect, PyrCell const& pos);

};

#endif // !defined(PYRAMID_HPP)
