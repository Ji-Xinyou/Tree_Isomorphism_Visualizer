#include <string>
#include <cstdlib>
#include <fstream>
#include <stack>
#include <deque>
#include <algorithm>
#include "tree.hpp"

using namespace std;

template<class T>
bool in_vec(vector<T> vec, T elem)
{
    for (auto &v: vec)
    {
        if (v == elem)
            return true;
    }
    return false;
}

string remove_space(string str)
{
    int left = 0, right = str.length() - 1;
    for (int i = 0; i < right; ++i)
    {
        if (str[i] == ' ')
            ++left;
        else 
            break;
    }
    for (int i = str.length() - 1; i >= 0; --i)
    {
        if (str[i] == ' ')
            --right;
        else 
            break;
    }

    str = str.substr(left, right - left + 1);
    return str;
}

// return a vector of parent and child
// e.g. A->B ---> vec[0] = "A", vec[1] = "B"
vector<string> process_string(string str)
{
    string parent, child;
    vector<string> vec;

    int pos = str.find("->");
    parent = str.substr(0, pos);
    child = str.substr(pos + 2);

    // remove the space at the beginning and the end
    parent = remove_space(parent);
    child = remove_space(child);

    vec.push_back(parent);
    vec.push_back(child);

    return vec;
}

/* ------------------------- CLASS MEMBER OF mytree ------------------------- */

void mytree::build_map_from_file(string filename)
{
    // logger debug
    bool debug = false;

    // saving the parent and child each time
    // ret[0]: parent   ret[1]: child
    vector<string> ret;

    fstream file;
    string str;
    string parent, child;

    file.open(filename, ios::in);
    if (!file)
        printf("File open failed\n");
    else
    {
        while (getline(file, str))
        {
            ret = process_string(str);
            parent = ret[0];
            child = ret[1];
            // no duplicated child
            if (!in_vec(parent_child_map[parent], child))
                parent_child_map[parent].push_back(child);
        }
    }

    file.close();

    // DEBUG SESSION
    if (debug)
    {
        map<string, vector<string> >::iterator it;
        for (it = parent_child_map.begin(); it != parent_child_map.end(); ++it)
        {
            cout << it->first << ' ';
            for (int i = 0; i < it->second.size(); i++)
                cout << it->second[i] << ' ';
            cout << endl;
        }
    }
}

string mytree::find_root()
{
    map<string, vector<string> >::iterator it;
    map<string, bool> is_root;
    for (it = parent_child_map.begin(); it != parent_child_map.end(); it++)
    {
        is_root[it->first] = true;
        for (int i = 0; i < it->second.size(); i++)
            is_root[it->second[i]] = true;
    }
    for (it = parent_child_map.begin(); it != parent_child_map.end(); it++)
    {
        vector<string> childs = it->second;
        for (int i = 0; i < childs.size(); ++i)
            is_root[childs[i]] = false;
    }
    map<string, bool>::iterator iter;
    for (iter = is_root.begin(); iter != is_root.end(); iter++)
    {
        if (iter->second)
            return iter->first;
    }
    return "NoRoot";
}

void mytree::build_tree_from_map()
{
    bool debug = false;

    vector<string> tmp;
    // first find the root of the tree
    // the root has no node pointing at it
    string rt = find_root();

    Node *cur = new Node(rt);

    stack<Node *> mystack;
    mystack.push(cur);
    while (!mystack.empty())
    {
        Node *topNode = mystack.top();
        mystack.pop();

        // assign root node
        if (topNode->name == rt)
            root = topNode;

        // push the rest of the childs
        tmp = parent_child_map[topNode->name];
        for (int i = 0; i < tmp.size(); i++)
        {
            Node *newNode = new Node(tmp[i]);
            mystack.push(newNode);
            // link the parent and childs
            topNode->childs.push_back(newNode);
            newNode->parent = topNode;

            // DEBUG SESSION
            if (debug)
                printf("%s->%s\n", topNode->name.c_str(), newNode->name.c_str());
        }
    }
}

void mytree::traverse()
{
    Node *rt = root;
    deque<Node *> q;
    q.push_back(rt);
    while (!q.empty())
    {
        Node *n = q.front();
        q.pop_front();

        printf("%s\n", n->name.c_str());
        vector<Node *> childs = n->childs;
        for (int i = 0; i < childs.size(); ++i)
        {
            q.push_back(childs[i]);
        }
    }
}

