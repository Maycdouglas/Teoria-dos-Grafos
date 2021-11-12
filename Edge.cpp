#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Edge's methods
**************************************************************************************************/

// Constructor
Edge::Edge(int target_id, int origin_id){

    this->target_id = target_id;
    this->origin_id = origin_id;
    this->next_edge = nullptr;
    this->weight = 0;
    this->retorno = false;

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

Edge* Edge::getNextEdge(){

    return this->next_edge;

}

float Edge::getWeight(){

    return this->weight;

}

int Edge::getOriginId() {
    return this->origin_id;
}

bool Edge::getRetorno() {
    return this->retorno;
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



