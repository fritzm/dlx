#include <iomanip>
#include <iostream>
#include <map>

#include "Pyramid.hpp"

using namespace std;


class PyrCell
{
public:

    int xOffset;
    int yOffset;
    int zOffset;

    PyrCell(int x, int y, int z)
    :   xOffset(x), yOffset(y), zOffset(z)
    {
    }

    PyrCell(string const& name)
    {
        xOffset = stoi(name.substr(0,1));
        yOffset = stoi(name.substr(1,1));
        zOffset = stoi(name.substr(2,1));
    }

    bool operator<(PyrCell const& rhs) const
    {
        if (zOffset < rhs.zOffset) return true;
        if (zOffset > rhs.zOffset) return false;
        if (yOffset < rhs.yOffset) return true;
        if (yOffset > rhs.yOffset) return false;
        if (xOffset < rhs.xOffset) return true;
        if (xOffset > rhs.xOffset) return false;
        return false;
    }

    bool operator==(PyrCell const& rhs) const
    {
        if (zOffset != rhs.zOffset) return false;
        if (yOffset != rhs.yOffset) return false;
        if (xOffset != rhs.xOffset) return false;
        return true;
    }

    string name() const
    {
        return to_string(xOffset) + to_string(yOffset) + to_string(zOffset);
    }
};

ostream& operator<<(ostream &str, PyrCell const& cell)
{
    str << "(" << cell.xOffset << "," << cell.yOffset << "," << cell.zOffset << ")";
    return str;
};


class PyrPiece
{
public:

    string name;
    vector<PyrCell> cells;

    bool operator<(PyrPiece const& rhs) const
    {
        if (name < rhs.name) return true;
        if (name > rhs.name) return false;
        if (cells < rhs.cells) return true;
        if (cells > rhs.cells) return false;
        return false;
    }

    bool operator==(PyrPiece const& rhs) const
    {
        if (name != rhs.name) return false;
        if (cells != rhs.cells) return false;
        return true;
    }

};

ostream& operator<<(ostream& str, PyrPiece const& piece)
{
    str << piece.name << ": ";
    for(auto & cell : piece.cells) str << cell;
    return str;
};


namespace {

vector<PyrPiece> pieces = {
    {"A", {{0,0,0},{1,0,0},{2,0,0},{3,0,0}}},
    {"B", {{0,0,0},{1,0,0},{2,0,0}}},
    {"C", {{0,0,0},{1,0,0}}},
    {"D", {{1,0,0},{2,0,0},{3,0,0}}},
    {"E", {{0,0,0},{1,0,0},{0,1,0},{1,1,0},{1,1,1}}},
    {"F", {{0,0,0},{1,0,0},{0,1,0}}},
    {"G", {{1,1,0},{2,1,0},{1,1,1},{2,1,1}}},
    {"H", {{0,0,0},{0,1,0},{1,1,0}}},
    {"I", {{0,0,0},{0,1,0},{1,1,0},{1,1,1}}},
    {"J", {{0,0,0},{1,0,0},{1,1,0},{1,2,0}}}
};

} // anonymous namespace


void Pyramid::buildBoard(set<PyrCell>& board)
{
    for(auto z=0; z<=2; ++z) {
        for(auto y=z; y<=(4-z); ++y) {
            for(auto x=z; x<=(4-z); ++x) {
                board.insert(PyrCell(x, y, z));
            }
        }
    }
}


void Pyramid::addPieceAspects(PyrPiece const& piece, set<PyrPiece>& aspects)
{
    auto aspect = piece;
    for(int flip=0; flip<2; ++flip) {
        for(int rot=0; rot<4; ++rot) {
            if (aspectFilter(piece, flip, rot)) continue;
            sort(aspect.cells.begin(), aspect.cells.end());
            int minx = numeric_limits<int>::max();
            int miny = numeric_limits<int>::max();
            int minz = numeric_limits<int>::max();
            for(auto const& cell: aspect.cells) {
                minx = min(minx, cell.xOffset);
                miny = min(miny, cell.yOffset);
                minz = min(minz, cell.zOffset);
            }
            for(auto& cell: aspect.cells) {
                cell.xOffset -= minz;
                cell.yOffset -= minz;
                cell.zOffset -= minz;
            }
            minx -= minz;
            miny -= minz;
            for(auto& cell: aspect.cells) {
                cell.xOffset -= (minx & ~1);
                cell.yOffset -= (miny & ~1);
            }
            aspects.insert(aspect);
            for(auto& cell: aspect.cells) {
                auto temp = cell.xOffset;
                cell.xOffset = cell.yOffset;
                cell.yOffset = -temp;
            }
        }
        for(auto& cell: aspect.cells) {
            cell.xOffset = (-cell.xOffset) + 1;
            cell.yOffset = cell.yOffset + 1;
            cell.zOffset = -cell.zOffset;
        }
    }
}


