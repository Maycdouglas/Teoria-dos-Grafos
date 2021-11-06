#ifndef TEORIAGRAFOS_NODE_H
#define TEORIAGRAFOS_NODE_H
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
class Node{

    // Attributes
private:
    Edge* first_edge;
    Edge* last_edge;
    int id;
    unsigned int in_degree;
    unsigned int out_degree;
    float weight;
    Node* next_node;
    Node* noPai;

public:
    // Constructor
    Node(int id);
    // Destructor
    ~Node();
    // Getters
    Edge* getFirstEdge();
    Edge* getLastEdge();
    int getId();
    int getInDegree();
    int getOutDegree();
    float getWeight();
    Node* getNextNode();
    Node* getNoPai();
    // Setters
    void setNextNode(Node* node);
    void setWeight(float weight);
    void setNoPai(Node* no);
    // Other methods
    bool searchEdge(int target_id);
    void insertEdge(int target_id, float weight, int origin_id, int id);
    void removeAllEdges();
    int removeEdge(int id, bool directed, Node* target_node);
    void incrementOutDegree();
    void decrementOutDegree();
    void incrementInDegree();
    void decrementInDegree();
    Edge* hasEdgeBetween(int target_id);
    // Auxiliar methods

};
#endif //TEORIAGRAFOS_NODE_H
