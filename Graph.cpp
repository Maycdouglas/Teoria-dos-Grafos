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

    noInicial->insertEdge(target_id, weight, id, false, target_id_rotulo, id_rotulo); //insere a aresta a partir do nó inicial

    if(directed) //verifica se é um digrafo
    {
        getNode(id)->incrementOutDegree();
        getNode(target_id)->incrementInDegree();
    }
    else
    {
        // uma aresta auxiliar é adicionada entre os dois nós para o percurso inverso ser possível
//        Node *noAux = getNode(target_id);
        noFinal->insertEdge(id, weight, target_id, true, target_id_rotulo, id_rotulo);

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

    //DEVE FUNCIONAR PARA OS GRAFOS E DIGRAFOS!!!

    Node *noAtual = getNodeByRotulo(id); //aponta para o nó de acordo com o idRotulo recebido

    //Clausula de segurança, para caso insira um ID inválido
    if(noAtual == nullptr){
        cout << "Este ID nao existe no grafo!" << endl;
        return "";
    }

    Node *noAlvo; //cria um nó ponteiro auxiliar
    Edge *arestaAtual, *arestaRetorno; //cria uma aresta e um aresta auxiliar
    int idRotuloNoAtual, idRotuloNoAlvo; //variaveis para receber o id dos nodes
    queue<int> filaVertices;  //cria uma fila de ID dos vertices
    string grafo, arestaDOT; //cria as variaveis responsaveis pelo grafo na linguagem DOT
    bool retorno;

    montarCabecalhoGrafoDOT(&grafo,&arestaDOT); //monta o cabeçalho do grafo

    filaVertices.push(noAtual->getId()); //insere o ID do no recebido na lista

    noAtual->setMarcado(true); //marca o no inicial

    while(!filaVertices.empty()){ //loop responsável por percorrer os nós da fila de vertices
        arestaAtual = noAtual->getFirstEdge(); //aponta para a primeira aresta no Nó Atual

        while(arestaAtual != nullptr) {
            noAlvo = getNode(arestaAtual->getTargetId()); //no auxiliar recebe o nó que a aresta atual aponta
            idRotuloNoAtual= noAtual->getIdRotulo(); //recebe o id rotulo do nó atual
            idRotuloNoAlvo= noAlvo->getIdRotulo(); //recebe o id rotulo do nó alvo atual
            retorno = false;

            if(!noAlvo->getMarcado()){  //verifica se o nó alvo não está está marcado
                //caso não esteja, a string do grafo é atualizada
                montarArestaGrafoDOT(&grafo,&arestaDOT,idRotuloNoAtual,idRotuloNoAlvo,arestaAtual->getWeight(), retorno);

                //o no alvo, a aresta e a aresta de retorno sao marcados
                noAlvo->setMarcado(true);
                arestaAtual->setMarcado(true);
                if(!this->getDirected()){
                    arestaRetorno = noAlvo->hasEdgeBetween(noAtual->getId());
                    arestaRetorno->setMarcado(true);
                }
                filaVertices.push(noAlvo->getId()); //o no alvo é inserido na fila
            } else if(!(arestaAtual->getMarcado())){ //verifica se a aresta nao foi marcada
                retorno = true;
                montarArestaGrafoDOT(&grafo,&arestaDOT,idRotuloNoAtual,idRotuloNoAlvo,arestaAtual->getWeight(), retorno);

                //marca a aresta atual e a aresta de retorno
                arestaAtual->setMarcado(true);
                if(!this->getDirected()){
                    arestaRetorno = noAlvo->hasEdgeBetween(noAtual->getId());
                    arestaRetorno->setMarcado(true);
                }
            }
            arestaAtual = arestaAtual->getNextEdge(); //vai para a proxima aresta do no atual
        }
        filaVertices.pop(); //retira o primeiro elemento da fila
        noAtual = getNode(filaVertices.front()); //aponta para o proximo nó da fila
    }

    grafo += "}";

    cout << grafo << endl;

    return grafo;
}

