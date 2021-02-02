#ifndef SOMA_HPP
#define SOMA_HPP

#include <set>
#include <vector>

#include "Puzzle.hpp"

class SomaCell;
class Element;
class SomaPiece;


class Soma : public Puzzle
{
protected:

    virtual void init() override;
    virtual void print(std::vector<Element*>& solution) override;

private:

    void addPieceAspects(SomaPiece const& piece, std::set<SomaPiece>& aspects);

    void addAspectPlacements(
        SomaPiece const& aspect, 
        std::set<SomaCell> const& board, 
        std::vector<SomaPiece>& placements
    );

    void buildBoard(std::set<SomaCell>& board);
    bool aspectFilter(SomaPiece const& piece, int flip, int rot);
    bool placementFilter(SomaPiece const& aspect, SomaCell const& pos);

};

#endif // !defined(SOMA_HPP)
