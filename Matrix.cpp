#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Matrix.hpp"
#include "Element.hpp"
#include "Header.hpp"

using namespace std;


Matrix::Matrix()
:   h("Root")
{
}


Header* Matrix::findColumn(string const& name)
{
    Element *ce = h.r;
    while((ce != &h) && (ce->col->name < name)) {
        ce = ce->r;
    }
    if (ce->col->name != name) {
        auto ne = new Header(name);
        ne->InsertLR(ce);
        ce = ne;
    }
    return ce->col;
}


int Matrix::findCovers(function<void (vector<Element *>&)> print)
{
    vector<Element *> solution;
    return search(solution, print);
}


Header *Matrix::chooseColumn()
{
    Header *c = nullptr;
    int s = numeric_limits<int>::max();
    for(auto j=static_cast<Header *>(h.r); j!=&h; j=static_cast<Header *>(j->r)) {
        if (j->count < s) {
            c = j;
            s = j->count;
        }
    }
    return c;
}


void Matrix::coverColumn(Header *col)
{
    col->l->r = col->r;
    col->r->l = col->l;    
    for(auto i=col->d; i!=col; i=i->d) {
        for(auto j=i->r; j!=i; j=j->r) {
            j->u->d = j->d;
            j->d->u = j->u;
            j->col->count -= 1;
        }
    }
}


void Matrix::uncoverColumn(Header *col)
{
    for(auto i=col->u; i!=col; i=i->u) {
        for(auto j=i->l; j!=i; j=j->l) {
            j->col->count += 1;
            j->d->u = j;
            j->u->d = j;
        }
    }
    col->r->l = col;
    col->l->r = col;
}


int Matrix::search(vector<Element *> &solution, function<void (vector<Element *>&)> print)
{
    int n = 0;
    if (h.r == &h) {
        print(solution);
        return 1;
    }
    auto c = chooseColumn();
    coverColumn(c);
    for(auto r=c->d; r!=c; r=r->d) {
        solution.push_back(r);
        for(auto j=r->r; j!=r; j=j->r) {
            coverColumn(j->col);
        }
        n += search(solution, print);
        for(auto j=r->l; j!=r; j=j->l) {
            uncoverColumn(j->col);
        }
        solution.pop_back();
    }
    uncoverColumn(c);
    return n;
}