void Graph::montarCabecalhoGrafoDOT(string *grafo, string *arestaDOT){
    if(this->directed) //verifica se o grafo é direcionado
    {
        *grafo = "strict digraph G {\n";
        *arestaDOT = " -> ";
    }
    else {
        *grafo = "strict graph G {\n";
        *arestaDOT = " -- ";
    }
}

void Graph::montarArestaGrafoDOT(string *grafo, string *arestaDOT, int idRotuloNoAtual, int idRotuloNoAlvo, float pesoAresta, bool retorno){
    *grafo += "\t" + to_string(idRotuloNoAtual) + *arestaDOT + to_string(idRotuloNoAlvo);
    *grafo += " [weight = " + to_string(pesoAresta);
    *grafo += ", label = " + to_string(pesoAresta);
    if(retorno){
        *grafo += ", color = red";
    }
    *grafo += "]\n";
}

string Graph::fechoTransitivoDireto(int idRotulo) {

    //Clausula de segurança, para caso o grafo nao seja direcionado
    if(!this->directed){
        cout << "Este grafo nao eh direcionado!" << endl;
        return "";
    }

    //Clausula de segurança, para caso insira um ID inválido
    if(getNodeByRotulo(idRotulo) == nullptr){
        cout << "Este ID nao existe no grafo!" << endl;
        return "";
    }

    string grafo, arestaDOT;
    montarCabecalhoGrafoDOT(&grafo,&arestaDOT);

    int idNoOrigem = getNodeByRotulo(idRotulo)->getId(); //recebe ID do No Inicial
    bool verticesVisitados[this->order];

    for(int i = 0; i < this->order; i++){ //seta todos os elementos do array com 0
        verticesVisitados[i] = false;
    }

    fechoTransitivoDiretoAux(idNoOrigem, verticesVisitados); //inicia busca em profundidade

    gerarSubgrafoVerticeInduzido(&grafo,&arestaDOT,idRotulo,verticesVisitados);

    cout << grafo << endl;

    return grafo;

}

void Graph::fechoTransitivoDiretoAux(int idNoOrigem, bool *verticesVisitados) {

    Node *noAtual = getNode(idNoOrigem);
    Edge *arestaAtual = noAtual->getFirstEdge();

    verticesVisitados[idNoOrigem - 1] = true;

    while(arestaAtual != nullptr) {

        if(!verticesVisitados[arestaAtual->getTargetId() - 1]){
            fechoTransitivoDiretoAux(arestaAtual->getTargetId(), verticesVisitados);
        }

        arestaAtual = arestaAtual->getNextEdge();
    }

}

void Graph::gerarSubgrafoVerticeInduzido(string *grafo, string *arestaDOT, int idRotuloNoOrigem, bool *verticesVisitados){
    Node *noAtual, *noAlvo;
    Edge *arestaAtual;
    int idRotuloNoAtual, idRotuloNoAlvo, idNoAtual, idNoAlvo;

    for(int i = 0; i < this->order; i++){
        if(verticesVisitados[i]){
            noAtual = getNode(i + 1);
            arestaAtual = noAtual->getFirstEdge();
            idRotuloNoAtual = noAtual->getIdRotulo();
            idNoAtual = noAtual->getId();

            if((idRotuloNoAtual != idRotuloNoOrigem))
            {
                while(arestaAtual != nullptr){
                    noAlvo = getNode(arestaAtual->getTargetId());
                    idRotuloNoAlvo = noAlvo->getIdRotulo();
                    idNoAlvo = noAlvo->getId();

                    if(visitou(idNoAlvo, verticesVisitados) && idRotuloNoAlvo != idRotuloNoOrigem) {
                        montarArestaGrafoDOT(grafo,arestaDOT,idRotuloNoAtual,idRotuloNoAlvo,arestaAtual->getWeight(),false);
                    }
                    arestaAtual = arestaAtual->getNextEdge();
                }
            }
        }
    }
    *grafo += "}";
}

