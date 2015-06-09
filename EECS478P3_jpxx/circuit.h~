#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include <stdlib.h>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <stack>
#include <set>

using namespace std;

#include "node.h"

typedef map<string,Node*>::iterator mapIter;
class Circuit
{
  private:
    map<string,Node*> nodeMap;
    string name;

    //create L to implement topoSort
    vector<Node*> L;

  public:
    // constructors
    Circuit() {};

    Circuit(const string &filename)
    { if (readBLIF(filename)) cout << "error in reading " << filename << endl; }

    // destructor
    ~Circuit();

    // get number of nodes in the circuit
    unsigned getNumNodes() { return nodeMap.size(); }

    // get and set the name of the circuit
    string getName() { return name; }
    int setName(const string &modelName) { name = modelName; return 0; }

    // get primary inputs and outputs
    vector<Node*> getPIs();
    vector<Node*> getPOs();

    // sets the node of <string> as primary input and output
    int setPI(const string &input);
    int setPO(const string &output);

    // set (assign) nodes in specific format as primary inputs and outputs from [startBit,endBit)
    // for a given string <string> and numBits, format is <string>[startBit] ... <string[endBit-1]
    int setPIs(const string &input, unsigned int startBit, unsigned int endBit);
    int setPOs(const string &output, unsigned int startBit, unsigned int endBit);

    // check to see if a node is there
    Node* findNode(const string &nodeName);

    // create an empty node - will return NULL if node already exists
    Node* createNode(const string &nodeName);

    // prints out the contents of the circuit
    int print();

    // reads in circuit from BLIF format
    int readBLIF(const string &filename);

    // writes circuit in BLIF format
    int writeBLIF(const string &filename);

    // erases the contents of the entire circuit
    int clear();
    
    //topological sort function, results stored in vector<Node*> L;
    void topologicalSorting();
    
    //topological sort visit function
    void topologicalSortingVisit(set<Node*> &initial, Node* N);
    
    //print topological order
    void print_topologicalSorting();
    
    //simulate outputs of the circuit
    void functionalSimulation(const string &filename);
    
    //print simulation results
    void print_functionalSimulation();


};

#endif

