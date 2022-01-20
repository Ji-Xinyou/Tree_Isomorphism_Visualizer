#include "draw_nodes.hpp"

/**
 * @brief nodes are just circles with names
 * 
 */
void Node_Draw::draw_lines() const
{
    if (color().visibility())
    {
        fl_circle(center.x, center.y, radius);
        fl_font(FL_ITALIC, FONTSIZE);
        fl_draw(name.c_str(), center.x - (radius / 4), center.y + (radius / 4));
    }
}