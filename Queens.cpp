#include <iostream>
#include <set>
#include <limits>
#include <sstream>
#include <utility>

#include "Queens.hpp"

#include "Element.hpp"
#include "Matrix.hpp"

using namespace std;


Queens::Queens(int n)
:   n(n)
{
}


void Queens::init()
{
    subGoals.emplace_back();
    auto& sg = subGoals.back();

    int r = (n-1)/2, rs = -1;
    for(int i=0; i<n; ++i) {
        r += (rs*i);
        rs *= -1;

        int f = (n-1)/2, fs = -1;
        for(int j=0; j<n; ++j) {
            f += (fs*j);
            fs *= -1;

            ostringstream rname;
            rname << "R" << r;
            auto e1 = new Element();
            auto c1 = sg.matrix.findColumn(rname.str());
            e1->insertUD(c1);
            ++sg.elems;

            ostringstream fname;
            fname << "F" << f;
            auto e2 = new Element();
            auto c2 = sg.matrix.findColumn(fname.str());
            e2->insertUD(c2);
            e2->insertLR(e1);
            ++sg.elems;

            int a = r+f;
            if (a!=0 && a!=(2*n-2)) {
                ostringstream aname;
                aname << "A" << a;
                auto e3 = new Element();
                auto c3 = sg.matrix.findColumn(aname.str(), false);
                e3->insertUD(c3);
                e3->insertLR(e1);
                ++sg.elems;
            }

            int b = n - 1 - r + f;
            if (b!=0 && b!=(2*n-2)) {
                ostringstream bname;
                bname << "B" << b;
                auto e4 = new Element();
                auto c4 = sg.matrix.findColumn(bname.str(), false);
                e4->insertUD(c4);
                e4->insertLR(e1);
                ++sg.elems;
            }

            ++sg.rows;
        }
    }
}


void Queens::print(vector<Element *>& solution)
{
    static int count = 0;

    set<pair<int, int>> queens;
    for(auto& e: solution) {
        auto re = e;
        for(; re->col->name[0] != 'R'; re=re->r);
        auto r = atoi(re->col->name.c_str()+1);
        auto f = atoi(re->r->col->name.c_str()+1);
        queens.insert(pair<int, int>(r, f));
    }

    cout << "#" << ++count << ":" << endl;
    for(int f=0; f<n; ++f) {
        for(int r=0; r<n; ++r) {
            cout << (queens.count(pair<int, int>(r, f)) ? "\u265b " : "\u30fb"); 
        }
        cout << endl;
    }
    cout << endl;
}