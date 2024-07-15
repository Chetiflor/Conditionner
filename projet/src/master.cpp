#include "master.hpp"
#include <queue>

void Master::step()
{
    std::queue<Tree<Node>*> sub_trees({main_tree});
    std::vector<Node *> nodes;
    while(sub_trees.size()!=0)
    {
        Tree<Node> *current = sub_trees.back();
        sub_trees.pop();
        Node * n = current->getNode();
        n->refresh();
        nodes.push_back(n);
        Tree<Node> *right=current->getSubTree(droite);
        if (right!=nullptr)
        {
            sub_trees.push(right);
        }
        Tree<Node> *left=current->getSubTree(gauche);
        if (left!=nullptr)
        {
            sub_trees.push(left);
        }
        
    }
    for(int i = 0; i <nodes.size(); i++)
    {
        nodes[i]->step();
    }
}