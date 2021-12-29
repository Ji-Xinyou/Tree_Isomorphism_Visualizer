#pragma once

#include <string>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "./revised-gui/Graph.h"

using namespace std;

struct Node
{

    string name;
    // probably has more parents(invalid), used to check validity
    Node* parent;
    vector<Node *> childs;

    Node(string n) : name(n), parent(nullptr){};
};

class mytree
{
    friend class Tree_Draw;

protected:
    Node *root;

    bool is_Tree;

    map<string, vector<string> > parent_child_map;

public:
    /**
     * @brief Construct a new mytree object
     * 
     * @param filename file to be read
     */
    mytree(string filename);

    void check_validity();

    /**
     * @brief 
     * open a file and build parent_child_map from it
     * @param filename filename to be opened
     */

    void build_map_from_file(string filename);

    /**
     * @brief
     * after we build the map, we can find the root of the tree
     * from the root, we build the entire tree
     */

    string find_root();

    /**
     * @brief 
     * from the parent_child_map, build the linked tree
     * data structure
     */

    void build_tree_from_map();

    /**
     * @brief 
     * traver the tree through inorder traversal
     */

    void traverse();

    /**
     * @brief 
     * get the encoding of a node, and return the 
     * sorted childlist of it (used to align nodes)
     * 
     * @param n 
     * the node to be encoded
     * 
     * @return pair<string, vector<Node*> > 
     * key: encoding
     * value: childlist (sorted)
     */

    pair<string, vector<Node *> > encode(Node *n);

    /**
     * @brief 
     * output all the encoding of nodes
     * used for debugging
     */

    void debug_encode();

    /**
     * @brief Get the encoding of root
     * 
     * @return pair<string, vector<Node*> > 
     * return the encoding of root, 
     * and the childlist (sorted) of it
     */

    pair<string, vector<Node *> > get_encoding();

    /**
     * @brief 
     * return whether the tree is isomorphism to *this
     * @param t the tree to be compared
     * @return true if isomorphism
     * @return false if not
     */
    bool is_isomorphism(mytree t);

    bool is_valid() { return is_Tree; }

    map<Node *, Node *> pair_tree_Nodes(mytree t);
};
