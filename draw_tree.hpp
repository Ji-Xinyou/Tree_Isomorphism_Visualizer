#pragma once

#include "revised-gui/Simple_window.h" // get access to our window library
#include "revised-gui/Graph.h"         // get access to our graphics library facilities
#include "revised-gui/GUI.h"
#include "tree.hpp"
#include "Mywindow.hpp"
#include <vector>

// using namespace Graph_lib;

/**
 * @brief class to draw trees
 * 
 */

class Tree_Draw : public Graph_lib::Shape
{
    friend class tree_buttons;
    friend class myWindow;

protected:
    mytree *tree; // tree to be drawn

    Point center_root; // the center_pos of the root nodes

    double dy; // the pixel gap between two node levels

    map<Node *, Point> node_pos; // mapping of node -> pos

    vector<Graph_lib::Line *> lines; // save the Graph_lib::Line* object,
                                     // called in mywindow's callback many times

    /**
     * @brief following two functions are called in the constructor
     *        initialize_position(): set up the mapping of node -> pos
     *        initialize_lines():    calculate the lines to be drawn and save in lines
     */
    void initilize_position();
    void initialize_lines();

    void draw_lines() const;

public:
    Tree_Draw(mytree *t, Point c, double dy = 60) : tree(t), center_root(c), dy(dy)
    {
        initilize_position();
        initialize_lines();
    }
};