bool Graph::visitou(int id, bool *verticesVisitados) {

    if(verticesVisitados[id - 1]){
        return true;
    }
    return false;
}

string Graph::fechoTransitivoIndireto(int idRotulo) {

//Clausula de segurança, para caso o grafo nao seja direcionado
    if(!this->directed){
        cout << "Este grafo nao eh direcionado!" << endl;
        return "";
    }

    //Clausula de segurança, para caso insira um ID inválido
    if(getNodeByRotulo(idRotulo) == nullptr){
        cout << "Este ID nao existe no grafo!" << endl;
        return "";
    }

    string grafo, arestaDOT;
    montarCabecalhoGrafoDOT(&grafo,&arestaDOT);

    bool verticesVisitados[this->order];
    list<int> fechoTransitivoIndireto;
    list<int> caminho;

    for(int i = 0; i < this->order; i++){ //seta todos os elementos do array com 0
        verticesVisitados[i] = false;
    }

    for(int i = 0; i < this->order; i++){
        fechoTransitivoIndiretoAux(i + 1, getNodeByRotulo(idRotulo)->getId(), verticesVisitados, &fechoTransitivoIndireto, &caminho); //inicia busca em profundidade
    }

    cout << "Tamanho da lista fecho: " << fechoTransitivoIndireto.size() << endl;

    cout << "Tamanho da lista caminho: " << caminho.size() << endl;

    for(list<int>::iterator it = fechoTransitivoIndireto.begin(); it!=fechoTransitivoIndireto.end();it++){
        cout << *it << " ";
    }

    cout << endl;

    for(list<int>::iterator it = caminho.begin(); it!=caminho.end();it++){
        cout << *it << " ";
    }

    cout << endl;
    //gerarSubgrafoVerticeInduzido(&grafo,&arestaDOT,idRotulo,verticesVisitados);

    cout << grafo << endl;

    return grafo;

}

void Graph::fechoTransitivoIndiretoAux(int idNoOrigem, int idNoAlvo, bool *verticesVisitados, list<int> *fechoTransitivoIndireto, list<int> *caminho) {

    Node *noAtual = getNode(idNoOrigem);
    Edge *arestaAtual = noAtual->getFirstEdge();

    verticesVisitados[idNoOrigem - 1] = true;

    caminho->push_back(idNoOrigem);

    while(arestaAtual != nullptr) {

        if(arestaAtual->getTargetId() == idNoAlvo){
            fechoTransitivoIndireto->splice(fechoTransitivoIndireto->end(),*caminho);
            break;
        }

        if(!verticesVisitados[arestaAtual->getTargetId() - 1]){
            fechoTransitivoDiretoAux(arestaAtual->getTargetId(), verticesVisitados);
        }

        arestaAtual = arestaAtual->getNextEdge();
    }

    caminho->clear();

}

