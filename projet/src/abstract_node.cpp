#include <algorithm>

#include "node.hpp"

Node *Node::getAbstract(TreeIndex linked)
{
    return new AbstractNode(linked);
}


AbstractNode::AbstractNode()
{
    linked_index = TreeIndex();
    dying_flag = false;
}

AbstractNode::AbstractNode(TreeIndex linked)
{
    linked_index = linked;
    dying_flag = false;
}

AbstractNode::~AbstractNode()
{
    // todo
}

void AbstractNode::onBirth()
{
}

void AbstractNode::refresh()
{
    // todo
}

void AbstractNode::step()
{
    // todo
}

void AbstractNode::learn(float commander_temperature)
{
    // todo
}

activation AbstractNode::getState()
{
    Node * n = getInstance();
    if(n != nullptr)
    {
        return n->getState();
    }
    return down;
}

void AbstractNode::displace(TreeIndex origin, TreeIndex destination)
{

    linked_index.replacePrefix(origin, destination);
}

Node *AbstractNode::copy(bool mutations)
{
    return new AbstractNode(linked_index);
}

NodeType AbstractNode::type()
{
    return Abstract;
}

Node *AbstractNode::getInstance()
{
    if (instance != nullptr)
    {
        return instance;
    }
    std::uniform_real_distribution<float> distribution(0, 1);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    float x = distribution(generator);
    Tree<Node> *tmp = main_tree->getSubTree(linked_index);
    Tree<Node> *tmp_sub_tree;
    if (x < left_proba)
    {
        tmp_sub_tree = tmp->getSubTree(gauche);
    }
    else
    {
        tmp_sub_tree = tmp->getSubTree(droite);
    }
    if (tmp_sub_tree != nullptr)
    {
        instance = tmp_sub_tree->getNode()->getInstance();
        return instance;
    }
    std::cout << "Error : abstract node without left children" << std::endl;
    return nullptr;
}

std::string AbstractNode::toString()
{
    return std::to_string(getState());
}