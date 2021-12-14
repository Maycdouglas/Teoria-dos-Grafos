#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cfloat>
#include <iomanip>

#define INFINITO 99999999

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{
    return this->order;
}
int Graph::getNumberEdges()
{
    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{
    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{
    return this->weighted_edge;
}
//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{
    return this->weighted_node;
}

Node *Graph::getFirstNode()
{
    return this->first_node;
}

Node *Graph::getLastNode()
{
    return this->last_node;
}

void Graph::insertNode(int id_rotulo, float peso)
{
    Node *no = new Node(this->order + 1, id_rotulo); //cria um novo nó

    //verifica se o grafo tem peso nos nós
    if(this->getWeightedNode())
    {
        no->setWeight(peso); //insere o peso no novo nó
    }

    if(this->first_node == nullptr)  //verifica se existe nó no grafo, caso não exista, altera o primeiro e ultimo nó
    {
        this->first_node = no;
        this->last_node = no;
    }
    else {                              //caso exista, altera apenas o último nó e o seguinte do que antes era o último

        this->last_node->setNextNode(no);
        this->last_node = no;
    }

    order++;

}

void Graph::insertEdge(int id_rotulo, int target_id_rotulo, float weight)
{

    //Verifica se o nó Inicial existe
    if(!searchNode(id_rotulo))
    {
        insertNode(id_rotulo,0);
    }

    //Verifica se o nó Final existe
    if(!searchNode(target_id_rotulo))
    {
        insertNode(target_id_rotulo,0);
    }

    Node *noInicial = getNodeByRotulo(id_rotulo);   //recebe o nó inicial a partir do ID recebido
    Node *noFinal = getNodeByRotulo(target_id_rotulo);   //recebe o nó final a partir do ID recebido

    int id = noInicial->getId(); //recebe o id do nó inicial
    int target_id = noFinal->getId(); //recebe o id do nó final

    noInicial->insertEdge(target_id, weight, id, false); //insere a aresta a partir do nó inicial

    if(directed) //verifica se é um digrafo
    {
        getNode(id)->incrementOutDegree();
        getNode(target_id)->incrementInDegree();
    }
    else
    {
        // uma aresta auxiliar é adicionada entre os dois nós para o percurso inverso ser possível
//        Node *noAux = getNode(target_id);
        noFinal->insertEdge(id, weight, target_id, true);

        getNode(id)->incrementOutDegree();
        getNode(id)->incrementInDegree();

        getNode(target_id)->incrementOutDegree();
        getNode(target_id)->incrementInDegree();
    }
    this->number_edges ++;
}

void Graph::removeNode(int id){

    Node *no = this->first_node;
    Node *noAux = this->first_node;

    if(this->first_node->getId() == id) ///verifica se o no a ser excluído é o primeiro nó do grafo
    {
        this->first_node = no->getNextNode();
        delete no;
        return;
    }

    no = no->getNextNode();  ///nó recebe o próximo nó

    while (no != nullptr ) ///loop que busca o nó a ser excluído e o seu antecessor
    {
        if (no->getId() == id)
        {
            noAux->setNextNode(no->getNextNode()); ///altera o próximo nó do antecessor para ser o nó sucessor do nó que será excluído

            if (no == last_node) ///verifica se o nó a ser excluído é o último do grafo
            {
                last_node = noAux;
            }

            delete no;
            return;
        }

        noAux = noAux->getNextNode();
        no = no->getNextNode();
    }

}

bool Graph::searchNode(int id)
{
    Node *no = this->first_node;

    while (no != nullptr) ///loop responsavel por encontrar o nó no grafo
    {
        if (no->getIdRotulo() == id) ///condicional que verifica se o ID Rotulo do nó foi encontrado
        {
            return true;
        }
        no = no->getNextNode();
    }

    return false;
}

Node *Graph::getNode(int id)
{
    Node *no = this->first_node;

    while (no != nullptr) ///loop responsavel por encontrar o nó no grafo
    {
        if (no->getId() == id) ///condicional que verifica se o ID do nó foi encontrado
        {
            return no;
        }
        no = no->getNextNode();
    }

    return nullptr;
}

Node *Graph::getNodeByRotulo(int id)
{
    Node *no = this->first_node;

    while (no != nullptr) ///loop responsavel por encontrar o nó no grafo
    {
        if (no->getIdRotulo() == id) ///condicional que verifica se o ID do nó foi encontrado
        {
            return no;
        }
        no = no->getNextNode();
    }

    return nullptr;
}

//NOSSOS METODOS

string Graph::buscaEmLargura(int id) {

    string grafo, arestaDOT;
    Node *no = getNodeByRotulo(id);
    Node *noAux = nullptr;
    Edge *aresta = nullptr;
    Edge *arestaAux = nullptr;
    Graph* arvore = new Graph(0, directed, weighted_edge, weighted_node);
    int idNodeSource;
    int idNodeTarget;
    queue<int> filaVertices;
    filaVertices.push(no->getId());

    if(this->directed)
    {
        grafo = "strict digraph G {\n";
        arestaDOT = " -> ";
    }
    else {
        grafo = "strict graph G {\n";
        arestaDOT = " -- ";
    }

    no->setMarcado(true);

    while(filaVertices.size() != 0){
        aresta = no->getFirstEdge();

        while(aresta != nullptr) {
            noAux = getNode(aresta->getTargetId());
            idNodeSource= no->getIdRotulo();
            idNodeTarget= noAux->getIdRotulo();
            if(noAux->getMarcado() == false){
                grafo += "\t" + to_string(idNodeSource) + arestaDOT + to_string(idNodeTarget);
                grafo += " [weight = " + to_string(aresta->getWeight());
                grafo += ", label = " + to_string(aresta->getWeight()) + "]\n";

                arvore->insertEdge(idNodeSource, idNodeTarget, aresta->getWeight());
                noAux->setMarcado(true);
                aresta->setMarcado(true);
                arestaAux = noAux->hasEdgeBetween(no->getId());
                arestaAux->setMarcado(true);
                filaVertices.push(noAux->getId());
            } else {
                if(!(aresta->getMarcado()))
                {
                    grafo += "\t" + to_string(idNodeSource) + arestaDOT + to_string(idNodeTarget);
                    grafo += " [weight = " + to_string(aresta->getWeight());
                    grafo += ", label = " + to_string(aresta->getWeight());
                    grafo += ", color = red]\n";
                    aresta->setMarcado(true);
                    arestaAux = noAux->hasEdgeBetween(no->getId());
                    arestaAux->setMarcado(true);
                }
            }
            aresta = aresta->getNextEdge();
        }

        filaVertices.pop();

        no = getNode(filaVertices.front());
    }

    grafo += "}";

    cout << grafo << endl;

    return grafo;

}

string Graph::fechoTransitivoDireto(int idRotulo) {

    int id = getNodeByRotulo(idRotulo)->getId();
    int visitados[this->order];
    int visitados2[this->order];

    for(int i = 0; i < this->order; i++){
        visitados[i] = 0;
    }

    for(int l = 0; l < this->order; l++){
        visitados2[l] = -1;
    }

    fechoTransitivoDiretoAux(id, visitados);

    int k, contador = 0;

    for(int m = 0; m < this->order; m++){

        if(visitados[m] == 1){
            k = getNode(m+1)->getIdRotulo();
            visitados2[contador] = k;
            contador++;
        }
    }

    for(int j = 0; j < this->order; j++){
        cout << visitados2[j] << endl;
    }

    return "maycon";

}

void Graph::fechoTransitivoDiretoAux(int id, int *visitados) {

    Node *no = getNode(id);
    Edge *aresta= no->getFirstEdge();

    visitados[id - 1] = 1;

    while(aresta != nullptr) {

        if( (visitados[aresta->getTargetId() - 1] == 0) ){
            fechoTransitivoDiretoAux(aresta->getTargetId(), visitados);
        }

        aresta = aresta->getNextEdge();
    }

}

bool Graph::visitou(int id, int *visitados) {
    for(int i = 0; i < this->order; i++){
        if(visitados[i] == id){
            return true;
        }
    }
    return false;
}



