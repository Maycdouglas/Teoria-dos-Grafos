#ifndef TEORIAGRAFOS_EDGE_H
#define TEORIAGRAFOS_EDGE_H
using namespace std;

// Definition of the Edge class
class Edge{

    // Attributes
private:
    int target_id;
    int origin_id;
    Edge* next_edge; // lista de arestas do nó origin_id
    float weight;


public:
    // Constructor
    Edge(int target_id, int origin_id);
    // Destructor
    ~Edge();
    // Getters
    int getTargetId();
    Edge* getNextEdge();
    float getWeight();
    int getOriginId();
    // Setters
    void setNextEdge(Edge* edge);
    void setWeight(float weight);


};
#endif //TEORIAGRAFOS_EDGE_H
