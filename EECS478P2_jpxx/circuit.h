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

using namespace std;

#include "node.h"

typedef map<string,Node*>::iterator mapIter;
class Circuit
{
  private:
    map<string,Node*> nodeMap;
    string name;
    
  public:
    Circuit() {};
    
    Circuit(const string &circuitName):name(circuitName) {}
    
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
    
    // writes circuit in BLIF format
    int writeBLIF(const string &filename);
    
    // erases the contents of the entire circuit
    int clear();
    
    // start library definitions
    int createONENode(Node* node);
    int createZERONode(Node* node);
    int createBUF1Node(Node* input, Node* output);
    int createOR2Node(Node* input1, Node* input2, Node* output);
    int createAND2Node(Node* input1, Node* input2, Node* output);
    int createXOR3Node(Node* input1, Node* input2, Node* input3, Node* output);
    int createMUX4Node(Node* select1, Node* select2,
                       Node* input1, Node* input2, Node* input3, Node* input4, Node* output);
    
    // start module definitions
    int createADDModule(const string &input1, const string &input2, const string &cin, const string &output, const string &cout, unsigned int numBits);
    int createMULTModule(const string &input1, const string &input2, const string &output, unsigned int numBits);
    int createSHIFTModule(const string &input, const string &output, unsigned int numBits, unsigned int numShift);
    
    // start datapath definitions
    int createSUMABMULTIPLYSUMCDModule(const string &input1, const string &input2, const string &input3, const string &input4, const string &output);
};

#endif

