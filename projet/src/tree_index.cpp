#include <tree_index.hpp>


TreeIndex::TreeIndex()
{
    setFromString("");
}

TreeIndex::TreeIndex(std::string s)
{
    setFromString(s);
}

side TreeIndex::pop()
{
    side s = tree_int.front();
    tree_int.pop_front();
    return s;
}

void TreeIndex::push(side s)
{
    tree_int.push_back(s);
}

bool TreeIndex::null()
{
    return tree_int.size() == 0;
}

size_t TreeIndex::size()
{
    return tree_int.size();
}

std::string TreeIndex::toString()
{
    std::string str = "";
    for (int i = 0; i < tree_int.size(); i++)
    {
        str += tree_int[i] ? "1" : "0";
    }
    return str;
}

void TreeIndex::setFromString(std::string s)
{
    tree_int = {};
    for (int i = 0; i < s.size(); i++)
    {
        side sd = s[i] == '1';
        tree_int.push_back(sd);
    }
}

bool TreeIndex::operator==(TreeIndex const &rhs) const
{
    if(tree_int.size() != rhs.tree_int.size())
    {
        return false;
    }
    for(int i =0; i<tree_int.size(); i++)
    {
        if(tree_int[i]!=rhs.tree_int[i])
        {
            return false;
        }
    }
    return true;
}

bool TreeIndex::operator<(TreeIndex const &rhs) const
{
    // ordered descending left first
    // 0<00<01<1
    int i = 0;
    int inf = std::min(tree_int.size(),rhs.tree_int.size());
    while(i<inf)
    {
        if(tree_int[i] && !rhs.tree_int[i])
        {
            return false;
        }
        i++;
    }
    return tree_int.size()<rhs.tree_int.size();
}

bool TreeIndex::operator[](int const &n) const
{
    return tree_int[n];
};

void TreeIndex::replacePrefix(TreeIndex former, TreeIndex later)
{
    if(former.tree_int.size()> tree_int.size())
    {
        return;
    }
    for (int i = 0; i < former.tree_int.size(); i++)
    {
        if (tree_int[i]!=former.tree_int[i])
        {
            return;
        }
    }
    tree_int.erase(tree_int.begin(),tree_int.begin()+former.size());
    tree_int.insert(tree_int.begin(),later.tree_int.begin(),later.tree_int.end());
    
};