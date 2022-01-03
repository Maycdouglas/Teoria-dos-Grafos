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

    string grafo, arestaDOT;
    grafo = "strict digraph G {\n";
    arestaDOT = " -> ";

    Node *no = getFirstNode();
    Edge *aresta;
    Node *noAux = nullptr;
    int idNodeSource, idNodeTarget;

    while (no != nullptr) {
        aresta = no->getFirstEdge();
        idNodeSource= no->getIdRotulo();
        if((idNodeSource != idRotulo) && (visitou(idNodeSource,visitados2))){
            while(aresta != nullptr) {
                noAux = getNode(aresta->getTargetId());
                idNodeTarget= noAux->getIdRotulo();

                if(visitou(idNodeTarget, visitados2)) {
                    grafo += "\t" + to_string(idNodeSource) + arestaDOT + to_string(idNodeTarget);
                    grafo += " [weight = " + to_string(aresta->getWeight());
                    grafo += ", label = " + to_string(aresta->getWeight()) + "]\n";
                }

                aresta = aresta->getNextEdge();
            }
        }
        no = no->getNextNode();

    }

    grafo += "}";

    cout << grafo << endl;

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

bool Graph::visitou(int id, int *visitados2) {
    for(int i = 0; i < this->order; i++){
        if(visitados2[i] == id){
            return true;
        }
    }
    return false;
}

string Graph::fechoTransitivoIndireto(int idRotulo) {



    return "maycon";

}

