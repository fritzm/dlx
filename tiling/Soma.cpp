#include <iomanip>
#include <iostream>
#include <map>

#include "Soma.hpp"

using namespace std;


class SomaCell
{
public:

    int xOffset;
    int yOffset;
    int zOffset;

    SomaCell(int x, int y, int z)
    :   xOffset(x), yOffset(y), zOffset(z)
    {
    }

    SomaCell(string const& name)
    {
        xOffset = stoi(name.substr(0,1));
        yOffset = stoi(name.substr(1,1));
        zOffset = stoi(name.substr(2,1));
    }

    bool operator<(SomaCell const& rhs) const
    {
        if (zOffset < rhs.zOffset) return true;
        if (zOffset > rhs.zOffset) return false;
        if (yOffset < rhs.yOffset) return true;
        if (yOffset > rhs.yOffset) return false;
        if (xOffset < rhs.xOffset) return true;
        if (xOffset > rhs.xOffset) return false;
        return false;
    }

    bool operator==(SomaCell const& rhs) const
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

ostream& operator<<(ostream &str, SomaCell const& cell)
{
    str << "(" << cell.xOffset << "," << cell.yOffset << "," << cell.zOffset << ")";
    return str;
};


class SomaPiece
{
public:

    string name;
    vector<SomaCell> cells;

    bool operator<(SomaPiece const& rhs) const
    {
        if (name < rhs.name) return true;
        if (name > rhs.name) return false;
        if (cells < rhs.cells) return true;
        if (cells > rhs.cells) return false;
        return false;
    }

    bool operator==(SomaPiece const& rhs) const
    {
        if (name != rhs.name) return false;
        if (cells != rhs.cells) return false;
        return true;
    }

};

ostream& operator<<(ostream& str, SomaPiece const& piece)
{
    str << piece.name << ": ";
    for(auto & cell : piece.cells) str << cell;
    return str;
};


namespace {

vector<SomaPiece> pieces = {
    {"V", {{0,0,0},{0,0,1},{0,1,1}}},
    {"L", {{0,0,0},{0,0,1},{0,1,1},{0,2,1}}},
    {"T", {{0,0,0},{1,0,0},{2,0,0},{1,1,0}}},
    {"Z", {{0,1,0},{1,1,0},{1,0,0},{2,0,0}}},
    {"A", {{0,0,0},{0,1,0},{0,1,1},{1,1,1}}},
    {"B", {{0,0,1},{0,1,1},{0,1,0},{1,1,0}}},
    {"P", {{0,0,1},{0,1,1},{1,1,1},{0,1,0}}}
};

} // anonymous namespace


void Soma::buildBoard(set<SomaCell>& board)
{
    for(auto z=0; z<=2; ++z) {
        for(auto y=0; y<=2; ++y) {
            for(auto x=0; x<=2; ++x) {
                board.insert(SomaCell(x, y, z));
            }
        }
    }
}


void Soma::addPieceAspects(SomaPiece const& piece, set<SomaPiece>& aspects)
{
    auto aspect = piece;
    for(int flip=0; flip<6; ++flip) {
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
                cell.xOffset -= minx;
                cell.yOffset -= miny;
                cell.zOffset -= minz;
            }
            aspects.insert(aspect);
            for(auto& cell: aspect.cells) {
                auto temp = cell.xOffset;
                cell.xOffset = cell.yOffset;
                cell.yOffset = -temp;
            }
        }
        for(auto& cell: aspect.cells) {
            auto temp = cell.xOffset;
            cell.xOffset = cell.zOffset;
            cell.zOffset = -temp;
        }
        if (flip & 1) {
            for(auto& cell: aspect.cells) {
                auto temp = cell.xOffset;
                cell.xOffset = cell.yOffset;
                cell.yOffset = -temp;
            }
        } else {
            for(auto& cell: aspect.cells) {
                auto temp = cell.yOffset;
                cell.yOffset = cell.xOffset;
                cell.xOffset = -temp;
            }
        }
    }
}


void Soma::addAspectPlacements(
    SomaPiece const& aspect, 
    set<SomaCell> const& board, 
    vector<SomaPiece> & placements)
{
    for(auto const& pos: board) {
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


bool Soma::aspectFilter(SomaPiece const& piece, int flip, int rot)
{
    if ((piece.name == "T") && ((flip != 0) || (rot != 0))) return true;
    return false;
}


bool Soma::placementFilter(SomaPiece const& aspect, SomaCell const& pos)
{
    if ((aspect.name == "T") && !(pos == SomaCell(0, 0, 0))) return true;
    if ((aspect.name == "P") && (pos.xOffset != 1)) return true;
    return false;
}


void Soma::init()
{    
    subGoals.emplace_back();
    auto& sg = subGoals.back();

    set<SomaPiece> aspects;
    for(auto const& piece: pieces) {
        addPieceAspects(piece, aspects);
    }

    set<SomaCell> board;
    buildBoard(board);

    vector<SomaPiece> placements;
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


void Soma::print(vector<Element *>& solution)
{
    static int count = 0;

    static string corner[16] = { // 0=up; 1=left; 2=right; 4=down
        "\u0020", "\u2575", "\u2574", "\u2518",
        "\u2576", "\u2514", "\u2500", "\u2534",
        "\u2577", "\u2502", "\u2510", "\u2524",
        "\u250c", "\u251c", "\u252c", "\u253c"
    };

    static auto printTop = [](map<SomaCell, string>& board, int x, int y, int z) {
        cout << corner[
            ((board[SomaCell(x-1, y+1, z)] != board[SomaCell(x,   y+1, z)]) ? 1 : 0) +
            ((board[SomaCell(x-1, y+1, z)] != board[SomaCell(x-1, y,   z)]) ? 2 : 0) +
            ((board[SomaCell(x,   y+1, z)] != board[SomaCell(x,   y,   z)]) ? 4 : 0) +
            ((board[SomaCell(x-1, y,   z)] != board[SomaCell(x,   y,   z)]) ? 8 : 0)
        ];
        cout << ((board[SomaCell(x, y+1, z)] != board[SomaCell(x, y, z)]) 
            ? "\u2500\u2500\u2500" : "\u0020\u0020\u0020");
    };

    static auto printMid = [](map<SomaCell, string>& board, int x, int y, int z) {
        string name = board[SomaCell(x, y, z)];
        cout << ((board[SomaCell(x-1, y, z)] != name) ? "\u2502" : "\u0020");
        cout << " " << (name.empty() ? " " : name) << " ";                
    };

    map<SomaCell, string> board;
    for(auto& re: solution) {
        auto pe = re;
        for(; pe->col->name.length() > 1; pe=pe->l);
        for(auto ce=pe->r; ce!=pe; ce=ce->r) {
            SomaCell cell(ce->col->name);
            ++cell.xOffset; ++cell.yOffset; ++cell.zOffset;
            board[cell] = pe->col->name;
        }
    }
    
    cout << "#" << ++count << ":" << endl;

    for(int y=3; y>=0; --y) {
        for(int z=1; z<=3; ++z) {
            for(int x=1; x<=4; ++x) printTop(board, x, y, z);
        }
        cout << endl;
        for(int z=1; z<=3; ++z) {
            for(int x=1; x<=4; ++x) printMid(board, x, y, z);
        }
        cout << endl;
    }
}
