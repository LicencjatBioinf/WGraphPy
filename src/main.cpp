#include <pybind11/pybind11.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;

// An AVL tree node
class Node
{
public:
     long double key;
     long double prob;
     long double flow;
    Node* left;
    Node* right;
    Node* predecessor;
    Node* successor;
    Node* clone;
    int height;
};


class RPath {
public:
    Node* start;
    Node* stop;
     long double cost;
     long double tmp_cost;
     long double anty;
    RPath(Node* start)
    {
        this->start = start;
        this->stop = start->successor;
        this->cost = 0;
        if (stop != NULL) {
            int sign = (start->flow >= 0) ? 1 : -1;
            this->tmp_cost = (stop->key - start->key) * sign;
            if (this->tmp_cost > this->cost)this->cost = this->tmp_cost;
            this->anty = (this->start->flow >= 0) ? -1 : this->start->flow;

        }
        else this->cost = std::numeric_limits< long double>::infinity();;

    }
    void elongate() {
        Node* tmp = stop;
        this->stop = stop->successor;
        if (stop != NULL) {
            int sign = (tmp->flow >= 0) ? 1 : -1;
            this->tmp_cost += (stop->key - tmp->key) * sign;
            if (this->tmp_cost > this->cost)this->cost = this->tmp_cost;
            if (this->anty < tmp->flow && tmp->flow < 0.)
                this->anty = tmp->flow;

        }
        else this->cost = std::numeric_limits< long double>::infinity();;
    }
     long double actualize() {
         long double to_transport = min({ start->prob,stop->prob,abs(anty) });
        this->anty = -1.;
        Node* node = this->start;
         long double dist = 0.;
        while (node != this->stop) {
            int sign = (node->flow >= 0) ? 1 : -1;
            node->flow += to_transport;
            dist += to_transport * (node->successor->key - node->key) * sign;
            if (node->flow == 0) {
                this->cost += (node->successor->key - node->key);
                this->tmp_cost += (node->successor->key - node->key);
            }
            if ((anty < node->flow) && (node->flow < 0.)) {
                this->anty = node->flow;
            }
            node = node->successor;


        }
        this->start->prob -= to_transport;
        this->stop->prob -= to_transport;

        return dist;

    }

};
class LPath {
public:
    Node* start;
    Node* stop;
     long double cost;
     long double tmp_cost;
     long double anty;
    LPath(Node* start)
    {
        this->start = start;
        this->stop = start->predecessor;
        this->cost = 0;
        if (stop != NULL) {
            int sign = (start->flow <= 0) ? 1 : -1;
            this->tmp_cost = (start->key - stop->key) * sign;
            if (this->tmp_cost > this->cost)this->cost = this->tmp_cost;
            this->anty = (stop->flow <= 0) ? 1 : this->stop->flow;

        }
        else this->cost = std::numeric_limits< long double>::infinity();;

    }
    void elongate() {
        Node* tmp = stop;
        this->stop = stop->predecessor;
        if (stop != NULL) {
            int sign = (tmp->flow <= 0) ? 1 : -1;
            this->tmp_cost += (tmp->key - stop->key) * sign;
            if (this->tmp_cost > this->cost)this->cost = this->tmp_cost;
            if (this->anty > tmp->flow && tmp->flow > 0.)
                this->anty = tmp->flow;
        }
        else this->cost = std::numeric_limits< long double>::infinity();;
    }
     long double actualize() {
         long double to_transport = min({ start->prob,stop->prob,anty });
        this->anty = 1.;
        Node* node = this->stop;
         long double dist = 0.;
        while (node != this->start) {
            int sign = (node->flow <= 0) ? 1 : -1;
            node->flow -= to_transport;
            dist += to_transport * (node->successor->key - node->key) * sign;
            if (node->flow == 0) {
                this->cost += (node->successor->key - node->key);
                this->tmp_cost += (node->successor->key - node->key);
            }
            if ((anty > node->flow) && (node->flow > 0.)) {
                this->anty = node->flow;
            }
            node = node->successor;


        }
        this->start->prob -= to_transport;
        this->stop->prob -= to_transport;
        return dist;

    }

};

class WGraph {


public:
    Node* root;
     long double wdist;
    bool flg;
     long double transported;
    WGraph() {
        this->wdist = 0.;
        this->transported = 0.;
        this->flg = 1;
        this->root = NULL;
    }

    Node* newNode( long double key,  long double prob)
    {
        Node* node = new Node();
        node->key = key;
        node->prob = prob;
        node->flow = 0.;
        node->left = NULL;
        node->right = NULL;
        node->successor = NULL;
        node->predecessor = NULL;
        node->clone = NULL;

        node->height = 1; // new node is initially
                          // added at leaf
        return(node);
    }

