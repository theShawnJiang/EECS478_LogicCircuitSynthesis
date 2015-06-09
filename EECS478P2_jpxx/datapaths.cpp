#include "circuit.h"


int Circuit::createSUMABMULTIPLYSUMCDModule(const string &input1, const string &input2, const string &input3, const string &input4, const string &output)
{
    Node* node;
    string name;
    string outName;
    // create 8-bit input nodes
    for (unsigned int i = 0; i < 8; ++i)
    {
        stringstream sstr;
        sstr << i;
        name = input1 + "[" + sstr.str() + "]";
        node = createNode(name);
        name = input2 + "[" + sstr.str() + "]";
        node = createNode(name);
        name = input3 + "[" + sstr.str() + "]";
        node = createNode(name);
        name = input4 + "[" + sstr.str() + "]";
        node = createNode(name);
    }
    
    // create 18-bit output nodes
    for (unsigned int i = 0; i < 18; ++i)
    {
        stringstream sstr;
        sstr << i;
        name = output + "[" + sstr.str() + "]";
        node = createNode(name);
    }
    
    // assign '0's
    Node* zeroNode = createNode("ZERO");
    createZERONode(zeroNode);
    
    // add: a + b
    outName = "Sumab[8]";
    createADDModule(input1, input2, "ZERO", "Sumab", outName, 8);
    
    // add: c + d
    outName = "Sumcd[8]";
    createADDModule(input3, input4, "ZERO", "Sumcd", outName, 8);
    
    // mult: (a + b) * (c + d)
    createMULTModule("Sumab", "Sumcd", output, 9);

  // after you have implemented this function,
  // change 'return -1' to 'return 0'
  return 0;
}

