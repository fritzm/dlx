#include <iostream>
#include <sstream>

#include "Sudoku.hpp"

using namespace std;


Sudoku::Sudoku()
{
}


void Sudoku::init()
{
    int board[9][10]={0}, row[9][10]={0}, col[9][10]={0}, box[9][10]={0};

    cout << "Enter sudoku grid as nine lines of nine digits (or '.' for unspecified):" << endl;
    for(int r=0; r<9;) {
        string line;
        getline(cin, line);
        if (line.length() != 9) throw runtime_error("not 9 characters");
        for(int c=0; c<9; ++c) {
            int b = ((r/3)*3) + (c/3);
            if (line[c] != '.') {
                if (line[c] < '1' or line[c] > '9')
                    throw runtime_error(string("bad character '") + line[c] + "'");
                int d = line[c] - '0';
                if (row[r][d])
                    throw runtime_error(string("duplicate ") + line[c] + " in row");
                if (col[c][d])
                    throw runtime_error(string("duplicate ") + line[c] + " in column");
                if (box[b][d])
                    throw runtime_error(string("duplicate ") + line[c] + " in box");
                board[r][c] = d;
                row[r][d] = 1;
                col[c][d] = 1;
                box[b][d] = 1;
            }
        }
        ++r;
    }
    cout << endl;

    subGoals.emplace_back();
    auto& sg = subGoals.back();

    for(int r=0; r<9; ++r) {
        for(int c=0; c<9; ++c) {
            if (!board[r][c]) {
                int b = ((r/3)*3) + (c/3);
                for(int d=1; d<10; ++d) {
                    if (!row[r][d] && !col[c][d] && !box[b][d]) {
                        ostringstream pname;
                        pname << "P" << r << c;
                        auto e1 = new Element();
                        auto c1 = sg.matrix.findColumn(pname.str());
                        e1->insertUD(c1);
                        ++sg.elems;
                        ostringstream rname;
                        rname << "R" << r << d;
                        auto e2 = new Element();
                        auto c2 = sg.matrix.findColumn(rname.str());
                        e2->insertUD(c2);
                        e2->insertLR(e1);
                        ++sg.elems;
                        ostringstream cname;
                        cname << "C" << c << d;
                        auto e3 = new Element();
                        auto c3 = sg.matrix.findColumn(cname.str());
                        e3->insertUD(c3);
                        e3->insertLR(e1);
                        ++sg.elems;
                        ostringstream bname;
                        bname << "B" << b << d;
                        auto e4 = new Element();
                        auto c4 = sg.matrix.findColumn(bname.str());
                        e4->insertUD(c4);
                        e4->insertLR(e1);
                        ++sg.elems;
                    }
                }
            }
            ++sg.rows;
        }
    }
}


void Sudoku::print(std::vector<Element *>& solution)
{
    int board[9][10]={0};
    for(auto& e: solution) {
        auto re = e;
        for(; re->col->name[0] != 'P'; re=re->r);
        auto r = re->col->name.data()[1] - '0';
        auto c = re->col->name.data()[2] - '0';
        auto d = re->r->col->name.data()[2] - '0';
        board[r][c] = d;
    }
    for(int r=0; r<9; ++r) {
        for(int c=0; c<9; ++c) {
            if (!board[r][c]) {
                cout << ".";
            } else {
                cout << board[r][c];
            }
        }
        cout << endl;
    }
    cout << endl;
}
