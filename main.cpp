#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>
#include <tuple>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include "Graph.h"
#include "Node.h"

using namespace std;

void exportarGrafo(Graph* graph, ofstream& output_file) {

    string grafo, arestaDOT;

    if(graph->getDirected()) //verifica se o grafo é direcionado
    {
        grafo = "strict digraph G {\n";
        arestaDOT = " -> ";
    }
    else {
        grafo = "strict graph G {\n";
        arestaDOT = " -- ";
    }

    Node *noAtual = graph->getFirstNode();
    Edge *arestaAtual;

    while(noAtual != nullptr){
        arestaAtual = noAtual->getFirstEdge();
        while (arestaAtual != nullptr) {
            grafo += "\t" + to_string(noAtual->getIdRotulo()) + arestaDOT + to_string(arestaAtual->getTargetIdRotulo());
            if(graph->getWeightedEdge()){
                grafo += " [weight = " + to_string(arestaAtual->getWeight());
                grafo += ", label = " + to_string(arestaAtual->getWeight());
                if (arestaAtual->getVermelho()) {
                    grafo += ", color = red";
                }
            } else{
                if (arestaAtual->getVermelho()) {
                    grafo += " [color = red";
                }
            }
            grafo += "]\n";
            arestaAtual = arestaAtual->getNextEdge();
        }
        noAtual = noAtual->getNextNode();
    }

    grafo += "}";

    output_file << grafo;

}

Graph* leituraInstancia(ifstream& input_file, int directed, int weightedEdge, int weightedNode){

    //Variaveis para auxiliar na criacao dos nos no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;
    //int numEdges;
    int pesoAresta;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando o grafo
    Graph* graph = new Graph(0, directed, weightedEdge, weightedNode);


    //Leitura de arquivo
    if(weightedEdge){
        while(input_file >> idNodeSource >> idNodeTarget >> pesoAresta) {
            graph->insertEdge(idNodeSource, idNodeTarget, pesoAresta);
        }
    } else{
        while(input_file >> idNodeSource >> idNodeTarget >> pesoAresta) {
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
        }
    }


    return graph;
}

int menu(){

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Gerar grafo" << endl;
    cout << "[2] Busca em Largura" << endl;
    cout << "[3] Fecho Transitivo Direto" << endl;
    cout << "[4] Fecho Transitivo Indireto" << endl;
    cout << "[5] Dijkstra" << endl;
    cout << "[6] Floyd" << endl;
    cout << "[7] Kruskal" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {

        //Gera o grafo enviado
        case 1:{
            exportarGrafo(graph,output_file);
            Node *no = graph->getFirstNode();
            while(no != nullptr)
            {
                cout << "No do grafo: " << no->getId() << endl;
                cout << "No Rotulo do grafo: " << no->getIdRotulo() << endl;
                cout << "Arestas: " << endl;
                Edge *aresta = no->getFirstEdge();
                while(aresta != nullptr)
                {
                    cout << "No origem: " << aresta->getOriginId() << " No alvo: " << aresta->getTargetId() << " Peso da Aresta: "<< aresta->getWeight() << endl;
                    aresta = aresta->getNextEdge();
                }
                cout << "==============" << endl;

                no = no->getNextNode();
            }
            cout << "Ordem do grafo eeh: " << graph->getOrder() << endl;
            break;
        }
            //Busca em Largura
        case 2:{
//            exportarGrafo(graph->buscaEmLargura(40), output_file);
                int idRotulo;
                cout << "Insira o ID do Noh: ";
                cin >> idRotulo;
                output_file << graph->buscaEmLargura(idRotulo);
            break;
        }

            //Fecho Transitivo Direto
        case 3:{
            int idRotulo;
            cout << "Insira o ID do Noh: ";
            cin >> idRotulo;
            output_file << graph->fechoTransitivoDireto(idRotulo);
            break;
        }

            //Fecho Transitivo Indireto
        case 4:{
            int idRotulo;
            cout << "Insira o ID do Noh: ";
            cin >> idRotulo;
            output_file << graph->fechoTransitivoIndireto(idRotulo);
            break;
        }

            //Algoritmo Guloso Randomizado Reativo
        case 5:{
            int idRotuloInicial, idRotuloFinal;
            cout << "Insira o ID do Noh Inicial: ";
            cin >> idRotuloInicial;
            cout << "\nInsira o ID do Noh Final: ";
            cin >> idRotuloFinal;
            output_file << graph->dijkstra(idRotuloInicial,idRotuloFinal);
            break;
        }

        case 6:{
            int idRotuloInicial, idRotuloFinal;
            cout << "Insira o ID do Noh Inicial: ";
            cin >> idRotuloInicial;
            cout << "\nInsira o ID do Noh Final: ";
            cin >> idRotuloFinal;
            output_file << graph->floyd(idRotuloInicial,idRotuloFinal);
            break;
        }

        case 7:{

            int subconjuntoVertices[graph->getOrder()];
            int qntdVertices;
            int vertice;
            cout << "Insira a quantidade de vertices presentes no seu subconjunto: ";
            cin >> qntdVertices;
            Graph *arvore;

            if(qntdVertices > 1){
                for(int i = 0; i < qntdVertices; i++){
                    cin >> subconjuntoVertices[i];
                }
                arvore = graph->kruskal(subconjuntoVertices,qntdVertices);
                if(arvore != nullptr){
                    exportarGrafo(arvore, output_file);
                }
            } else {
                cout << "Quantidade de vértices inválida! Insira 2 ou mais vértices." << endl;
            }

//            string teste = graph->kruskal();
            break;
        }

        default:
        {
            cout << " Error!!! invalid option!!" << endl;
        }

    }
}

int mainMenu(ofstream& output_file, Graph* graph){

    int selecao = 1;

    while(selecao != 0){
        //system("clear");
        selecao = menu();

        if(selecao != 0){
            if(output_file.is_open())
                selecionar(selecao, graph, output_file);

            else
                cout << "Unable to open the output_file" << endl;
        }

        output_file << endl;

    }

    return 0;
}

int main(int argc, char const *argv[]) {

    //Verifica se todos os parâmetros do programa foram inseridos
    if (argc != 6) {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    //SIMPLESMENTE NÃO SEI O QUE ESTÁ ACONTECENDO NESSE BLOCO DE CÓDIGO
    string instance;
    if(input_file_name.find("v") <= input_file_name.size()){
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Graph* graph;

    if(input_file.is_open()){

        graph = leituraInstancia(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

    }else
        cout << "Unable to open " << argv[1];


    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();



    return 0;
}
