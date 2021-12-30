
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
    void fechoTransitivoDiretoAux(int id, bool *verticesVisitados);
    string fechoTransitivoIndireto(int id);
    void fechoTransitivoIndiretoAux(int id, bool *visitados);
    void gerarSubgrafoVerticeInduzido(string *grafo, string *arestaDOT, int idRotulo, bool *verticesVisitados);
    bool visitou(int id, bool *verticesVisitados);
    string dijkstra(int idRotuloInicial, int idRotuloFinal);
    bool estaNaLista(int idTarget, list<int> *listaVerticesDisponiveis);
    void retirarElementoLista(list<int> *listaVerticesDisponiveis, int verticeMenorCaminhoAtual);
    string floyd(int idRotuloInicial, int idRotuloFinal );
    Graph* kruskal();
    bool estaNaMesmaSubarvore(int *vertices, int idOrigem, int idAlvo);
    void montarCabecalhoGrafoDOT(string *grafo, string *arestaDOT);
    void montarArestaGrafoDOT(string *grafo, string *arestaDOT, int idRotuloNoAtual, int idRotuloNoAlvo, float pesoAresta, bool retorno);


private:
    //Auxiliar methods

};
#endif //TEORIAGRAFOS_GRAPH_H
