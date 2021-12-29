// contruct buttons used for highlighting nodes
// use two trees to construct

// call pair_tree_Nodes() to get the mapping order of each tree
// i.e. the map of map1[Node*] = Node*

// in draw_tree of each Tree_Draw, use node_pos to get the
// mapping of Node -> pos
// i.e. the map of map2[Node*] -> point (pos of node)

/* pipeline:
 * so if A is mapped to B
 * 1. pos_map[A] = pos_A
 * 2. node_map[A] = B
 * 3. pos_map[B] = pos_B
 * 4. draw two highlighed node in pos_A and pos_B
 */

#pragma once

#include "revised-gui/Simple_window.h" // get access to our window library
#include "revised-gui/Graph.h"         // get access to our graphics library facilities
#include "revised-gui/GUI.h"
#include "tree.hpp"
#include "draw_tree.hpp"
#include <vector>
#include <unordered_map>

typedef std::pair<int, int> coordinate;

class Tree_Draw;

class tree_buttons
{
friend class myWindow;
private:
protected:
    mytree* t1;
    mytree* t2;

    Tree_Draw* td1;
    Tree_Draw* td2;

    map<Node *, Node *> node_to_node;
    map<Node *, coordinate> node_to_pos;
    map<coordinate, Node *> pos_to_node;

    vector<Point> button_pos;

    int radius;

    /**
     * @brief following three functions are called in the constructor
     *        they initialize three member maps and save the position of button
     *        in button_pos vec
     */
    void get_node_map();
    void get_pos_map();
    void generate_button_pos();

public:
    tree_buttons(mytree *tree1, mytree *tree2,
                 Tree_Draw *td1, Tree_Draw *td2,
                 int r=10);
};