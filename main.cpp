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
    output_file << "strict graph G {\n";

    for(int m = 0; m < graph->getOrder(); m++)
    {
        if(graph->getNode(m + 1)->getNoPai() != nullptr)
        {
            output_file << graph->getNode(m + 1)->getNoPai()->getId() << " -- "  << graph->getNode(m + 1)->getId();
            output_file << " [weight = " << graph->getNode(m + 1)->hasEdgeBetween(graph->getNode(m + 1)->getNoPai()->getId())->getWeight();
            output_file << ", label = " << graph->getNode(m + 1)->hasEdgeBetween(graph->getNode(m + 1)->getNoPai()->getId())->getWeight() << "]\n";
        }
    }
    output_file << "}";
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
    Graph* graph = new Graph(order, directed, weightedEdge, weightedNode);
    int i = 1;

    //Criando os nos
    while( i <= order)
    {
        graph->insertNode(i,0);
        i++;
    }

    //Leitura de arquivo
    int contadorArestas = 1;
    while(input_file >> idNodeSource >> idNodeTarget >> pesoAresta) {

        graph->insertEdge(idNodeSource, idNodeTarget, pesoAresta, contadorArestas);

        if(directed){
            contadorArestas = contadorArestas + 1;
        }
        else {

        }
        contadorArestas = contadorArestas + 2;
    }

    return graph;
}

int menu(){

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Gerar grafo" << endl;
    cout << "[2] Algoritmo Guloso Randomizado" << endl;
    cout << "[3] Algoritmo Guloso Randomizado Reativo" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph* graph, ofstream& output_file){

    switch (selecao) {

        //Gera o grafo enviado
        case 1:{
            exportarGrafo(graph,output_file);
            break;
        }
            //Algoritmo Guloso Randomizado
        case 2:{

            break;
        }

            //Algoritmo Guloso Randomizado Reativo
        case 3:{

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
