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
    
    myWindow win(topleft, 800, 800, "Tree Isomorphism");

    win.wait_for_button();

    return 0;
}
