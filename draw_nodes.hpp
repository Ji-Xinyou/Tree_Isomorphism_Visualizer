#pragma once

#include "revised-gui/Simple_window.h" // get access to our window library
#include "revised-gui/Graph.h"         // get access to our graphics library facilities
#include "revised-gui/GUI.h"
#include "macro.hpp"

using namespace Graph_lib;

/**
 * @brief class to draw nodes
 * 
 */

class Node_Draw : public Shape
{
    friend class Tree_Draw;

protected:
    Point center;
    double radius;
    std::string name;

    void draw_lines() const;

public:
    Node_Draw(Point c, std::string n, double r = RADIUS) : center(c), radius(r), name(n) {}
};