pair<string, vector<Node *> > mytree::encode(Node *n)
{
    if (n->childs.empty())
    {
        vector<Node *> vec;
        vec.push_back(n);
        return make_pair("01", vec);
    }
    else
    {
        vector<Node *> childs = n->childs;
        vector<pair<string, vector<Node *> > > childs_encoding;
        pair<string, vector<Node *> > ce;
        // acquire all childs encoding and childlist
        // sort them in order and line up
        for (int i = 0; i < childs.size(); ++i)
        {
            ce = encode(childs[i]);
            childs_encoding.push_back(ce);
        }
        // sort by encoding (lex)
        sort(childs_encoding.begin(), childs_encoding.end());

        vector<pair<string, vector<Node *> > >::iterator it;
        // encoding and childlist of Node *n
        string my_encoding = "0";
        vector<Node *> my_list;

        for (it = childs_encoding.begin();
             it != childs_encoding.end();
             it++)
        {
            my_encoding += it->first;
            // push the childlist of all childs
            vector<Node *> childlist = it->second;
            for (int i = 0; i < childlist.size(); ++i)
                my_list.push_back(childlist[i]);
        }

        my_list.push_back(n);

        my_encoding += "1";

        return make_pair(my_encoding, my_list);
    }
}

pair<string, vector<Node *> > mytree::get_encoding()
{
    return encode(root);
}

void mytree::debug_encode()
{
    stack<Node *> mystack;
    mystack.push(root);
    while (!mystack.empty())
    {
        Node *top = mystack.top();
        mystack.pop();

        printf("Encoding of Node %s is %s\n",
               top->name.c_str(),
               encode(top).first.c_str());
        printf("Childlist: ");
        for (int i = 0; i < encode(top).second.size(); ++i)
            cout << encode(top).second[i]->name << ' ';
        cout << endl;

        for (int i = 0; i < top->childs.size(); ++i)
        {
            mystack.push(top->childs[i]);
        }
    }
}

bool mytree::is_isomorphism(mytree t)
{
    return get_encoding().first == t.get_encoding().first;
}

mytree::mytree(string filename)
{
    this->build_map_from_file(filename);
    this->build_tree_from_map();
    this->check_validity();
}

/**
 * @brief 
 * 
 * @param t1 first tree to be paired
 * @param t2 second tree to be paired
 * @return map<Node*, Node*>
 * the map represents the isomorphism-mapping relation
 * 
 * if node A is mapped to node 1, the mapping will be (A, 1)
 * However, the mapping is symmetrical, so (1, A) will also be added
 * 
 * the vector means that it contains all the mapping relation
 */
map<Node *, Node *> mytree::pair_tree_Nodes(mytree t)
{
    bool debug = false;

    map<Node *, Node *> mapping;

    pair<string, vector<Node *> > encoding_and_list_1, encoding_and_list_2;
    encoding_and_list_1 = this->get_encoding();
    encoding_and_list_2 = t.get_encoding();

    string encoding_t1 = encoding_and_list_1.first;
    string encoding_t2 = encoding_and_list_2.first;
    try
    {
        // double check
        if (encoding_t1 != encoding_t2)
            throw runtime_error("Trees are not isomorphic!");
        vector<Node *> clist_t1, clist_t2;
        clist_t1 = encoding_and_list_1.second;
        clist_t2 = encoding_and_list_2.second;

        if (debug)
            cout << "In pair_tree_nodes(): " << endl;

        // clist_t1 and clist_t2 should be of the same size
        for (int i = 0; i < clist_t1.size(); i++)
        {
            Node *n1 = clist_t1[i];
            Node *n2 = clist_t2[i];
            mapping[n1] = n2;
            mapping[n2] = n1;

            if (debug)
                printf("Map %s -> %s\n", n1->name.c_str(), n2->name.c_str());
        }
    }
    catch (exception &e)
    {
        cerr << e.what();
    }

    return mapping;
}

void mytree::check_validity() {
    map<string, vector<string> >::iterator it;
    map<string, vector<string> > child_parent_map; 

    for (it = parent_child_map.begin(); it != parent_child_map.end(); it++) {
        // cerr << it->first << " -> " ;
        for (auto _it = it->second.begin(); _it != it->second.end(); _it++)
            //cerr << *_it;
            child_parent_map[*_it].push_back(it->first);
        // cerr << endl;
    }

    // for (it = child_parent_map.begin(); it != child_parent_map.end(); it++) {
    //     cerr << it->first << " -> " ;
    //     for (auto _it = it->second.begin(); _it != it->second.end(); _it++)
    //         cerr << *_it;
    //     cerr << endl;
    // }

    is_Tree = true;

    for (it = child_parent_map.begin(); it != child_parent_map.end(); it++) {
        if (it->second.size() > 1)
            is_Tree = false;
    }
}


/* ------------------------- CLASS MEMBER OF mytree ------------------------- */

void debug_show_mapping(map<Node *, Node *> mapping)
{
    map<Node *, Node *>::iterator it;
    cout << "In debug_show_mapping() " << endl;
    for (it = mapping.begin(); it != mapping.end(); it++)
    {
        printf("%s maps to %s\n",
               it->first->name.c_str(),
               it->second->name.c_str());
    }
}
