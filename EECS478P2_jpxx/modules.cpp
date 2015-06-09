#include "circuit.h"

int Circuit::createADDModule(const string &input1, const string &input2, const string &cin, const string &output, const string &cout, unsigned int numBits)
{
    Node* node;
    string name;
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;
        
        // create input1 nodes
        name = input1 + "[" + sstr.str() + "]";
        node = createNode(name);
        // create input2 nodes
        name = input2 + "[" + sstr.str() + "]";
        node = createNode(name);
        
        // create output nodes
        name = output + "[" + sstr.str() + "]";
        node = createNode(name);
        
        // create (A * B), (A + B), Cin * (A + B) nodes for
        // storing these values to compute cout
        name = output + "AB[" + sstr.str() + "]";
        node = createNode(name);
         
        name = output + "AorB[" + sstr.str() + "]";
        node = createNode(name);
 
        name = output + "cin_AorB[" + sstr.str() + "]";
        node = createNode(name);

    }
    
    // create cin node
    node = createNode(cin);
    
    // create cout node
    node = createNode(cout);
     
    // assign '0's
    Node* zeroNode = createNode("ZERO");
    createZERONode(zeroNode);

    // create internal cin nodes
    for (unsigned int i = 0; i < numBits - 1; ++i)
    {
        stringstream sstr;
        sstr << i;
        name = output + "Internal_Cin[" + sstr.str() + "]";
        node = createNode(name);
    }
    
    
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr1;
        sstr1 << i;
        stringstream sstr2;
        sstr2 << i - 1;
        
        // find input1[i] nodes
        name = input1 + "[" + sstr1.str() + "]";
        Node* inNode1 = findNode(name);
        assert(inNode1 != NULL);
        // find input2[i] nodes
        name = input2 + "[" + sstr1.str() + "]";
        Node* inNode2 = findNode(name);
        assert(inNode2 != NULL);
        
        // find output[i] nodes
        name = output + "[" + sstr1.str() + "]";
        Node* outNode = findNode(name);
        assert(outNode != NULL);
        
        // find cin node
        if (i == 0) // first bit
        {
            name = cin;
        }
        else
        {
            name = output + "Internal_Cin[" + sstr2.str() + "]";
        }
        Node* cinNode = findNode(name);
        assert(cinNode != NULL);
        
        // find cout node
        if (i == numBits - 1) // last bit
        {
            name = cout;
        }
        else
        {
            name = output + "Internal_Cin[" + sstr1.str() + "]";
        }
        Node* coutNode = findNode(name);
        assert(coutNode != NULL);
     
        // find (A * B) nodes
        name = output + "AB[" + sstr1.str() + "]";
        Node* AND_AB = findNode(name);
        // find (A + B) nodes
        name = output + "AorB[" + sstr1.str() + "]";
        Node* OR_AB = findNode(name);
        // find Cin * (A + B) nodes
        name = output + "cin_AorB[" + sstr1.str() + "]";
        Node* Cin_AND_AorB = findNode(name);
         
        // Calculate Sum = A xor B xor Cin
        createXOR3Node(inNode1, inNode2, cinNode, outNode);
         
        // Calculate cout = A*B + Cin*(A + B)
        createAND2Node(inNode1, inNode2, AND_AB);
        createOR2Node(inNode1, inNode2, OR_AB);
        createAND2Node(cinNode, OR_AB, Cin_AND_AorB);
        createOR2Node(AND_AB, Cin_AND_AorB, coutNode);
 
    }
     
  // when you have implemented this function,
  // change 'return -1' to 'return 0'
  return 0;
}

