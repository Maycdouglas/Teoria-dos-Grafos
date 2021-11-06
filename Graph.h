
#ifndef TEORIAGRAFOS_GRAPH_H
#define TEORIAGRAFOS_GRAPH_H
#include "Node.h"
#include "Edge.h" // Include of the Edge class
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph{

    //Atributes
private:
    int order;
    int number_edges;
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node* first_node;
    Node* last_node;

public:
    //Constructor
    Graph(int order, bool directed, bool weighted_edge, bool weighted_node);
    //Destructor
    ~Graph();
    //Getters
    int getOrder();
    int getNumberEdges();
    bool getDirected();
    bool getWeightedEdge();
    bool getWeightedNode();
    Node* getFirstNode();
    Node* getLastNode();
    //Other methods
    void insertNode(int id, float peso);
    void insertEdge(int id, int target_id, float weight, int aresta_id);
    void removeNode(int id);
    bool searchNode(int id);
    Node* getNode(int id);
    Edge* getEdgeById(int id);


private:
    //Auxiliar methods

};
#endif //TEORIAGRAFOS_GRAPH_H
