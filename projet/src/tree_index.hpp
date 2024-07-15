#ifndef INDEX_HPP
#define INDEX_HPP
#define gauche false
#define droite true
#define side bool

#include <deque>
#include <string>

class TreeIndex
{
public:
    std::deque<side> tree_int;

    TreeIndex();
    TreeIndex(std::string s);
    side pop();
    void push(side s);
    bool null();
    size_t size();
    std::string toString();
    void setFromString(std::string s);
    bool operator==(TreeIndex const &rhs) const;
    bool operator<(TreeIndex const &rhs) const;
    bool operator[](int const &n) const;

    void replacePrefix(TreeIndex former, TreeIndex later);
};

#endif