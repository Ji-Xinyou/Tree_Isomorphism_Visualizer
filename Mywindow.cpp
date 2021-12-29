#include "revised-gui/GUI.h"
#include "revised-gui/Graph.h"
#include "Mywindow.hpp"
#include <FL/Fl_Input.H>
#include <string>

/* ----------------------------- Impl get_string ---------------------------- */

string Graph_lib::In_box::get_string()
{
    Fl_Input &pi = reference_to<Fl_Input>(pw);
    return string(pi.value());
}

/* ----------------------------- Impl get_string ---------------------------- */

myWindow::myWindow(Point xy, int w, int h, const string &title) : Window(xy, w, h, title),
                                                                  button_build_tree_pushed(false),
                                                                  button_exit_pushed(false),
                                                                  box_file1(Point(150, 50), 100, 20, "filename1"),
                                                                  box_file2(Point(350, 50), 100, 20, "filename2"),
                                                                  button_build_tree(Point(500, 50), 70, 20, "Build Tree", cb_build_tree),
                                                                  button_exit(Point(x_max() - 70, 0), 70, 40, "Exit", cb_exit)
// tree1(nullptr), tree2(nullptr),
// draw_t_1(nullptr), draw_t_2(nullptr)
{
    attach(button_build_tree);
    attach(button_exit);
    attach(box_file1);
    attach(box_file2);
};

bool myWindow::wait_for_button()
// modified event loop:
// handle all events (as per default), quit when button_pushed becomes true
// this allows graphics without control inversion
{
    show();
    button_exit_pushed = false;
#if 1
    // Simpler handler
    while (!button_exit_pushed)
        Fl::wait();
    Fl::redraw();
#else
    // To handle the case where the user presses the X button in the window frame
    // to kill the application, change the condition to 0 to enable this branch.
    Fl::run();
#endif
    return button_exit_pushed;
}

void myWindow::cb_exit(Graph_lib::Address, Graph_lib::Address pw)
{
    Graph_lib::reference_to<myWindow>(pw).exit();
}

void myWindow::exit()
{
    button_exit_pushed = true;
    hide();
}

void myWindow::cb_build_tree(Graph_lib::Address, Graph_lib::Address pw)
{
    Graph_lib::reference_to<myWindow>(pw).buildtree();
}

void myWindow::buildtree()
{
    std::string filename1, filename2;
    filename1 = box_file1.get_string();
    filename2 = box_file2.get_string();

    this->tree1 = new mytree(filename1);
    this->tree2 = new mytree(filename2);

    this->c1 = new Point(200, 200);
    this->c2 = new Point(600, 200);

    if (!tree1->is_valid()) {
        fl_font(FL_ITALIC, 20);
        this->tree1_error = new Graph_lib::Text(*(this->c1), "Tree1 is not a tree!");
        this->attach(*tree1_error);
    }

    if (!tree2->is_valid()) {
        fl_font(FL_ITALIC, 20);
        this->tree2_error = new Graph_lib::Text(*(this->c2), "Tree2 is not a tree!");
        this->attach(*tree2_error);
    }

    if (tree1->is_valid())
        this->td1 = new Tree_Draw(tree1, *c1);
    if (tree2->is_valid())
        this->td2 = new Tree_Draw(tree2, *c2);

    if (tree1->is_valid())
        this->attach(*td1);
    if (tree2->is_valid())
        this->attach(*td2);

    if (tree1->is_valid()) {
        for (int i = 0; i < td1->lines.size(); ++i)
            this->attach(*td1->lines[i]);
    }
    if (tree2->is_valid()) {
        for (int i = 0; i < td2->lines.size(); ++i)
            this->attach(*td2->lines[i]);
    }

    Fl::redraw();
    Fl::wait();

    if (!tree1->is_valid() || !tree2->is_valid())
        return;

    // pair tree node is called here
    this->tb = new tree_buttons(tree1, tree2, td1, td2, RADIUS);
    this->radius = tb->radius;

    if (!tree1->is_isomorphism(*tree2)) {
        fl_font(FL_ITALIC, 50);
        this->errortext = new Graph_lib::Text(Point(200, 200), "Trees are not isomorphic");
        this->attach(*errortext);

        Fl::redraw();
        fl_font(FL_ITALIC, FONTSIZE);
        return;
    }

    Graph_lib::Button *button;

    // create the buttons corresponding to tree_pos
    for (int i = 0; i < tb->button_pos.size(); ++i)
    {
        Point pos = tb->button_pos[i];
        button = new Graph_lib::Button(pos,
                                       2 * tb->radius,
                                       2 * tb->radius,
                                       "",
                                       cb_highlight);
        buttons_highlight.push_back(button);
    }

    // attach the buttons to window
    for (int i = 0; i < buttons_highlight.size(); ++i)
    {
        this->attach(*buttons_highlight[i]);
        buttons_highlight[i]->set_Invisible();
    }

    this->detach(*td1);
    this->detach(*td2);

    // delete tree1;
    // delete tree2;
    // delete c1;
    // delete c2;
    // delete td1;
    // delete td2;
}