void Pyramid::addAspectPlacements(
    PyrPiece const& aspect, 
    set<PyrCell> const& board, 
    vector<PyrPiece> & placements)
{
    for(auto const& pos: board) {
        if (((pos.xOffset - pos.zOffset) & 1) || ((pos.yOffset - pos.zOffset) & 1)) continue;
        auto placement = aspect;
        bool fit = true;
        for(auto& cell: placement.cells) {
            cell.xOffset += pos.xOffset;
            cell.yOffset += pos.yOffset;
            cell.zOffset += pos.zOffset;
            if (board.count(cell) == 0) {
                fit = false;
                break;
            }
        }    
        if (fit & !placementFilter(placement, pos)) {
            placements.push_back(placement);
        }
    }
}


bool Pyramid::aspectFilter(PyrPiece const& piece, int flip, int rot)
{
    return ((piece.name == "E") && ((rot > 0) || (flip > 0)));
}


bool Pyramid::placementFilter(PyrPiece const& aspect, PyrCell const& pos)
{
    return false;
}


void Pyramid::init()
{    
    subGoals.emplace_back();
    auto& sg = subGoals.back();

    set<PyrPiece> aspects;
    for(auto const& piece: pieces) {
        addPieceAspects(piece, aspects);
    }

    set<PyrCell> board;
    buildBoard(board);

    vector<PyrPiece> placements;
    for(auto const& aspect: aspects) {
        addAspectPlacements(aspect, board, placements);
    }

    for(auto const& placement: placements) {
        auto c = sg.matrix.findColumn(placement.name);
        auto e = new Element();
        e->insertUD(c);
        ++sg.elems;
        for(auto const& cell: placement.cells) {
            auto c2 = sg.matrix.findColumn(cell.name());
            auto e2 = new Element();
            e2->insertUD(c2);
            e2->insertLR(e);
            ++sg.elems;
            ++sg.rows;
        }
    }
}


void Pyramid::print(vector<Element *>& solution)
{
    static int count = 0;

    static string corner[16] = { // 0=up; 1=left; 2=right; 4=down
        "\u0020", "\u2575", "\u2574", "\u2518",
        "\u2576", "\u2514", "\u2500", "\u2534",
        "\u2577", "\u2502", "\u2510", "\u2524",
        "\u250c", "\u251c", "\u252c", "\u253c"
    };

    static auto printTop = [](map<PyrCell, string>& board, int x, int y, int z) {
        cout << corner[
            ((board[PyrCell(x-1, y+1, z)] != board[PyrCell(x,   y+1, z)]) ? 1 : 0) +
            ((board[PyrCell(x-1, y+1, z)] != board[PyrCell(x-1, y,   z)]) ? 2 : 0) +
            ((board[PyrCell(x,   y+1, z)] != board[PyrCell(x,   y,   z)]) ? 4 : 0) +
            ((board[PyrCell(x-1, y,   z)] != board[PyrCell(x,   y,   z)]) ? 8 : 0)
        ];
        cout << ((board[PyrCell(x, y+1, z)] != board[PyrCell(x, y, z)]) 
            ? "\u2500\u2500\u2500" : "\u0020\u0020\u0020");
    };

    static auto printMid = [](map<PyrCell, string>& board, int x, int y, int z) {
        string name = board[PyrCell(x, y, z)];
        cout << ((board[PyrCell(x-1, y, z)] != name) ? "\u2502" : "\u0020");
        cout << " " << (name.empty() ? " " : name) << " ";                
    };

    map<PyrCell, string> board;
    for(auto& re: solution) {
        auto pe = re;
        for(; pe->col->name.length() > 1; pe=pe->l);
        for(auto ce=pe->r; ce!=pe; ce=ce->r) {
            PyrCell cell(ce->col->name);
            ++cell.xOffset; ++cell.yOffset; ++cell.zOffset;
            board[cell] = pe->col->name;
        }
    }
    
    cout << "#" << ++count << ":" << endl;

    for(int y=5; y>=0; --y) {
        for(int x=1; x<=6; ++x) printTop(board, x, y, 1);
        for(int x=2; x<=5; ++x) printTop(board, x, y, 2);
        for(int x=3; x<=4; ++x) printTop(board, x, y, 3);
        cout << endl;
        for(int x=1; x<=6; ++x) printMid(board, x, y, 1);
        for(int x=2; x<=5; ++x) printMid(board, x, y, 2);
        for(int x=3; x<=4; ++x) printMid(board, x, y, 3);
        cout << endl;
    }
}