string Graph::dijkstra(int idRotuloInicial, int idRotuloFinal) {

    //FALTA TRATAR OS PESOS NEGATIVOS

    Node *noInicial = getNodeByRotulo(idRotuloInicial), *noFinal = getNodeByRotulo(idRotuloFinal);

    //Clausula de segurança para nós que não existem no grafo
    if(noInicial == nullptr || noFinal == nullptr){
        cout << "Insira um noh válido!" << endl;
        return "";
    }

    //Clausula de segurança para nós que não possuem Out Degree
    if(noInicial->getOutDegree() < 1){
        cout << "O noh inserido nao possui arestas saindo dele!" << endl;
        return "";
    }

    Node *noAtual = first_node;
    Node *noAlvo;

    float vetorCustos[this->order];
    Node *vetorPais[this->order];
    list<int> listaVerticesDisponiveis;
    Edge *arestaAtual;
    float menorCustoCaminho = INFINITY;
    float menorCustoCaminhoAux;
    int idNoMenorCustoCaminho;
    int idNoMenorCustoCaminhoAux;

    cout << "Chegou aqui 1" << endl;

    //Percorre os nós do grafo para popular a lista e os vetores
    while(noAtual != nullptr) {
        if(noAtual != noInicial){
            listaVerticesDisponiveis.push_back(noAtual->getId());
        }

        arestaAtual = noInicial->hasEdgeBetween(noAtual->getId());

        if(arestaAtual != nullptr) { //caso o nó atual seja um vizinho do nó inicial os vetores sao atualizados de acordo
            vetorPais[noAtual->getId() - 1] = noInicial;
            vetorCustos[noAtual->getId() - 1] = arestaAtual->getWeight();
            if(arestaAtual->getWeight() < menorCustoCaminho){ //se o peso da aresta for menor que o atual menor custo vai atualizar esse custo
                menorCustoCaminho = arestaAtual->getWeight();
                idNoMenorCustoCaminho = noAtual->getId();
            }
        } else {
            vetorCustos[noAtual->getId() - 1] = INFINITY;
            vetorPais[noAtual->getId() - 1] = nullptr;
        }

        noAtual = noAtual->getNextNode();
    }

    vetorCustos[noInicial->getId() - 1] = 0;

    cout << "Chegou aqui 2" << endl;

    noAtual = getNode(idNoMenorCustoCaminho);
    retirarElementoLista(&listaVerticesDisponiveis,idNoMenorCustoCaminho);

    cout << "Chegou aqui 3" << endl;

//    int contador;
    bool atualizouAuxiliaresMenorCusto;

    while(!listaVerticesDisponiveis.empty()){

        cout << "Estou no noh: " << noAtual->getIdRotulo() << " " << noAtual->getId() << endl;

        if(noAtual->getOutDegree() < 1){
            idNoMenorCustoCaminho = extrairIdMenorCustoDisponivel(vetorCustos,&listaVerticesDisponiveis);
            if(idNoMenorCustoCaminho < 1){
               break;
            }
            menorCustoCaminho = vetorCustos[idNoMenorCustoCaminho - 1];
        } else{
//            contador = 0;
            atualizouAuxiliaresMenorCusto = false;
            menorCustoCaminhoAux = INFINITY;
            arestaAtual = noAtual->getFirstEdge();
            while(arestaAtual != nullptr){
                noAlvo = getNode(arestaAtual->getTargetId());
                cout << "O no alvo da aresta atual eh: " << noAlvo->getIdRotulo() << " " << noAlvo->getId() << endl;
                cout << "Chegou aqui 4" << endl;
                if(estaNaLista(noAlvo->getId(),&listaVerticesDisponiveis)){
                    cout << "Chegou aqui 5" << endl;
                    if(vetorCustos[noAlvo->getId() - 1] > arestaAtual->getWeight() + menorCustoCaminho){
                        cout << "Chegou aqui 5.1" << endl;
                        vetorCustos[noAlvo->getId() - 1] = arestaAtual->getWeight() + menorCustoCaminho;
                        vetorPais[noAlvo->getId() - 1] = noAtual;
                        if(vetorCustos[noAlvo->getId() - 1] < menorCustoCaminhoAux){
                            cout << "Chegou aqui 5.2" << endl;
                            menorCustoCaminhoAux = vetorCustos[noAlvo->getId() - 1];
                            idNoMenorCustoCaminhoAux = noAlvo->getId();
                            atualizouAuxiliaresMenorCusto = true;
//                            contador++;
                        }
                    }
                }
                arestaAtual = arestaAtual->getNextEdge();
            }
            cout << "Chegou aqui 6" << endl;
            if(atualizouAuxiliaresMenorCusto){
                menorCustoCaminho = menorCustoCaminhoAux;
                idNoMenorCustoCaminho = idNoMenorCustoCaminhoAux;
            } else{
                idNoMenorCustoCaminho = extrairIdMenorCustoDisponivel(vetorCustos,&listaVerticesDisponiveis);
                menorCustoCaminho = vetorCustos[idNoMenorCustoCaminho - 1];
            }
        }
        cout << "Chegou aqui 7" << endl;
        noAtual = getNode(idNoMenorCustoCaminho);
        retirarElementoLista(&listaVerticesDisponiveis,idNoMenorCustoCaminho);
        cout << "Chegou aqui 8" << endl;
    }

    cout << "======<3" << endl;
    //Imprime vetor de pesos para conferir
    for(int i = 0; i < this->order; i++){
        cout<< "Custo vertice " << i + 1 << ": " << vetorCustos[i] << endl;
        if(vetorPais[i] != nullptr){
            cout << "Pai do vertice " << i + 1 << ": " <<vetorPais[i]->getIdRotulo() << endl;
        } else{
            cout << "Pai do vertice: "<< "Pai nulo" << endl;
        }
    }

    //Montar caminho
    string grafo = gerarCaminhoMinimo(vetorCustos,vetorPais,noInicial,noFinal);

    cout << grafo << endl;

    return grafo;

}