void myWindow::cb_highlight(Graph_lib::Address widget, Graph_lib::Address window)
{
    Graph_lib::reference_to<myWindow>(window).highlight((Fl_Widget *)widget);
}

void myWindow::highlight(Fl_Widget *button)
{
    // printf("I am pressed! My loc is (%d, %d)\n",
    //         button->x() + this->radius, button->y() + this->radius);
    
    // the coordinate of node
    Point xy(button->x() + this->radius, button->y() + this->radius);

    map<Node *, Node *> node_to_node = tb->node_to_node;
    map<Node *, coordinate> node_to_pos = tb->node_to_pos;
    map<coordinate, Node *> pos_to_node = tb->pos_to_node;

    // node1 and node2 are paired
    Node *node1, *node2;
    node1 = pos_to_node[std::make_pair(xy.x, xy.y)];
    node2 = node_to_node[node1];
    Point pos_node1 = xy;
    Point pos_node2(node_to_pos[node2].first, node_to_pos[node2].second);

    /* when node1 or node2 is clicked
     * 1. detach all circle
     * 2. all circle shall be drawn black
     * 3. circle at node1 and node2 shall be drawn highlighted
     */

    // detach all circle
    for (int i = 0; i < circles.size(); ++i)
        this->detach(*circles[i]);
    for (int i = 0; i < texts.size(); ++i)
        this->detach(*texts[i]);
    // clear the circles vector
    circles.clear();
    texts.clear();

    // prepare all circle black
    // also prepare the text
    map<coordinate, Node *>::iterator it;
    Graph_lib::Circle *circle;
    Graph_lib::Text *text;
    for (it = pos_to_node.begin(); it != pos_to_node.end(); ++it)
    {
        int x = it->first.first;
        int y = it->first.second;
        Point center_circle(x, y);
        Point center_text(center_circle.x - radius / 4,
                          center_circle.y + radius / 4);
        if (Point(x, y) != pos_node1 && Point(x, y) != pos_node2)
        {
            circle = new Graph_lib::Circle(center_circle, this->radius);
            circles.push_back(circle);
        }
        text = new Graph_lib::Text(center_text, it->second->name);
        text->set_font_size(FONTSIZE);
        text->set_font(FL_ITALIC);
        texts.push_back(text);
    }

    // prepare the two nodes blue
    circle = new Graph_lib::Circle (pos_node1, this->radius);
    circle->set_color(Graph_lib::Color::blue);
    circles.push_back(circle);

    circle = new Graph_lib::Circle (pos_node2, this->radius);
    circle->set_color(Graph_lib::Color::blue);
    circles.push_back(circle);

    // draw all circles
    for (int i = 0; i < circles.size(); ++i)
        this->attach(*circles[i]);

    for (int i = 0; i < texts.size(); ++i)
        this->attach(*texts[i]);
    
    Fl::redraw();
}
