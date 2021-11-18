#pragma once

#include "revised-gui/GUI.h"
#include "revised-gui/Graph.h"
#include "tree.hpp"
#include "draw_tree.hpp"
#include "tree_buttons.hpp"
#include "macro.hpp"
#include <FL/Fl_Button.H>
#include <vector>

/* ---------------------------------- start --------------------------------- */

class Tree_Draw;
class tree_buttons;

class myWindow : public Graph_lib::Window
{

protected:
    Graph_lib::In_box box_file1;
    Graph_lib::In_box box_file2;
    Graph_lib::Button button_build_tree;
    Graph_lib::Button button_exit;
    vector<Graph_lib::Button*> buttons_highlight;

    bool button_build_tree_pushed;
    bool button_exit_pushed;
    static void cb_exit(Graph_lib::Address, Graph_lib::Address);
    static void cb_build_tree(Graph_lib::Address, Graph_lib::Address);
    static void cb_highlight(Graph_lib::Address, Graph_lib::Address);

    /* -------- used to draw trees -------- */
    mytree* tree1;
    mytree* tree2;

    Point* c1;
    Point* c2;

    Tree_Draw* td1;
    Tree_Draw* td2;

    tree_buttons* tb;
    int radius;

    vector<Graph_lib::Circle*> circles;
    vector<Graph_lib::Text*> texts;
    /* -------- used to draw trees -------- */

public:
    myWindow(Point xy, int w, int h, const string &title);

    bool wait_for_button();

    void exit();

    void buildtree();

    void highlight(Fl_Widget* widget);
};