    // A utility function to get the
    // height of the tree
    Node* deep_copy(Node* node) {

        if (node == NULL)
        {
            return node;
        }
        else {
            Node* tmp = newNode(node->key, node->prob);
            tmp->left = deep_copy(node->left);
            tmp->right = deep_copy(node->right);
            node->clone = tmp;
            tmp->clone = node;
            return tmp;


        }
    }
 void reset_clones(Node* node) {
        if (node != NULL) {
            node->clone = NULL;
            reset_clones(node->left);
            reset_clones(node->right);
        }
    }
 Node* find_min() {
     Node* node = this->root;
     while (node->left != NULL)
     {
         node = node->left;
     }
     return node;

 }
 WGraph copy_graph() {
     WGraph* nowy;
     WGraph nowe;
     nowy = &nowe;
     nowy->wdist = 0;
     nowy->transported = 0;
     nowy->flg = 1;
     nowy->root = deep_copy(this->root);
     Node* node = this->find_min();
     node->clone->successor = node->successor->clone;
     node = node->successor;
     while (node->successor != NULL) {
         node->clone->successor = node->successor->clone;
         node->clone->predecessor = node->predecessor->clone;
         node = node->successor;
     }node->clone->successor = NULL;
     reset_clones(this->root);
     reset_clones(nowy->root);
     return *nowy;

 }



    int height(Node* N)
    {
        if (N == NULL)
            return 0;
        return N->height;
    }

    // A utility function to get maximum
    // of two integers
    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }
    // A utility function to right
    // rotate subtree rooted with y
    // See the diagram given above.
    Node* rightRotate(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left),
            height(y->right)) + 1;
        x->height = max(height(x->left),
            height(x->right)) + 1;

        return x;
    }

    // A utility function to left
    // rotate subtree rooted with x
    // See the diagram given above.
    Node* leftRotate(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left),
            height(x->right)) + 1;
        y->height = max(height(y->left),
            height(y->right)) + 1;

        return y;
    }

    // Get Balance factor of node N
    int getBalance(Node* N)
    {
        if (N == NULL)
            return 0;
        return height(N->left) - height(N->right);
    }



    Node* insert(Node* node, Node* nowy)
    {
        

        if (node == NULL)
            return(nowy);

        else if (nowy->key < node->key)
        {
            if (node->left == NULL)
            {
                node->left = nowy;
                Node* tmp = node->predecessor;
                nowy->predecessor = tmp;
                if (tmp != NULL)
                {
                    tmp->successor = nowy;
                    nowy->flow = tmp->flow;
                }
                nowy->successor = node;
                node->predecessor = nowy;

            }
            else node->left = insert(node->left, nowy);
        }
        else if (nowy->key > node->key)
        {
            if (node->right == NULL)
            {
                node->right = nowy;
                Node* tmp = node->successor;
                if (tmp != NULL)
                {
                    tmp->predecessor = nowy;
                }
                nowy->successor = tmp;
                nowy->predecessor = node;
                nowy->flow = node->flow;
                node->successor = nowy;
            }

            else node->right = insert(node->right, nowy);
        }
        else {
            if (nowy->prob <= node->prob) { this->flg = 0; }
            node->prob = abs(node->prob - nowy->prob);
            return node;
        }


        
        node->height = 1 + max(height(node->left),
            height(node->right));

        int balance = getBalance(node);

        // If this node becomes unbalanced, then
        // there are 4 cases

        // Left Left Case
        if (balance > 1 && nowy->key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && nowy->key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && nowy->key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && nowy->key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        /* return the (unchanged) node pointer */
        return node;
    }


     long double transport_prob(Node* node,  long double w) {
        long double wasser = w;
        RPath r = RPath(node);
        LPath l = LPath(node);

        while (node->prob > 0) {
            if ((r.cost == l.cost) && (r.cost == std::numeric_limits< long double>::infinity()))
            {
                return wasser;
            }

            if (r.cost < l.cost) {
                if (r.stop->prob > 0) wasser += r.actualize();
                else r.elongate();
            }
            else {
                if (l.stop->prob > 0) wasser += l.actualize();
                else l.elongate();
            }



        }   return wasser;
    }


    void preOrder(Node* root)
    {
        if (root != NULL)
        {
            cout << root->key << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }
    long double longest()
    {
        Node* node = find_min();
        long double a, b;
        a = 0.;
        b = 0.;
        int c = (node->flow > 0)? 1:-1;
        while (node->successor != NULL) {
            if (node->flow * c < 0) {
                if (a > b) { b = a; }
                c *= -1;
                a = 0;
            }
            a += abs(node->successor->key - node->key);
            node = node->successor;
        }
        if (a > b) { b = a; }
        return b;
    }
    void add_outflow( long double mass,  long double prob) {
        this->root = insert(this->root, newNode(mass, prob));
    }
    void add_inflow( long double mass,  long double prob) {
        Node* node = newNode(mass, prob);
        this->root = insert(this->root, node);
        if (this->flg == 1)
        {
            this->wdist = transport_prob(node, this->wdist);
        }
        else { this->flg = 1; }
        this->transported += prob;
    }


};













PYBIND11_MODULE(WGraphCpp, module) {
	module.doc() = "WGraph";
    pybind11::class_<WGraph>(module, "WGraphCpp")
        .def(pybind11::init<>())
        .def("add_inflow", &WGraph::add_inflow)
        .def("add_outflow", &WGraph::add_outflow)
        .def("copy", &WGraph::copy_graph)
        .def("longest_edge", &WGraph::longest)
        .def_readonly("wdist", &WGraph::wdist)
        .def_readonly("transported", &WGraph::transported);
}