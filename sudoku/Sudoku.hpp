#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include "Puzzle.hpp"

class Sudoku : public Puzzle
{
public:

    Sudoku();

private:

    void init() override;
    void print(std::vector<Element *>& solution) override;

};

#endif // !define(SUDOKU_HPP)
