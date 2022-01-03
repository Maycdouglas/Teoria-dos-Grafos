
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
    Node* first_node; //lista de nós do grafo
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
    void insertEdge(int id_rotulo, int target_id_rotulo, float weight);
    void removeNode(int id);
    bool searchNode(int id);
    Node* getNode(int id);
    Node* getNodeByRotulo(int id);

    //Nossos Metodos
    string buscaEmLargura(int id);
    string fechoTransitivoDireto(int id);
    void fechoTransitivoDiretoAux(int id, int *visitados);
    string fechoTransitivoIndireto(int id);
    void fechoTransitivoIndiretoAux(int id, int *visitados);
    bool visitou(int id, int*visitados2);
    string dijkstra(int idRotuloInicial, int idRotuloFinal);
    bool estaNaLista(int idTarget, list<int> *listaVerticesDisponiveis);
    string floyd(int idRotuloInicial, int idRotuloFinal );
    Graph* kruskal();
    bool estaNaMesmaSubarvore(int *vertices, int idOrigem, int idAlvo);
    Graph* prim();
    string ordenacaoTopologica(Graph *graph);
    void ordenacaoTopologicaAux(int v, bool visited[],stack<int>& Stack);

private:
    //Auxiliar methods

};
#endif //TEORIAGRAFOS_GRAPH_H
