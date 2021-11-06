#ifndef TEORIAGRAFOS_EDGE_H
#define TEORIAGRAFOS_EDGE_H
using namespace std;

// Definition of the Edge class
class Edge{

    // Attributes
private:
    int target_id;
    int origin_id;
    int id;
    Edge* next_edge;
    float weight;


public:
    // Constructor
    Edge(int target_id, int origin_id, int id);
    // Destructor
    ~Edge();
    // Getters
    int getTargetId();
    Edge* getNextEdge();
    float getWeight();
    int getOriginId();
    int getId();
    // Setters
    void setNextEdge(Edge* edge);
    void setWeight(float weight);


};
#endif //TEORIAGRAFOS_EDGE_H
