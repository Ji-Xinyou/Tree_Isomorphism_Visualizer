/**
 * @file main.cpp
 * @author Ji-Xinyou (jerryji0414@outlook.com)
 * @brief The main process running the window, where tree resides
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "tree.hpp"
#include "draw_tree.hpp"
#include "draw_nodes.hpp"
#include "tree_buttons.hpp"
#include "Mywindow.hpp"

#include "revised-gui/Simple_window.h" // get access to our window library
#include "revised-gui/Graph.h"         // get access to our graphics library facilities
#include "revised-gui/GUI.h"

#include <iostream>
#include <map>

using namespace std;

int main()
{
    Point topleft(100, 100);

    int width = 1200, height = 1200;
    myWindow win(topleft, width, height, "Tree Isomorphism");

    win.wait_for_button();

    return 0;
}
