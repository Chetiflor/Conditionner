#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <vector>

#include "tree_index.hpp"

// class testclass
// {
// public:
//     testclass()
//     {

//         id = count;
//         count++;
//     }
//     std::string toString() { return std::to_string(id); }
//     testclass *displace(TreeIndex origin, TreeIndex destination) { return new testclass(); }
//     int id;
//     static int count;
// };

enum NodeType
{
    Abstract,
    Real
};

template <class T>
class Tree
{
private:
    Tree<T> *left_subtree;
    Tree<T> *right_subtree;
    T *node;

public:
    std::vector<T *> birthing_nodes;
    Tree(/* args */);
    Tree(T *node);
    Tree(Tree<T> *l, Tree<T> *r, T *node);
    ~Tree();
    Tree<T> *getSubTree(side s);
    void setSubTree(Tree<T> *sub_tree, side s);
    Tree<T> *getSubTree(TreeIndex index);
    Tree<T> *copy(bool mutations);
    void moveTreeIndices(TreeIndex origin, TreeIndex destination);

    void duplicate(TreeIndex absolute_index); // USE ONLY FROM ROOT TREE
    void print();
    std::vector<std::string> toString(TreeIndex index);
    T *getNode();
    T *getNode(TreeIndex index);
    std::vector<T *> getInstances();
    void setNode(T *n);
    void set(Tree<T> *l, Tree<T> *r, T *node);
    void pull();
    void clean();
};

template <class T>
void destroy(Tree<T> *tree)
{

    Tree<T> *left_subtree = tree->getSubTree(gauche);
    Tree<T> *right_subtree = tree->getSubTree(droite);
    if (left_subtree != nullptr)
    {
        destroy(left_subtree);
    }
    if (right_subtree != nullptr)
    {
        destroy(right_subtree);
    }
    T *node = tree->getNode();
    delete (node);
    delete (tree);
}

template <class T>
Tree<T>::Tree(/* args */)
{
    left_subtree = nullptr;
    right_subtree = nullptr;
    node = nullptr;
}

template <class T>
Tree<T>::Tree(T *n)
{

    left_subtree = nullptr;
    right_subtree = nullptr;
    node = n;
}

template <class T>
Tree<T>::Tree(Tree<T> *l, Tree<T> *r, T *n)
{
    left_subtree = l;
    right_subtree = r;
    node = n;
}

template <class T>
Tree<T>::~Tree()
{
    if (left_subtree != nullptr)
    {
        delete (left_subtree);
        left_subtree = nullptr;
    }
    if (right_subtree != nullptr)
    {
        delete (right_subtree);
        right_subtree = nullptr;
    }
}

template <class T>
Tree<T> *Tree<T>::copy(bool mutations)
{
    T *tmp_node = nullptr;
    if (node != nullptr)
    {
        tmp_node = node->copy(mutations);
    }
    Tree<T> *tmp_left = nullptr;
    if (left_subtree != nullptr)
    {
        tmp_left = left_subtree->copy(mutations);
    }
    Tree<T> *tmp_right = nullptr;
    if (right_subtree != nullptr)
    {
        tmp_right = right_subtree->copy(mutations);
    }
    return new Tree<T>(tmp_left, tmp_right, tmp_node);
}

template <class T>
void Tree<T>::moveTreeIndices(TreeIndex origin, TreeIndex destination)
{
    node->displace(origin, destination);
    if (left_subtree != nullptr)
    {
        left_subtree->moveTreeIndices(origin, destination);
    }
    if (right_subtree != nullptr)
    {
        right_subtree->moveTreeIndices(origin, destination);
    }
}

template <class T>
void Tree<T>::duplicate(TreeIndex absolute_index)
{
    TreeIndex origin = absolute_index;
    TreeIndex left_destination = absolute_index;
    left_destination.push(gauche);
    TreeIndex right_destination = absolute_index;
    right_destination.push(droite);

    Tree<T> *to_duplicate = getSubTree(absolute_index);

    Tree<T> *r_tmp = to_duplicate->copy(true);
    Tree<T> *l_tmp = new Tree<T>(to_duplicate->getSubTree(gauche), to_duplicate->getSubTree(droite), to_duplicate->getNode());

    r_tmp->moveTreeIndices(origin,right_destination); // !!!!!!!!!!!!!!!!!!!!! problem here !!!!!!!!!!!!!!!!!!!!
    l_tmp->moveTreeIndices(origin,left_destination);

    to_duplicate->set(l_tmp, r_tmp, to_duplicate->getNode()->getAbstract(origin));

    for (int i = 0; i < birthing_nodes.size(); i++)
    {
        birthing_nodes[i]->onBirth();
    }

    birthing_nodes = {};
}
template <class T>
Tree<T> *Tree<T>::getSubTree(side s)
{
    if (s == gauche)
    {
        return (left_subtree);
    }
    else
    {
        return (right_subtree);
    }
}
template <class T>
Tree<T> *Tree<T>::getSubTree(TreeIndex index)
{

    if (!index.null())
    {
        side s = index.pop();
        if (s == gauche && left_subtree != nullptr)
        {
            return left_subtree->getSubTree(index);
        }
        if (s == droite && right_subtree != nullptr)
        {
            return right_subtree->getSubTree(index);
        }
    }
    return this;
}