void Graph::fechoTransitivoIndiretoAux(int id, int *visitados) {

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

string Graph::dijkstra(int idRotuloInicial, int idRotuloFinal) {

    float vetorPesos[this->order];
    int idInicial = getNodeByRotulo(idRotuloInicial)->getId();
    int idFinal = getNodeByRotulo(idRotuloFinal)->getId();
    int verticeMenorCaminhoAtual;
    float menorCaminho = INFINITO;
    bool chegou;
    int verticeAux;
    float menorAux;
    Node *noInicial = getNodeByRotulo(idRotuloInicial);
    Node *noAux = noInicial->getNextNode();
    Edge *aresta = noInicial->getFirstEdge();

    list<int> listaVerticesDisponiveis;
    list<int>::iterator it;

    vetorPesos[noInicial->getId() - 1 ] = 0;

    while(noAux != nullptr) {
        listaVerticesDisponiveis.push_back(noAux->getId());
        vetorPesos[noAux->getId() - 1] = INFINITO;
        noAux = noAux->getNextNode();
    }

    for(int i = 0; i < this->order; i++){
        cout<< vetorPesos[i] << endl;
    }

    cout << "======<1" << endl;

    for(it = listaVerticesDisponiveis.begin(); it!=listaVerticesDisponiveis.end();it++){
        //printa os numeros pares começando do inicio da lista
            cout << getNode(*it)->getIdRotulo() << endl;
    }

    while(aresta != nullptr) {
        if(vetorPesos[aresta->getTargetId() - 1] > aresta->getWeight())
        {
            vetorPesos[aresta->getTargetId() - 1] = aresta->getWeight();
            if(aresta->getWeight() < menorCaminho)
            {
                verticeMenorCaminhoAtual = aresta->getTargetId();
                menorCaminho = aresta->getWeight();
                cout << "verticeMenorCaminhoAtual = " << verticeMenorCaminhoAtual << endl;
            }
        }
        aresta = aresta->getNextEdge();
    }

    cout << "======<2" << endl;

    for(int i = 0; i < this->order; i++){
        cout<< vetorPesos[i] << endl;
    }

    for(it = listaVerticesDisponiveis.begin(); it!=listaVerticesDisponiveis.end();it++){
        if(*it == verticeMenorCaminhoAtual)
        {
            listaVerticesDisponiveis.erase(it);
            break;
        }
    }

    cout << "Chegou aqui 1" << endl;

    while( !(listaVerticesDisponiveis.empty()) ){

        cout << verticeMenorCaminhoAtual << endl;
        noAux = getNode(verticeMenorCaminhoAtual);
        cout << "Chegou aqui 2.1 " << endl;
        aresta = noAux->getFirstEdge();
        cout << "Chegou aqui 2.2" << endl;
        menorAux = INFINITO;

        cout << "Chegou aqui 2.3" << endl;



        while(aresta != nullptr) {
            chegou = false;
            cout << "Chegou aqui 2" << endl;
            if(estaNaLista(aresta->getTargetId(),&listaVerticesDisponiveis)){
                cout << "CHEGOU AQUI no PRIMEIRO IF" << endl;
                if(vetorPesos[aresta->getTargetId() - 1] > aresta->getWeight() + menorCaminho)
                {
                    cout << "CHEGOU AQUI no SEGUNDO IF" << endl;
                    vetorPesos[aresta->getTargetId() - 1] = aresta->getWeight() + menorCaminho;
                    cout << "vetorPesos[aresta->getTargetId() - 1 = " << vetorPesos[aresta->getTargetId() - 1] << endl;
                    if(vetorPesos[aresta->getTargetId() - 1] < menorAux) {
                        cout << "Entrou no IF" << endl;
                        menorAux = vetorPesos[aresta->getTargetId() - 1];
                        verticeAux = aresta->getTargetId();
                        chegou = true;
                    }
                }
            }
            aresta = aresta->getNextEdge();

        }
        if(!chegou)
        {
            verticeAux = listaVerticesDisponiveis.front();
        }
        cout << "Chegou aqui 3" << endl;
        cout << "verticeAux" << verticeAux << endl;
        verticeMenorCaminhoAtual = verticeAux;
        menorCaminho = menorAux;
        cout << "Chegou aqui 4" << endl;
        for(it = listaVerticesDisponiveis.begin(); it!=listaVerticesDisponiveis.end();it++){
            cout << "Chegou aqui 5" << endl;
            if(*it == verticeMenorCaminhoAtual)
            {
                cout << "Chegou aqui 6" << endl;
                listaVerticesDisponiveis.erase(it);
                break;
            }
        }
        cout << "Chegou aqui 7" << endl;
    }

    cout << "======<3" << endl;

    for(int i = 0; i < this->order; i++){
        cout<< vetorPesos[i] << endl;
    }

    return "maycon";
}

bool Graph::estaNaLista(int idTarget, list<int> *listaVerticesDisponiveis){

    cout << "CHEGOU NA FUNCAO SEPARADA" << endl;

    for(list<int>::iterator it = listaVerticesDisponiveis->begin(); it!=listaVerticesDisponiveis->end();it++){
        if(*it == idTarget){
            return true;
        }
    }

    return false;
}

string Graph::floyd(int idRotuloInicial, int idRotuloFinal ){
    float matrizVertices[this->order][this->order];
    int i,j,k,l;
    Node *no, *noAux;
    Edge *aresta;

    for(i = 0; i < this->order; i++) {
        no = getNode(i + 1);
        for(j = 0; j < this->order; j++) {
            noAux = getNode(j + 1);
            if(i == j) {
                matrizVertices[i][j] = 0;
            } else if(no->searchEdge(noAux->getId())){
                aresta = no->hasEdgeBetween(noAux->getId());
                matrizVertices[i][j] = aresta->getWeight();
            } else {
                matrizVertices[i][j] = INFINITO;
            }
        }
    }

    cout << "Impressao da matriz inicial: " << endl;

    for(i = 0; i < this->order; i++) {
        for(j = 0; j < this->order; j++) {
            cout << matrizVertices[i][j] << " ";
        }
        cout << endl;
    }

    for( k = 0; k < this->order; k++) {

        for(i = 0; i < this->order; i++) {
            no = getNode(i + 1);
            for(j = 0; j < this->order; j++) {
                noAux = getNode(j + 1);
                if(i != k && j != k && i != j){
                    if(matrizVertices[i][k] != INFINITO && matrizVertices[k][j] != INFINITO){
                        if(matrizVertices[i][j] > matrizVertices[i][k] + matrizVertices[k][j]){
                            matrizVertices[i][j] = matrizVertices[i][k] + matrizVertices[k][j];
                        }
                    }
                }
            }
        }

    }

    cout << "Impressao da matriz final: " << endl;

    for(i = 0; i < this->order; i++) {
        for(j = 0; j < this->order; j++) {
            cout << matrizVertices[i][j] << " ";
        }
        cout << endl;
    }


    return "maycon";
}

Graph* Graph::kruskal(){

    Graph *arvore = new Graph(0,this->directed,this->weighted_edge,this->weighted_node);
    int idRotulo;

    for(int i = 0; i < this->order; i++) {
        idRotulo = getNode(i + 1)->getIdRotulo();
        arvore->insertNode(idRotulo,0);
    }
    for(int i = 0; i < this->order; i++) {
        cout << arvore->getNode(i+1)->getIdRotulo() << " ";
    }

    cout << endl;

    queue<Edge*> filaArestas, filaArestasAux;
    Node *no = this->first_node;
    Edge *aresta;
    bool encontrouPosicao;

    while(no != nullptr){
        cout << "=======" << endl;
        cout << "Estou no Noh " << no->getIdRotulo() << endl;
        cout << "-------" << endl;
        aresta = no->getFirstEdge();
        while(aresta != nullptr){
            cout << "Estou na Aresta com alvo " << getNode(aresta->getTargetId())->getIdRotulo() << endl;
            encontrouPosicao = false;
            if(!aresta->getRetorno()){
                if (filaArestas.empty() && filaArestasAux.empty()){ //verifica se as duas filas estao vazias
                    filaArestas.push(aresta);
                } else if(filaArestasAux.empty()) { //verifica se a fila auxiliar esta vazia
                    if(filaArestas.back()->getWeight() <= aresta->getWeight()){
                        filaArestas.push(aresta);
                    } else{
                        while(!filaArestas.empty()){
                            if(encontrouPosicao || filaArestas.front()->getWeight() <= aresta->getWeight()){
                                cout << "IF Deve ter adicionado aresta com alvo " << getNode(filaArestas.front()->getTargetId())->getIdRotulo() << endl;
                                filaArestasAux.push(filaArestas.front());
                                filaArestas.pop();
                            } else{
                                cout << "ELSE Deve ter adicionado aresta com alvo " << getNode(aresta->getTargetId())->getIdRotulo() << endl;
                                filaArestasAux.push(aresta);
                                encontrouPosicao = true;
                            }
                        }
                    }
                } else if(filaArestas.empty()){ //verifica se a fila principal esta vazia
                    if(filaArestasAux.back()->getWeight() <= aresta->getWeight()) {
                        filaArestasAux.push(aresta);
                    } else {
                        while(!filaArestasAux.empty()){
                            if(encontrouPosicao || filaArestasAux.front()->getWeight() <= aresta->getWeight()){
                                cout << "IF Deve ter adicionado aresta com alvo " << getNode(filaArestasAux.front()->getTargetId())->getIdRotulo() << endl;
                                filaArestas.push(filaArestasAux.front());
                                filaArestasAux.pop();
                            } else{
                                cout << "ELSE Deve ter adicionado aresta com alvo " << getNode(aresta->getTargetId())->getIdRotulo() << endl;
                                filaArestas.push(aresta);
                                encontrouPosicao = true;
                            }
                        }
                    }
                }
            }
            aresta = aresta->getNextEdge();
        }
        no = no->getNextNode();
    }

    cout << "Tamanho da fila principal: " << filaArestas.size() << endl;
    cout << "Tamanho da fila auxiliar: " << filaArestasAux.size() << endl;

    if(filaArestas.empty() && !filaArestasAux.empty()) {
        filaArestas.swap(filaArestasAux);
    }

//    while(!filaArestas.empty()){
//        cout << filaArestas.front()->getWeight() << " ";
//        filaArestas.pop();
//    }

    cout << endl;

    int vertices[arvore->getOrder()];
    for(int i = 0; i < arvore->getOrder(); i++){
        vertices[i] = i + 1;
    }

    for(int i = 0; i < arvore->getOrder(); i++){
        cout << vertices[i] << " ";
    }
    cout << endl;

    int idOrigem, idAlvo, idOrigemRotulo, idAlvoRotulo;

    while(!filaArestas.empty()) {
        idOrigem = filaArestas.front()->getOriginId();
        idAlvo = filaArestas.front()->getTargetId();
        cout << "idOrigem = " << idOrigem << " idAlvo = " << idAlvo << endl;
        if(!estaNaMesmaSubarvore(vertices, idOrigem, idAlvo)){
            idOrigemRotulo = arvore->getNode(idOrigem)->getIdRotulo();
            idAlvoRotulo = arvore->getNode(idAlvo)->getIdRotulo();

            arvore->insertEdge(idOrigemRotulo,idAlvoRotulo,filaArestas.front()->getWeight());

            if(vertices[idOrigem - 1] <= vertices[idAlvo - 1]){
                for(int i = 0; i < arvore->getOrder(); i++){
                    if(vertices[i] == vertices[idAlvo - 1]){
                        vertices[i] = vertices[idOrigem - 1];
                    }
                }
            } else {
                for(int i = 0; i < arvore->getOrder(); i++){
                    if(vertices[i] == vertices[idOrigem - 1]){
                        vertices[i] = vertices[idAlvo - 1];
                    }
                }
            }
        }
        cout << "======" << endl;
        cout << "IMPRIMINDO VETOR: " << endl;
        for(int i = 0; i < arvore->getOrder(); i++){
            cout << vertices[i] << " ";
        }
        cout << endl;
        filaArestas.pop();
    }

    cout << "ORDEM DA ARVORE: " << arvore->getOrder() << endl;
    cout << "NUMERO DE ARESTAS DA ARVORE: " << arvore->getNumberEdges() << endl;

    return arvore;
}

bool Graph::estaNaMesmaSubarvore(int *vertices, int idOrigem, int idAlvo){
    if(vertices[idOrigem - 1] == vertices[idAlvo - 1]){
        cout << vertices[idOrigem - 1] << " = " << vertices[idAlvo - 1] << endl;
        return true;
    }
    return false;
}

string Graph::ordenacaoTopologica()
{   
    if(!this->directed)
    {
        return "ERRO: Grafo não direcionado";
    }
     stack<int> Stack;
    //setando todos os vértices para não visitados
    bool* visited = new bool[this->order];
    for (int i = 0; i < this->order; i++)
        visited[i] = false;

    //recursivamente classifica os vértices um por um
    for (int i = 0; i < this->order; i++){
        cout << "to aqui" << endl;
        if (!visited[i])
            ordenacaoTopologicaAux(i, visited, Stack);
    }

    while (!Stack.empty()) {
        cout << getNode(Stack.top() + 1)->getIdRotulo() << " ";
        Stack.pop();
    }
    return "xiao te amo";
}

void Graph::ordenacaoTopologicaAux(int v, bool visited[],stack<int>& Stack)
{
    //variável com total de vértices e seus adjacentes listados
    int V = this->number_edges;
    list<int> adj;
    // marcando o nó atual como visitado
     visited[v] = true;

    
    // percorre os vértices adjacentes
    list<int>::iterator i;
    for (i = adj.begin(); i != adj.end(); ++i)
        if (!visited[*i])
            ordenacaoTopologicaAux(*i, visited, Stack);

    
    // empilha o vertice atual
    Stack.push(v);
}

//Graph* graph::prim()
//{
//     if (graph->getDirected()) {
//        cout << "Graph is directed" << endl;
//    } else {
//        if (!graph->getWeightedEdge()) {
//            cout << "Graph is not weighted" << endl;
//        } else {
//            graph->agmPrim(output_file);
//        }
//    }
//}