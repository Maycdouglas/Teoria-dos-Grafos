#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Edge's methods
**************************************************************************************************/

// Constructor
Edge::Edge(int target_id, int origin_id, int target_id_rotulo, int origin_id_rotulo){

    this->target_id = target_id;
    this->target_id_rotulo = target_id_rotulo;
    this->origin_id = origin_id;
    this->origin_id_rotulo = origin_id_rotulo;
    this->next_edge = nullptr;
    this->weight = 0;
    this->retorno = false;
    this->vermelho = false;

}

// Destructor
Edge::~Edge(){
//
//    if (this->next_edge != nullptr){
//        delete this->next_edge;
//        this->next_edge = nullptr;
//    }
    this->next_edge = nullptr;
    cout << "destruiu" << endl;
}

// Getters
int Edge::getTargetId(){

    return this->target_id;

}

int Edge::getTargetIdRotulo(){

    return this->target_id_rotulo;

}

Edge* Edge::getNextEdge(){

    return this->next_edge;

}

float Edge::getWeight(){

    return this->weight;

}

int Edge::getOriginId() {
    return this->origin_id;
}

int Edge::getOriginIdRotulo() {
    return this->origin_id_rotulo;
}

bool Edge::getRetorno() {
    return this->retorno;
}

bool Edge::getVermelho() {
    return this->vermelho;
}

bool Edge::getMarcado() {
    return this->marcado;
}

// Setters
void Edge::setNextEdge(Edge* edge){

    this->next_edge = edge;

}

void Edge::setWeight(float weight){

    this->weight = weight;

}

void Edge::setRetorno(bool retorno) {
    this->retorno = retorno;
}

void Edge::setVermelho(bool vermelho) {
    this->vermelho = vermelho;
}

void Edge::setMarcado(bool marcado) {
    this->marcado = marcado;
}