template <class T>
void Tree<T>::setSubTree(Tree<T> *tree, side s)
{
    if (s == gauche)
    {
        left_subtree = tree;
    }
    else
    {
        right_subtree = tree;
    }
}

template <class T>
void Tree<T>::print()
{
    TreeIndex index;
    std::vector<std::string> strings = toString(index);
    for (int i = 0; i < strings.size(); i++)
    {
        std::cout << strings[i] << std::endl;
    }
}

template <class T>
std::vector<std::string> Tree<T>::toString(TreeIndex index)
{
    std::vector<std::string> to_return;
    std::string str;

    str = index.toString() + ": {";
    if (node != nullptr)
    {
        str += node->toString();
    }
    str += "}";
    if (left_subtree == nullptr && right_subtree == nullptr)
    {
        return {str};
    }
    std::string blank_prefix = std::string(str.size(), ' ');

    if (left_subtree != nullptr)
    {
        TreeIndex l_index = index;
        l_index.push(gauche);
        std::vector<std::string> l_strings = left_subtree->toString(l_index);
        for (int i = 0; i < l_strings.size(); i++)
        {
            std::string fork_marker = " |   ";
            std::string prefix = blank_prefix;
            if (i == 0)
            {
                fork_marker = " +-> ";
                prefix = str;
            }
            to_return.push_back(prefix + fork_marker + l_strings[i]);
        }
    }

    if (right_subtree != nullptr)
    {
        TreeIndex r_index = index;
        r_index.push(droite);
        std::vector<std::string> r_strings = right_subtree->toString(r_index);
        for (int i = 0; i < r_strings.size(); i++)
        {
            std::string fork_marker = "     ";
            if (i == 0)
            {
                fork_marker = " +-> ";
            }
            to_return.push_back(blank_prefix + fork_marker + r_strings[i]);
        }
    }
    return to_return;
}

template <class T>
T *Tree<T>::getNode()
{
    return node;
}

template <class T>
T *Tree<T>::getNode(TreeIndex index)
{
    return getSubTree(index)->getNode();
}

template <class T>
std::vector<T *> Tree<T>::getInstances()
{
    return {};
};

template <class T>
void Tree<T>::setNode(T *n)
{
    node = n;
}

template <class T>
void Tree<T>::set(Tree<T> *l, Tree<T> *r, T *n)
{
    left_subtree = l;
    right_subtree = r;
    node = n;
}

template <class T>
void Tree<T>::pull()
{
    bool left = left_subtree != nullptr;
    bool right = right_subtree != nullptr;
    if (left)
    {
        left_subtree->pull();
    }
    if(right)
    {
        right_subtree->pull();
    }
    if (left != right)
    {
        Tree<T> *tmp;
        if (left)
        {
            tmp = left_subtree;
        }
        else
        {
            tmp = right_subtree;
        }
        left_subtree = tmp->getSubTree(gauche);
        tmp->setSubTree(nullptr, gauche);
        right_subtree = tmp->getSubTree(droite);
        tmp->setSubTree(nullptr, droite);
        delete (node);
        node = tmp->getNode();
        delete (tmp);
    }
}
template <class T>
void Tree<T>::clean()
{

    if (left_subtree != nullptr)
    {
        T *n = left_subtree->getNode();
        if (n->isDying())
        {
            delete (n);
            n = nullptr;
            delete (left_subtree);
            left_subtree = nullptr;
        }
        else
        {
            left_subtree->clean();
        }
    }
    if (right_subtree != nullptr)
    {
        T *n = right_subtree->getNode();
        if (n->isDying())
        {
            delete (n);
            n = nullptr;
            delete (right_subtree);
            right_subtree = nullptr;
            return;
        }
        else
        {
            right_subtree->clean();
        }
    }
}


#endif