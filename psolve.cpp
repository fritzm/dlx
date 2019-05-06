#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>

#include "Element.hpp"
#include "Header.hpp"
#include "Matrix.hpp"
#include "Piece.hpp"

using namespace std;


void addPieceAspects(Piece const& piece, set<Piece> & aspects)
{
    auto aspect = piece;
    for(int flip=0; flip<2; ++flip) {
        for(int rot=0; rot<4; ++rot) {
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
    Piece const& aspect, 
    set<Cell> const& board, 
    bool (filter)(Piece const& placement),
    vector<Piece> & placements)
{
    auto placement = aspect;
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
        if (fit & !filter(placement)) {
            placements.push_back(placement);
        }
    }
}


void init(
    Matrix &matrix,
    vector<Piece> const& pieces, 
    set<Cell> const& board,
    bool (filter)(Piece const& placement))
{
    set<Piece> aspects;
    for(auto const& piece: pieces) {
        addPieceAspects(piece, aspects);
    }

    vector<Piece> placements;
    for(auto const& aspect: aspects) {
        addAspectPlacements(aspect, board, filter, placements);
    }

    for(auto const& placement: placements) {
        auto c = matrix.findColumn(placement.name);
        auto e = new Element();
        e->InsertUD(c);
        for(auto const& cell: placement.cells) {
            ostringstream cname;
            cname << cell.rOffset << cell.cOffset;
            auto c2 = matrix.findColumn(cname.str());
            auto e2 = new Element();
            e2->InsertUD(c2);
            e2->InsertLR(e);
        }
    }
}


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


set<Cell> board = {
    {0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9},
    {1,0}, {1,1}, {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7}, {1,8}, {1,9},
    {2,0}, {2,1}, {2,2}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7}, {2,8}, {2,9},
    {3,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7}, {3,8}, {3,9},
    {4,0}, {4,1}, {4,2}, {4,3}, {4,4}, {4,5}, {4,6}, {4,7}, {4,8}, {4,9},
    {5,0}, {5,1}, {5,2}, {5,3}, {5,4}, {5,5}, {5,6}, {5,7}, {5,8}, {5,9}
};


bool filter(Piece const& placement)
{
    return (placement.name == "X" 
            && ((placement.cells[0].rOffset > 1) 
                || (placement.cells[0].cOffset > 4)));
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

    string board[8][12];
    for(auto &re: solution) {
        auto pe = re;
        for(; pe->col->name.length() > 1; pe=pe->l);
        for(auto ce=pe->r; ce!=pe; ce=ce->r) {
            int r = ce->col->name[0] - '0' + 1;
            int c = ce->col->name[1] - '0' + 1;
            board[r][c] = pe->col->name;
        }
    }

    cout << "#" << ++count << ":" << endl;
    for(int r=1; r<8; ++r) {
        for(int c=1; c<12; ++c) {
            cout << corner[
                ((board[r-1][c-1] != board[r-1][c]) ? 1 : 0)
                + ((board[r-1][c-1] != board[r][c-1]) ? 2 : 0)
                + ((board[r-1][c] != board[r][c]) ? 4 : 0)
                + ((board[r][c-1] != board[r][c]) ? 8 : 0)
            ];
            cout << ((board[r-1][c] != board[r][c]) ? "\u2500\u2500\u2500" : "\u0020\u0020\u0020");
        }
        cout << endl;
        for(int c=1; c<12; ++c) {
            cout << ((board[r][c-1] != board[r][c]) ? "\u2502\u0020\u0020\u0020" : "\u0020\u0020\u0020\u0020");            
        }
        cout << endl;
    }
}


int main(int argc, const char *argv[])
{
    Matrix matrix;
    init(matrix, pieces, board, filter);
    matrix.findCovers(print);
    return 0;
}
