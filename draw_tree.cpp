#include "draw_tree.hpp"
#include "draw_nodes.hpp"
#include <cmath>
#include "macro.hpp"

/**
 * @brief The line outside the circle, wont go straight into the circle
 * 
 * @param c1 the center of circle1
 * @param c2 the center of circle2
 * @param radius 
 * @return pair<Point, Point> 
 * two points to draw line between
 */
pair<Point, Point> better_line_point(Point c1, Point c2, double radius=RADIUS)
{
    double theta = atan2(c2.y - c1.y, c1.x - c2.x);
    Point p1, p2;
    p1.x = c1.x - radius * cos(theta);
    p1.y = c1.y + radius * sin(theta);
    p2.x = c2.x + radius * cos(theta);
    p2.y = c2.y - radius * sin(theta);
    return make_pair(p1, p2);
}

/**
 * @brief prepare the position of the tree, and prepare the nodes
 * 
 */
void Tree_Draw::initilize_position()
{
    bool debug = false;
    
    Point c = center_root;
    Node *root = (*tree).root;

    deque<Node*> q;
    q.push_back(root);

    // count is to count the current # of nodes in this level
    int count;
    int level = 1;

    double dx;
    Point draw_point(c.x, c.y);
    while (!q.empty())
    {
        int count = q.size();
        // draw the points for this level
        // each point in same level, has different x but same y
        // the distance of x should be dx

        // count the dx
        if (count == 1)
            dx = 0;
        else
            dx = (this->tree->width_win / 2 - 100) / (count - 1);

        // start from the very left
        draw_point.x -= (count - 1) / 2.0 * dx;
        for (int i = 0; i < count; ++i)
        {
            // prepare the nodes
            Node *curNode = q.front();
            q.pop_front();

            node_pos[curNode] = Point(draw_point.x, draw_point.y);

            if (debug) 
                printf("Level %d (%d, %d) %s\n",
                    level,
                    draw_point.x,
                    draw_point.y,
                    curNode->name.c_str());

            for (int i = 0; i < curNode->childs.size(); ++i)
                q.push_back(curNode->childs[i]);

            draw_point.x += dx;
        }
        level += 1;
        // goes to next level, y should plus dy
        // x reset
        draw_point.y += dy;
        draw_point.x = c.x;
    }
}

/**
 * @brief prepares the lines between nodes for the tree
 *        draws nothing
 * 
 */
void Tree_Draw::initialize_lines()
{
    map<Node*, Point>::const_iterator it;
    // draw lines
    for (it = node_pos.cbegin(); it != node_pos.cend(); it++)
    {
        Node *n = it->first;
        Point xy = it->second;
        Graph_lib::Line* line;
        for (int i = 0; i < n->childs.size(); ++i)
        {
            Point child_pos = node_pos.at(n->childs[i]);
            if (color().visibility())
            {
                pair<Point, Point> p1p2 = better_line_point(xy, child_pos);
                line = new Graph_lib::Line(Point(p1p2.first.x, p1p2.first.y),
                                           Point(p1p2.second.x, p1p2.second.y));
                this->lines.push_back(line);
            }
        }
    }
}

/**
 * @brief draw the nodes of the tree
 * 
 */
void Tree_Draw::draw_lines() const
{
    // for each node in node_pos, we create a Node_Draw
    // By the relationship offered by map, we add edges
    map<Node*, Point>::const_iterator it;
    // draw nodes
    for (it = node_pos.cbegin(); it != node_pos.cend(); it++)
    {
        Node *n = it->first;
        Point xy = it->second;
        Node_Draw draw_node(xy, n->name);
        if (color().visibility())
            draw_node.draw_lines();
    }
}

