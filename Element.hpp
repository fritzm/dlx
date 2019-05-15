#ifndef ELEMENT_H
#define ELEMENT_H

class Header;

class Element
{
public:

    Element();

    void insertLR(Element *before);
    void insertUD(Element *before);

    Element *l; 
    Element *r; 
    Element *u; 
    Element *d;

    Header *col;

};

class Header : public Element
{
public:

    Header(std::string const& name);

    int count;
    std::string name;   

};

#endif // !defined(ELEMENT_H)