string Graph::gerarCaminhoMinimo(float *vetorCustos, Node **vetorPais, Node *noInicial, Node *noFinal){
    string grafo, arestaDOT;
    montarCabecalhoGrafoDOT(&grafo,&arestaDOT);

    //TALVEZ EU CONSIGA FAZER ISSO AQUI SEM USAR ARESTA, USANDO O VETOR CUSTOS

    int idRotuloNoAtual, idRotuloNoAlvo;
    Node *noAtual = noFinal;
    Node *noAlvo = vetorPais[noFinal->getId() - 1];
    Edge *arestaAtual = noAlvo->hasEdgeBetween(noAtual->getId());

    idRotuloNoAtual = noAtual->getIdRotulo();
    idRotuloNoAlvo = noAlvo->getIdRotulo();

    float pesoArestaAtual;
    pesoArestaAtual = arestaAtual->getWeight();

    while(noAtual != noInicial){
        idRotuloNoAtual = noAtual->getIdRotulo();
        idRotuloNoAlvo = noAlvo->getIdRotulo();
        arestaAtual = noAlvo->hasEdgeBetween(noAtual->getId());
        pesoArestaAtual = arestaAtual->getWeight();
        montarArestaGrafoDOT(&grafo,&arestaDOT,idRotuloNoAtual,idRotuloNoAlvo,pesoArestaAtual,false);
        noAtual = noAlvo;
        noAlvo = vetorPais[noAlvo->getId() - 1];
    }
    grafo += "}";

    return grafo;
}

int Graph::extrairIdMenorCustoDisponivel(float *vetorCustos, list<int> *listaVerticesDisponiveis){

    int idMenorCusto = 0;
    int menorCusto = INFINITY;

    cout << "Chegou aqui 50" << endl;

    for(int i = 0; i < this->order; i++){
        if(estaNaLista(i + 1,listaVerticesDisponiveis)){
            if(vetorCustos[i] < menorCusto){
                menorCusto = vetorCustos[i];
                idMenorCusto = i + 1;
            }
        }
    }

    return idMenorCusto;
}

bool Graph::estaNaLista(int idTarget, list<int> *listaVerticesDisponiveis){

    cout << "CHEGOU NA FUNCAO SEPARADA" << endl;

    for(list<int>::iterator it = listaVerticesDisponiveis->begin(); it!=listaVerticesDisponiveis->end();it++){
        cout << *it << " ";
        if(*it == idTarget){
            cout << endl;
            return true;
        }
    }
    cout << endl;
    return false;
}

void Graph::retirarElementoLista(list<int> *listaVerticesDisponiveis, int verticeMenorCaminhoAtual){
    for(list<int>::iterator it = listaVerticesDisponiveis->begin(); it!=listaVerticesDisponiveis->end();it++){
        if(*it == verticeMenorCaminhoAtual)
        {
            listaVerticesDisponiveis->erase(it);
            break;
        }
    }
}

