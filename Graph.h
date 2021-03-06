
#ifndef TEORIAGRAFOS_GRAPH_H
#define TEORIAGRAFOS_GRAPH_H
#include "Node.h"
#include "Edge.h" // Include of the Edge class
#include <fstream>
#include <stack>
#include <list>
#include <queue>

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
    string fechoTransitivoIndireto(int idRotulo);
    void fechoTransitivoIndiretoAux(int idNoOrigem, int idNoAlvo, bool *verticesVisitados, bool *fechoTransitivoIndireto, bool *alcanca);
    void gerarSubgrafoVerticeInduzido(string *grafo, string *arestaDOT, int idRotulo, bool *verticesVisitados);
    bool visitou(int id, bool *verticesVisitados);
    string dijkstra(int idRotuloInicial, int idRotuloFinal);
    string gerarCaminhoMinimo(Node **vetorPais, Node *noInicial, Node *noFinal);
    int extrairIdMenorCustoDisponivel(float *vetorCustos, list<int> *listaVerticesDisponiveis);
    bool estaNaLista(int idTarget, list<int> *listaVerticesDisponiveis);
    void retirarElementoLista(list<int> *listaVerticesDisponiveis, int verticeMenorCaminhoAtual);
    string floyd(int idRotuloInicial, int idRotuloFinal );
    string gerarCaminhoMinimoFloyd(list<int> *caminho, float *matrizVertices);
    Graph* kruskal(int *subconjuntoVertices, int qntdVertices);
    void ordenarArestasOrdemCrescente(Graph *arvore, queue<Edge*> *filaArestas, queue<Edge*> *filaArestasAux);
    bool estaNaMesmaSubarvore(int *vertices, int idOrigem, int idAlvo);
    void montarCabecalhoGrafoDOT(string *grafo, string *arestaDOT);
    void montarArestaGrafoDOT(string *grafo, string *arestaDOT, int idRotuloNoAtual, int idRotuloNoAlvo, float pesoAresta, bool retorno);
    string ordenacaoTopologica();
    void ordenacaoTopologicaAux(int v, bool visited[],stack<int>& Stack);

private:
    //Auxiliar methods

};
#endif //TEORIAGRAFOS_GRAPH_H
