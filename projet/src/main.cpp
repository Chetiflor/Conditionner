#include <iostream>
#include <random>

#include "tree.tpp"
#include "node.hpp"
#include "master.hpp"


Tree<Node> *const tree = new Tree<Node>();
Tree<Node> *const Node::main_tree = tree;
RealNode *n = new RealNode();
//Master * master = new Master(tree);

int main()
{
    tree->setNode(n);
    tree->print();
    std::cout << "---" << std::endl;
    TreeIndex index;
    tree->duplicate(index);
    tree->print();
    std::cout << "---" << std::endl;
    index.setFromString("0");
    tree->duplicate(index);
    index.setFromString("01");
    tree->duplicate(index);
    index.setFromString("010");
    tree->duplicate(index);
    tree->print();
    std::cout << "---" << std::endl;
    index.setFromString("01");
    tree->duplicate(index);
    tree->print();
    std::cout << "---" << std::endl;
    tree->getNode(TreeIndex("01101"))->setFlag(true);
    //tree->getNode(TreeIndex("01100"))->setFlag(true);
    tree->print();
    std::cout << "---" << std::endl;
    //master->step();
    tree->print();
    return 0;
}