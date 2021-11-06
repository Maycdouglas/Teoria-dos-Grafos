#include "Edge.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Edge's methods
**************************************************************************************************/

// Constructor
Edge::Edge(int target_id, int origin_id, int id){

    this->target_id = target_id;
    this->origin_id = origin_id;
    this->id = id;
    this->next_edge = nullptr;
    this->weight = 0;

}

// Destructor
Edge::~Edge(){
//
//    if (this->next_edge != nullptr){
//        delete this->next_edge;
//        this->next_edge = nullptr;
//    }
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

int Edge::getId()
{
    return this->id;
}

// Setters
void Edge::setNextEdge(Edge* edge){

    this->next_edge = edge;

}

void Edge::setWeight(float weight){

    this->weight = weight;

}



