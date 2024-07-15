#ifndef MASTER_HPP
#define MASTER_HPP

#include "tree.tpp"
#include "node.hpp"

class Master
{
private:
    Tree<Node> * const main_tree;


public:
    Master(Tree<Node> * main_tree):main_tree(main_tree){};
    ~Master(){};

    void step();
};






#endif