string Graph::floyd(int idRotuloInicial, int idRotuloFinal ){

    float matrizVertices[this->order][this->order];
    Node *noAtual, *noAlvo, *noInicial = getNodeByRotulo(idRotuloInicial), *noFinal = getNodeByRotulo(idRotuloFinal);
    Edge *arestaAtual;
    int linha, coluna, idInicial = noInicial->getId(), idFinal = noFinal->getId();
    list <int> caminho;
    list<int>::iterator it;

    //Clausula de segurança para nós que não existem no grafo
    if(noInicial == nullptr || noFinal == nullptr){
        cout << "O no escolhido nao esta presente no grafo!" << endl;
        return "";
    }

    //Clausula de segurança para nós que não possuem Out Degree
    if(noInicial->getOutDegree() < 1){
        cout << "O no escolhido possui grau de saida igual a zero!" << endl;
        return "";
    }

    //Loop responsável por montar a matriz inicial
    for(linha = 0; linha < this->order; linha++) {
        noAtual = getNode(linha + 1);
        for(coluna = 0; coluna < this->order; coluna++) {
            noAlvo = getNode(coluna + 1);
            if(linha == coluna) {  //responsável por colocar o valor 0 na distância entre o vértice e ele mesmo
                matrizVertices[linha][coluna] = 0;
            } else if(noAtual->searchEdge(noAlvo->getId())){ //verifica se existe uma aresta entre os dois vertices
                arestaAtual = noAtual->hasEdgeBetween(noAlvo->getId());
                //Clausula de segurança para caso exista arestas com peso negativo
                if(arestaAtual->getWeight() < 0){
                    cout << "Algoritmo interrompido por ter aresta com peso negativo!" << endl;
                    return "";
                }

                matrizVertices[linha][coluna] = arestaAtual->getWeight(); //insere a distancia entre os dois vertices na matriz

                //caso exista aresta entre o nó inicial e o nó final, a lista caminho é atualizada
                if(linha == idInicial - 1 && coluna == idFinal - 1){
                    caminho.push_front(idInicial);
                    caminho.push_back(idFinal);
                }
            } else {
                matrizVertices[linha][coluna] = INFINITY; //quando não houver arestas entre os vértices, o valor INFINITO é usado
            }
        }
    }


    //Loop responsável por atualizar a matriz com os caminhos obtidos
    for(int k = 0; k < this->order; k++) {
        for(linha = 0; linha < this->order; linha++) {
            noAtual = getNode(linha + 1);
            for(coluna = 0; coluna < this->order; coluna++) {
                noAlvo = getNode(coluna + 1);
                //verifica se os indices da matriz são diferentes entre si para poderem ser atualizados se possível
                if(linha != k && coluna != k && linha != coluna){
                    //verifica se o vértice não está numa linha ou coluna que é impossível sofrer alterações no momento
                    if(matrizVertices[linha][k] != INFINITY && matrizVertices[k][coluna] != INFINITY){
                        //verifica se o caminho atual na matriz é maior que o novo caminho proposto
                        if(matrizVertices[linha][coluna] > matrizVertices[linha][k] + matrizVertices[k][coluna]){
                            matrizVertices[linha][coluna] = matrizVertices[linha][k] + matrizVertices[k][coluna];
                            //verifica se atualizou o caminho entre os nós escolhidos pelo usuario
                            if(linha == idInicial - 1 && coluna == idFinal - 1){
                                //se o caminho estiver vazio, realiza a primeira inserção
                                if(caminho.empty()){
                                    caminho.push_front(k + 1);
                                    caminho.push_back(idFinal);
                                } else{
                                    it = caminho.end();
                                    it--;
                                    caminho.insert(it,k + 1); //adiciona o novo vertice no caminho na penultima posição
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    string grafo;

    //verifica se o caminho está vazio
    if(!caminho.empty()){
        grafo = gerarCaminhoMinimoFloyd(&caminho, matrizVertices[0]);
    } else{
        //Retorna um erro em caso de caminho não existente.
        cout << "Nao possui caminho entre os dois vertices!" << endl;
        return "";
    }

    cout << grafo << endl;

    return grafo;
}

string Graph::gerarCaminhoMinimoFloyd(list<int> *caminho, float *matrizVertices){

    string grafo, arestaDOT;
    //monta o cabeçalho da string responsavel por formar o grafo na linguagem DOT
    montarCabecalhoGrafoDOT(&grafo,&arestaDOT);

    Node *noAtual, *noAlvo;
    int idNoAtual, idNoAlvo, idNoFinal = caminho->back();
    float distancia;

    //percorre os nós presentes no caminho
    for(auto it = caminho->begin(); *it!=caminho->back(); it++){
        noAtual = getNode(*it);
        idNoAtual = noAtual->getId();
        noAlvo = getNode(*next(it,1));
        idNoAlvo = noAlvo->getId();

        distancia = matrizVertices[this->order*(idNoAtual - 1) + (idNoAlvo - 1)];

        //monta a aresta do caminho de acordo com a linguagem DOT
        montarArestaGrafoDOT(&grafo,&arestaDOT,noAtual->getIdRotulo(),noAlvo->getIdRotulo(),distancia,false);
    }
    grafo += "}";

    return grafo;
}

Graph* Graph::kruskal(int *subconjuntoVertices, int qntdVertices){

    Graph *arvore = new Graph(0,this->directed,this->weighted_edge,this->weighted_node);
    int idRotulo;
    Node *noAtual;

    //Loop responsavel por inserir os nos selecionados na arvore
    for(int i = 0; i < qntdVertices; i++){
        noAtual = getNodeByRotulo(subconjuntoVertices[i]);
        if(noAtual != nullptr){
            arvore->insertNode(noAtual->getIdRotulo(),0);
        } else {
            cout << "Um dos vertices escolhidos nao esta presente no grafo!" << endl;
            return nullptr;
        }
    }

    for(int i = 0; i < qntdVertices; i++) {
        cout << arvore->getNode(i+1)->getIdRotulo() << " ";
    }

    cout << endl;

    queue<Edge*> filaArestas, filaArestasAux;

    ordenarArestasOrdemCrescente(arvore,&filaArestas,&filaArestasAux);

    cout << "Tamanho da fila principal: " << filaArestas.size() << endl;
    cout << "Tamanho da fila auxiliar: " << filaArestasAux.size() << endl;

    if(filaArestas.empty() && !filaArestasAux.empty()) {
        filaArestas.swap(filaArestasAux);
    }

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
        idOrigemRotulo = filaArestas.front()->getOriginIdRotulo();
        idOrigem = arvore->getNodeByRotulo(idOrigemRotulo)->getId();
        idAlvoRotulo = filaArestas.front()->getTargetIdRotulo();
        idAlvo = arvore->getNodeByRotulo(idAlvoRotulo)->getId();
        cout << "idOrigem = " << idOrigem << " idAlvo = " << idAlvo << endl;
        if(!estaNaMesmaSubarvore(vertices, idOrigem, idAlvo)){
            cout << "HERE 1" << endl;
            idOrigemRotulo = arvore->getNode(idOrigem)->getIdRotulo();
            cout << "HERE 1.1" << endl;
            idAlvoRotulo = arvore->getNode(idAlvo)->getIdRotulo();
            cout << "HERE 1.2" << endl;

            arvore->insertEdge(idOrigemRotulo,idAlvoRotulo,filaArestas.front()->getWeight());
            cout << "HERE 2" << endl;
            if(vertices[idOrigem - 1] <= vertices[idAlvo - 1]){
                cout << "HERE 3" << endl;
                for(int i = 0; i < arvore->getOrder(); i++){
                    if(vertices[i] == vertices[idAlvo - 1]){
                        vertices[i] = vertices[idOrigem - 1];
                    }
                }
            } else {
                cout << "HERE 4" << endl;
                for(int i = 0; i < arvore->getOrder(); i++){
                    if(vertices[i] == vertices[idOrigem - 1]){
                        vertices[i] = vertices[idAlvo - 1];
                    }
                }
            }
        }
        cout << "HERE 5" << endl;
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

void Graph::ordenarArestasOrdemCrescente(Graph *arvore, queue<Edge*> *filaArestas, queue<Edge*> *filaArestasAux){
    Node *noAtual = this->first_node;
    Edge *arestaAtual;
    bool encontrouPosicao;
    //loop responsavel por percorrer cada no do grafo e ordenar a lista de arestas
    while(noAtual != nullptr){
        cout << "=======" << endl;
        cout << "Estou no Noh " << noAtual->getIdRotulo() << endl;
        cout << "-------" << endl;
        if (arvore->getNodeByRotulo(noAtual->getIdRotulo()) != nullptr){
            arestaAtual = noAtual->getFirstEdge();
            while(arestaAtual != nullptr){
                cout << "Estou na Aresta com alvo " << getNode(arestaAtual->getTargetId())->getIdRotulo() << endl;
                if(arvore->getNodeByRotulo(getNode(arestaAtual->getTargetId())->getIdRotulo()) != nullptr){
                    encontrouPosicao = false;
                    if(!arestaAtual->getRetorno()){
                        if (filaArestas->empty() && filaArestasAux->empty()){ //verifica se as duas filas estao vazias
                            filaArestas->push(arestaAtual);
                        } else if(filaArestasAux->empty()) { //verifica se a fila auxiliar esta vazia
                            if(filaArestas->back()->getWeight() <= arestaAtual->getWeight()){
                                filaArestas->push(arestaAtual);
                            } else{
                                while(!filaArestas->empty()){
                                    if(encontrouPosicao || filaArestas->front()->getWeight() <= arestaAtual->getWeight()){
                                        cout << "IF Deve ter adicionado aresta com alvo " << getNode(filaArestas->front()->getTargetId())->getIdRotulo() << endl;
                                        filaArestasAux->push(filaArestas->front());
                                        filaArestas->pop();
                                    } else{
                                        cout << "ELSE Deve ter adicionado aresta com alvo " << getNode(arestaAtual->getTargetId())->getIdRotulo() << endl;
                                        filaArestasAux->push(arestaAtual);
                                        encontrouPosicao = true;
                                    }
                                }
                            }
                        } else if(filaArestas->empty()){ //verifica se a fila principal esta vazia
                            if(filaArestasAux->back()->getWeight() <= arestaAtual->getWeight()) {
                                filaArestasAux->push(arestaAtual);
                            } else {
                                while(!filaArestasAux->empty()){
                                    if(encontrouPosicao || filaArestasAux->front()->getWeight() <= arestaAtual->getWeight()){
                                        cout << "IF Deve ter adicionado aresta com alvo " << getNode(filaArestasAux->front()->getTargetId())->getIdRotulo() << endl;
                                        filaArestas->push(filaArestasAux->front());
                                        filaArestasAux->pop();
                                    } else{
                                        cout << "ELSE Deve ter adicionado aresta com alvo " << getNode(arestaAtual->getTargetId())->getIdRotulo() << endl;
                                        filaArestas->push(arestaAtual);
                                        encontrouPosicao = true;
                                    }
                                }
                            }
                        }
                    }
                }
                arestaAtual = arestaAtual->getNextEdge();
            }
        }
        noAtual = noAtual->getNextNode();
    }
}

bool Graph::estaNaMesmaSubarvore(int *vertices, int idOrigem, int idAlvo){
    if(vertices[idOrigem - 1] == vertices[idAlvo - 1]){
        cout << vertices[idOrigem - 1] << " = " << vertices[idAlvo - 1] << endl;
        return true;
    }
    return false;
}