int Circuit::createMULTModule(const string &input1, const string &input2, const string &output, unsigned int numBits)
{
    Node* node;
    string name;
    // create input nodes
    for (unsigned int i = 0; i < numBits; ++i)
    {
        stringstream sstr;
        sstr << i;
        // create input1 nodes
        name = input1 + "[" + sstr.str() + "]";
        node = createNode(name);
        // create input2 nodes
        name = input2 + "[" + sstr.str() + "]";
        node = createNode(name);
        
        //create output nodes
        name = output + "[" + sstr.str() + "]";
        node = createNode(name);
    }
    
    // assign '0's
    Node* zeroNode = createNode("ZERO");
    createZERONode(zeroNode);
    
    // 1-bit multiplication
    if (numBits == 1)
    {
        // find input1[0]
        name = input1 + "[0]";
        Node* inNode1 = findNode(name);
        assert(inNode1 != NULL);
        // find input2[0]
        name = input2 + "[0]";
        Node* inNode2 = findNode(name);
        assert(inNode2 != NULL);
        // find output[0]
        name = output + "[0]";
        Node* outNode = findNode(name);
        assert(outNode != NULL);
        // output[0] = input1[0] * input2[0]
        createAND2Node(inNode1, inNode2, outNode);
        // find the MSB of the product
        name = output + "[1]";
        outNode = findNode(name);
        assert(outNode != NULL);
        // assign 0 to MSB of the product
        createBUF1Node(zeroNode, outNode);
    }
    // 2-bit multiplication
    else if (numBits == 2)
    {
        for (unsigned int i = 0; i < numBits; ++i)
        {
            stringstream sstr;
            sstr << i;
            // create string variable to store shifted_input1
            string Shift = input1 + "shift[" + sstr.str() + "]";
            // left shift input1 by i bits, store the result to input1shift[i]
            createSHIFTModule(input1, Shift, numBits, i);
            // find input2[i]
            name = input2 + "[" + sstr.str() + "]";
            Node* input2Node = findNode(name);
            assert(input2Node != NULL);
            
            for (unsigned int j = 0; j < numBits + i; ++j)
            {
                stringstream sstr1;
                sstr1 << i;
                stringstream sstr2;
                sstr2 << j;
                // create Partial_Products i[j] nodes
                // i represents the ith level of Partial_Products
                // j represents the jth bit of every Partial_Products
                name = "Partial_Products" + sstr1.str() + "[" + sstr2.str() + "]";
                node = createNode(name);
                Node* outNode = findNode(name);
                assert(outNode != NULL);
                // find shifted_input1[j]
                name = Shift + "[" + sstr2.str() + "]";
                Node* inNode = findNode(name);
                assert(inNode != NULL);
                // Partial Products i[j] = input2[i] * shifted_input1[j]
                createAND2Node(inNode, input2Node, outNode);
            }
        }
        
            stringstream sstr1;
            sstr1 << numBits;
            // Creat Partial_Products0[numBits] node to extend its
            // length, then Partial_Products0 will have the same
            // bit-length with Partial_Products1
            name = "Partial_Products0[" + sstr1.str() + "]";
            node = createNode(name);
            Node* outNode = findNode(name);
            // assgin 0 to Partial_Products0[numBits] node
            createBUF1Node(zeroNode, outNode);
        
            string PP0 = "Partial_Products0";
            string PP1 = "Partial_Products1";
            stringstream sstr2;
            sstr2 << 2*numBits - 1;
            // assign the MSB (2*numbits - 1) of product
            string output_MSB = output + "[" + sstr2.str() + "]";
        
            string PP_sum = output;
            // using ADD module to add two Partial_Products to get the final product
            createADDModule(PP0, PP1, "ZERO", output, output_MSB, 1+numBits);

    }
    // n-bit multiplication, n = numBits > 2
    else
    {
        for (unsigned int i = 0; i < numBits; ++i)
        {
            stringstream sstr;
            sstr << i;
            // create string variable to store shifted_input1
            string Shift = input1 + "shift[" + sstr.str() + "]";
            // left shift input1 by i bits, store the result to input1shift[i]
            createSHIFTModule(input1, Shift, numBits, i);
            // find input2[i]
            name = input2 + "[" + sstr.str() + "]";
            Node* input2Node = findNode(name);
            assert(input2Node != NULL);
            
            for (unsigned int j = 0; j < numBits + i; ++j)
            {
                stringstream sstr1;
                sstr1 << i;
                stringstream sstr2;
                sstr2 << j;
                // create Partial_Products i[j] nodes
                // i represents the level of Partial_Products
                // j represents the jth bit of every level of Partial_Products
                name = "Partial_Products" + sstr1.str() + "[" + sstr2.str() + "]";
                node = createNode(name);
                Node* outNode = findNode(name);
                assert(outNode != NULL);
                // find shifted_input1[j]
                name = Shift + "[" + sstr2.str() + "]";
                Node* inNode = findNode(name);
                assert(inNode != NULL);
                // Partial Products i[j] = input2[i] * shifted_input1[j]
                createAND2Node(inNode, input2Node, outNode);
            }
            stringstream sstr1;
            sstr1 << numBits;
            // Creat Partial_Products0[numBits] node to extend its length
            name = "Partial_Products0[" + sstr1.str() + "]";
            node = createNode(name);
            Node* outNode = findNode(name);
            // assgin 0 to Partial_Products0[numBits] node
            createBUF1Node(zeroNode, outNode);
            
            // the following codes are used to add all of the Partial_Products,
            // different level of Partial_Products should use different method
            // to add them together, therefore, if statement is used here
            
            // add the first two Partial_Products: Partial_Products0 and Partial_Products1
            if (i == 1)
            {
                stringstream sstr1;
                sstr1 << i - 1;
                stringstream sstr2;
                sstr2 << i;
                stringstream sstr3;
                sstr3 << i + numBits;
                
                string PP_last = "Partial_Products" + sstr1.str();
                string PP_current = "Partial_Products" + sstr2.str();
                // create PP_sum to store the result of Partial_Products0 * Partial_Products1
                string PP_sum = "sum" + sstr2.str();
                // store the carry out bit of Partial_Products0 * Partial_Products1
                string PP_sum_cout = "sum" + sstr2.str() + "[" + sstr3.str() + "]";
                
                createADDModule(PP_last, PP_current, "ZERO", PP_sum, PP_sum_cout, i + numBits);
            }
            // add other levels of Partial_Products, except the last level
            else if (i != numBits - 1)
            {
                stringstream sstr1;
                sstr1 << i - 1;
                stringstream sstr2;
                sstr2 << i;
                stringstream sstr3;
                sstr3 << i + numBits;
                
                string PP_last = "sum" + sstr1.str();
                string PP_current = "Partial_Products" + sstr2.str();
                string PP_sum = "sum" + sstr2.str();
                string PP_sum_cout = "sum" + sstr2.str() + "[" + sstr3.str() + "]";
                
                createADDModule(PP_last, PP_current, "ZERO", PP_sum, PP_sum_cout, i + numBits);
            }
            // add the last level of Partial_Products, get the final result of n_bit multiplication
            else if (i == numBits - 1)
            {
                stringstream sstr1;
                sstr1 << i - 1;
                stringstream sstr2;
                sstr2 << i;
                stringstream sstr3;
                sstr3 << 2*numBits - 1;
                
                string PP_last = "sum" + sstr1.str();
                string PP_current = "Partial_Products" + sstr2.str();
                // find the MSB of final output of multiplication
                string output_MSB = output + "[" + sstr3.str() + "]";
                
                createADDModule(PP_last, PP_current, "ZERO", output, output_MSB, i + numBits);
            }
        
          }
    }
    
  // when you have implemented this function,
  // change 'return -1' to 'return 0'
  return 0;
}


