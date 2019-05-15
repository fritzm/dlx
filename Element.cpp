#include <string>

#include "Element.hpp"

using namespace std;


Element::Element()
:
    l(this), r(this), u(this), d(this), col(nullptr)
{
}


void Element::InsertLR(Element *before)
{
    r = before;
    l = before->l;
    r->l = this;
    l->r = this;
}


void Element::InsertUD(Element *before)
{
    d = before;
    u = before->u;
    d->u = this;
    u->d = this;
    col = before->col;
    col->count += 1;
}


Header::Header(string const& name)
:
    count(0), name(name)
{
    col = this;
}
