#ifndef ELEMENT_H
#define ELEMENT_H

class Header;

class Element
{
public:

    Element();

    void InsertLR(Element *before);
    void InsertUD(Element *before);

    Element *l; 
    Element *r; 
    Element *u; 
    Element *d;

    Header *col;

};

#endif // !defined(ELEMENT_H)