int Circuit::createSHIFTModule(const string &input, const string &output, unsigned int numBits, unsigned int numShift)
{
  Node* node;
  // create input nodes
  for (unsigned int i = 0; i < numBits; ++i)
  {
    stringstream sstr;
    sstr << i;
    string name = input + "[" + sstr.str() + "]";
    
    node = createNode(name);
  }
  
  // create output nodes
  for (unsigned int i = 0; i < numBits+numShift; ++i)
  {
    stringstream sstr;
    sstr << i;
    string name = output + "[" + sstr.str() + "]";
    
    node = createNode(name);
  }
  
  // assign '0's to the least numShift bits
  Node* zeroNode = createNode("ZERO");
  createZERONode(zeroNode);
  
  for (unsigned int i = 0; i < numShift; ++i)
  {
    stringstream sstr;
    sstr << i;
    string name = output + "[" + sstr.str() + "]";
    
    Node* outNode = findNode(name);
    assert(outNode != NULL);
    
    createBUF1Node(zeroNode, outNode);
  }
  
  // assign inputs to the remaining numBits bits
  for (unsigned int i = numShift; i < numBits+numShift; ++i)
  {
    string name;
    
    // find input node[i-numShift]
    stringstream inStr;
    inStr << i-numShift;
    name = input + "[" + inStr.str() + "]";
    Node* inNode = findNode(name);
    assert(inNode != NULL);
    
    // find output node[i]
    stringstream outStr;
    outStr << i;
    name = output + "[" + outStr.str() + "]";
    Node* outNode = findNode(name);
    assert(outNode != NULL);
    
    // assign
    createBUF1Node(inNode, outNode);
  }
  
  return 0;
}

