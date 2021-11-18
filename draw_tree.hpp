#pragma once

#include "revised-gui/Simple_window.h" // get access to our window library
#include "revised-gui/Graph.h"         // get access to our graphics library facilities
#include "revised-gui/GUI.h"
#include "tree.hpp"
#include "Mywindow.hpp"
#include <vector>

// using namespace Graph_lib;

class Tree_Draw : public Graph_lib::Shape
{
friend class tree_buttons;
friend class myWindow;
protected:
    mytree *tree;
    Point center_root;
    double dy;
    map<Node*, Point> node_pos;
    vector<Graph_lib::Line*> lines;

    void draw_lines() const;

    void initilize_position();
    void initialize_lines();

public:
    Tree_Draw(mytree *t, Point c, double dy = 60) : tree(t), center_root(c), dy(dy) 
    {
        initilize_position();
        initialize_lines();
    }
};