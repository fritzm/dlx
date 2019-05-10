#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>

#include "polyos.hpp"

#include "Element.hpp"
#include "Matrix.hpp"

using namespace std;

class Cell
{
public:

    int rOffset;
    int cOffset;

    Cell(int row, int col)
    :   rOffset(row), cOffset(col)
    {
    }

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
        str << std::setfill('0') << std::setw(2) << rOffset << std::setw(2) << cOffset;
        return str.str();
    }

};


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


vector<Piece> pieces = {
    {"F", {{0,1}, {0,2}, {1,0}, {1,1}, {2,1}}},
    {"I", {{0,0}, {1,0}, {2,0}, {3,0}, {4,0}}},
    {"L", {{0,0}, {1,0}, {2,0}, {3,0}, {3,1}}},
    {"N", {{0,0}, {0,1}, {1,1}, {1,2}, {1,3}}},
    {"P", {{0,0}, {0,1}, {1,0}, {1,1}, {2,0}}},
    {"T", {{0,0}, {0,1}, {0,2}, {1,1}, {2,1}}},
    {"U", {{0,0}, {1,0}, {1,1}, {1,2}, {0,2}}},
    {"V", {{0,0}, {1,0}, {2,0}, {2,1}, {2,2}}},
    {"W", {{0,0}, {1,0}, {1,1}, {2,1}, {2,2}}},
    {"X", {{0,1}, {1,0}, {1,1}, {1,2}, {2,1}}},
    {"Y", {{0,2}, {1,0}, {1,1}, {1,2}, {1,3}}},
    {"Z", {{0,0}, {0,1}, {1,1}, {2,1}, {2,2}}}
};


void rectBoard(set<Cell> &board, int rows, int cols)
{
    for(auto r=0; r<rows; ++r) {
        for(auto c=0; c<cols; ++c) {
            board.insert(Cell(r, c));
        }
    }
}


class Config 
{
public:

    virtual void buildBoard(set<Cell> &board) = 0;
    virtual bool aspectFilter(Piece const& piece, int flip, int rot) = 0;
    virtual bool placementFilter(Piece const& aspect, Cell const& pos) = 0;

};


class pentominoes6x10 : public Config
{
public:

    void buildBoard(set<Cell> &board) override
    {
        rectBoard(board, 6, 10);
    }

    bool aspectFilter(Piece const& piece, int flip, int rot) override
    {
        return false;
    }

    bool placementFilter(Piece const& aspect, Cell const& pos) override
    {
        return (aspect.name == "X" && ((pos.rOffset > 1) || (pos.cOffset > 3)));
    }

};


class pentominoes5x12 : public Config
{
public:

    void buildBoard(set<Cell> &board) override
    {
        rectBoard(board, 5, 12);
    }

    bool aspectFilter(Piece const& piece, int flip, int rot) override
    {
        return ((piece.name == "F") && ((flip == 1) || (rot > 1)));
    }

    bool placementFilter(Piece const& aspect, Cell const& pos) override
    {
        return false;
    }

};


class pentominoes4x15 : public Config
{
public:

    void buildBoard(set<Cell> &board) override
    {
        rectBoard(board, 4, 15);
    }

    bool aspectFilter(Piece const& piece, int flip, int rot) override
    {
        return ((piece.name == "F") && ((flip == 1) || (rot > 1)));
    }

    bool placementFilter(Piece const& aspect, Cell const& pos) override
    {
        return false;
    }

};


class pentominoes3x20 : public Config
{
public:

    void buildBoard(set<Cell> &board) override
    {
        rectBoard(board, 3, 20);
    }

    bool aspectFilter(Piece const& piece, int flip, int rot) override
    {
        return ((piece.name == "F") && ((flip == 1) || (rot > 1)));
    }

    bool placementFilter(Piece const& aspect, Cell const& pos) override
    {
        return false;
    }

};


map<p5BoardType, Config *> configs = {
    { p5BoardType::P5_6X10, new pentominoes6x10() },
    { p5BoardType::P5_5X12, new pentominoes5x12() },
    { p5BoardType::P5_4X15, new pentominoes4x15() },
    { p5BoardType::P5_3X20, new pentominoes3x20() }
};


