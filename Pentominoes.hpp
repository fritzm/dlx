#ifndef PENTOMINOES_H
#define PENTOMINOES_H

#include <set>
#include <vector>

class Cell;
class Element;
class Matrix;
class Piece;


class Pentominoes
{
public:

    virtual ~Pentominoes();

    void solve(bool countOnly);

protected:

    void init(Matrix& matrix, std::vector<Piece> const& pieces);

    void addPieceAspects(Piece const& piece, std::set<Piece>& aspects);

    void addAspectPlacements(
        Piece const& aspect, 
        std::set<Cell> const& board, 
        std::vector<Piece>& placements
    );

    virtual void buildBoard(std::set<Cell>& board) = 0;
    virtual bool aspectFilter(Piece const& piece, int flip, int rot);
    virtual bool placementFilter(Piece const& aspect, Cell const& pos);

};


class Pentominoes6x10 : public Pentominoes
{
protected:

    void buildBoard(std::set<Cell>& board) override;
    bool placementFilter(Piece const& aspect, Cell const& pos) override;

};


class Pentominoes5x12 : public Pentominoes
{
protected:

    void buildBoard(std::set<Cell>& board) override;
    bool aspectFilter(Piece const& piece, int flip, int rot) override;

};


class Pentominoes4x15 : public Pentominoes
{
protected:

    void buildBoard(std::set<Cell>& board) override;
    bool aspectFilter(Piece const& piece, int flip, int rot) override;

};


class Pentominoes3x20 : public Pentominoes
{
protected:

    void buildBoard(std::set<Cell>& board) override;
    bool aspectFilter(Piece const& piece, int flip, int rot) override;

};

#endif // !defined(PENTOMINOES_H)
