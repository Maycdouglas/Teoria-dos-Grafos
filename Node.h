#ifndef TEORIAGRAFOS_NODE_H
#define TEORIAGRAFOS_NODE_H
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
class Node{

    // Attributes
private:
    Edge* first_edge; //lista de arestas do nó
    Edge* last_edge;
    int id;
    int idRotulo;
    unsigned int in_degree;
    unsigned int out_degree;
    float weight;
    Node* next_node; //lista de nós do grafo
    bool marcado;

public:
    // Constructor
    Node(int id, int idRotulo);
    // Destructor
    ~Node();
    // Getters
    Edge* getFirstEdge();
    Edge* getLastEdge();
    int getId();
    int getIdRotulo();
    int getInDegree();
    int getOutDegree();
    float getWeight();
    Node* getNextNode();
    bool getMarcado();
    // Setters
    void setNextNode(Node* node);
    void setWeight(float weight);
    void setMarcado(bool marcado);
    // Other methods
    bool searchEdge(int target_id);
    void insertEdge(int target_id, float weight, int origin_id, bool retorno, int target_id_rotulo, int origin_id_rotulo);
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