void addPieceAspects(Config *config, Piece const& piece, set<Piece> & aspects)
{
    auto aspect = piece;
    for(int flip=0; flip<2; ++flip) {
        for(int rot=0; rot<4; ++rot) {
            if (config->aspectFilter(piece, flip, rot)) continue;
            sort(aspect.cells.begin(), aspect.cells.end());
            int minrow = numeric_limits<int>::max();
            int mincol = numeric_limits<int>::max();
            for(auto const& cell: aspect.cells) {
                minrow = min(minrow, cell.rOffset);
                mincol = min(mincol, cell.cOffset);
            }
            for(auto & cell: aspect.cells) {
                cell.rOffset -= minrow;
                cell.cOffset -= mincol;
            }
            aspects.insert(aspect);
            for(auto & cell: aspect.cells) {
                auto temp = cell.rOffset;
                cell.rOffset = cell.cOffset;
                cell.cOffset = -temp;
            }
        }
        for(auto & cell: aspect.cells) {
            cell.rOffset = -cell.rOffset;
        }
    }
}


void addAspectPlacements(
    Config *config,
    Piece const& aspect, 
    set<Cell> const& board, 
    vector<Piece> & placements)
{
    for(auto const& pos: board) {
        auto placement = aspect;
        bool fit = true;
        for(auto & cell: placement.cells) {
            cell.rOffset += pos.rOffset;
            cell.cOffset += pos.cOffset;
            if (board.count(cell) == 0) {
                fit = false;
                break;
            }
        }    
        if (fit & !config->placementFilter(aspect, pos)) {
            placements.push_back(placement);
        }
    }
}


void init(Matrix &matrix, vector<Piece> const& pieces, p5BoardType boardType)
{
    auto config = configs[boardType];

    set<Piece> aspects;
    for(auto const& piece: pieces) {
        addPieceAspects(config, piece, aspects);
    }

    set<Cell> board;
    config->buildBoard(board);

    vector<Piece> placements;
    for(auto const& aspect: aspects) {
        addAspectPlacements(config, aspect, board, placements);
    }

    for(auto const& placement: placements) {
        auto c = matrix.findColumn(placement.name);
        auto e = new Element();
        e->InsertUD(c);
        for(auto const& cell: placement.cells) {
            ostringstream cname;
            cname << setfill('0') << setw(2) << cell.rOffset << setw(2) << cell.cOffset;
            auto c2 = matrix.findColumn(cname.str());
            auto e2 = new Element();
            e2->InsertUD(c2);
            e2->InsertLR(e);
        }
    }
}


void print(vector<Element *> &solution)
{
    static int count = 0;

    static string corner[16] = {
        "\u0020", "\u2575", "\u2574", "\u2518",
        "\u2576", "\u2514", "\u2500", "\u2534",
        "\u2577", "\u2502", "\u2510", "\u2524",
        "\u250c", "\u251c", "\u252c", "\u253c"
    };

    map<Cell, string> board;
    int rmax = 0;
    int cmax = 0;

    for(auto &re: solution) {
        auto pe = re;
        for(; pe->col->name.length() > 1; pe=pe->l);
        for(auto ce=pe->r; ce!=pe; ce=ce->r) {
            int r = stoi(ce->col->name.substr(0,2)) + 1;
            int c = stoi(ce->col->name.substr(2,2)) + 1;
            rmax = max(rmax, r);
            cmax = max(cmax, c);
            board[Cell(r,c)] = pe->col->name;
        }
    }

    cout << "#" << ++count << ":" << endl;
    for(int r=1; r<=rmax+1; ++r) {
        for(int c=1; c<=cmax+1; ++c) {
            cout << corner[
                ((board[Cell(r-1, c-1)] != board[Cell(r-1, c  )]) ? 1 : 0) +
                ((board[Cell(r-1, c-1)] != board[Cell(r,   c-1)]) ? 2 : 0) +
                ((board[Cell(r-1, c  )] != board[Cell(r,   c  )]) ? 4 : 0) +
                ((board[Cell(r,   c-1)] != board[Cell(r,   c  )]) ? 8 : 0)
            ];
            cout << ((board[Cell(r-1, c)] != board[Cell(r, c)]) ? "\u2500\u2500\u2500" : "\u0020\u0020\u0020");
        }
        cout << endl;
        for(int c=1; c<=cmax+1; ++c) {
            cout << ((board[Cell(r, c-1)] != board[Cell(r, c)]) ? "\u2502\u0020\u0020\u0020" : "\u0020\u0020\u0020\u0020");            
        }
        cout << endl;
    }
}


void dontPrint(vector<Element *> &solution)
{
}


void pentominoes(p5BoardType boardType, bool countOnly)
{
    Matrix matrix;
    init(matrix, pieces, boardType);
    int nodeCount = 0;
    int solutionCount = 0;
    matrix.findCovers(nodeCount, solutionCount, countOnly ? dontPrint : print);
    cout << solutionCount << " solutions, " << nodeCount << " nodes visted" << endl;
}
