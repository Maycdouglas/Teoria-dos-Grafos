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
    bool retorno;
    bool vermelho;
    bool marcado;


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
    bool getRetorno();
    bool getVermelho();
    bool getMarcado();
    // Setters
    void setNextEdge(Edge* edge);
    void setWeight(float weight);
    void setRetorno(bool retorno);
    void setVermelho(bool vermelho);
    void setMarcado(bool marcado);


};
#endif //TEORIAGRAFOS_EDGE_H
