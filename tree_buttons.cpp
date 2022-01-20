#include "tree_buttons.hpp"

/**
 * @brief Construct a new tree buttons::tree buttons object
 * 
 * @param tree1 
 * @param tree2 
 * @param td1 treedraw object
 * @param td2 
 * @param r radius
 */
tree_buttons::tree_buttons(mytree *tree1,
                           mytree *tree2,
                           Tree_Draw *td1,
                           Tree_Draw *td2,
                           int r) : 

                           t1(tree1), t2(tree2), 
                           td1(td1), td2(td2),
                           radius(r)
{
    get_node_map();
    get_pos_map();
    generate_button_pos();
}

/**
 * @brief get the mapping of nodes
 * 
 */
void tree_buttons::get_node_map()
{
    this->node_to_node = t1->pair_tree_Nodes(*t2);
}

/**
 * @brief get the mapping of positions
 * 
 */
void tree_buttons::get_pos_map()
{
    map<Node *, Point> pos_1 = td1->node_pos;
    map<Node *, Point> pos_2 = td2->node_pos;

    map<Node *, Point>::iterator it;
    for (it = pos_1.begin(); it != pos_1.end(); it++)
    {
        node_to_pos[it->first] = std::make_pair(it->second.x, it->second.y);
        pos_to_node[std::make_pair(it->second.x, it->second.y)] = it->first;
        // printf("(%d, %d) -> %s\n", it->second.x, it->second.y, it->first->name.c_str());
    }

    for (it = pos_2.begin(); it != pos_2.end(); it++)
    {
        node_to_pos[it->first] = std::make_pair(it->second.x, it->second.y);
        pos_to_node[std::make_pair(it->second.x, it->second.y)] = it->first;
        // printf("(%d, %d) -> %s\n", it->second.x, it->second.y, it->first->name.c_str());
    }
}

/**
 * @brief generate the buttons' positions
 * 
 */
void tree_buttons::generate_button_pos()
{
    map<Node *, coordinate>::iterator it;
    double r = RADIUS;
    for (it = node_to_pos.begin(); it != node_to_pos.end(); it++)
    {
        Point pos (it->second.first, it->second.second);
        Point topleft(pos.x - r, pos.y - r);
        button_pos.push_back(topleft);
    }
}