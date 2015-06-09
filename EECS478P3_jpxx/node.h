#ifndef __NODE_H__
#define __NODE_H__

#include <string>
#include <iostream>
#include <vector>
#include <math.h>


using namespace std;

#include "truthTable.h"

// define node type
enum nodeType {PRIMARY_INPUT, PRIMARY_OUTPUT, INTERNAL, ZERO_NODE, ONE_NODE};

// define value type
class Circuit;

class Node
{
  friend class Circuit;

  private:
    string name;
    nodeType type;
    vector<Node*> fanin;
    TruthTable tt;
    
    //create for topoSort
    int mark;
    
    //value to store simulation results
    truthType value;

  public:
    // constructors
    Node():type(INTERNAL) {}

    Node(const string &nodeName):name(nodeName), type(INTERNAL) {}

    // destructor
    ~Node() {};

    // returns and sets the name of the node
    string getName() { return name; }
    int setName(const string &n) { name = n; return 0; }

    // returns and sets the type of the node
    nodeType getType() { return type; }
    int setType(nodeType t) { type = t; return 0; }

    // returns the vector of fanin nodes
    vector<Node*> getFanin() { return fanin; }

    // adds a fanin node
    int addFanin(Node* &inNode) { fanin.push_back(inNode); return 0; }

    // returns the number of fanin nodes (variables)
    unsigned getNumFanin() { return fanin.size(); }

    // clear functions
    int clearName() { name = ""; return 0; }
    int clearFanin() { fanin.clear(); return 0; }
    int clearTT() { tt.clear(); return 0; }
    int clear() { name = ""; clearFanin(); clearTT(); return 0; }

    // prints node information
    int print()
    {
      cout << "Name: " << name << " [TYPE = ";
      switch(type)
      {
        case PRIMARY_INPUT : cout << "PRIMARY_INPUT";  break;
        case PRIMARY_OUTPUT: cout << "PRIMARY_OUTPUT"; break;
        case INTERNAL      : cout << "INTERNAL";       break;
        case ZERO_NODE     : cout << "ZERO_NODE";      break;
        case ONE_NODE      : cout << "ONE_NODE";       break;
      }
      cout << "]" << endl;

      if (type == PRIMARY_OUTPUT || type == INTERNAL)
      {
        cout << "Fanin nodes: ";
        for (unsigned i = 0; i < fanin.size(); ++i)
          cout << fanin[i]->name << " ";
        cout << endl;
        tt.print();
      }

      return 0;
    }


};

#endif

