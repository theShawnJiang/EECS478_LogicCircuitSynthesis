#include "circuit.h"
#include "node.h"

//Topological Sorting based on Depth-first Search:
//This algorithm loops through each node in an arbitrary order, initiating
//a depth-first search that terminates when it hits any node that has already
//been visited since the beginning of the topological sort
void Circuit::topologicalSorting()
{
    while (!L.empty())
    {
        L.pop_back(); //empty list that will contain the sorted nodes
    }
    
    set<Node*> initial;
    set<Node*>::iterator address;

    for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
    {
        Node* N = it->second;
        N -> mark = 0; //initial the mark
        initial.insert(N);
    }

    while (initial.size() != 0)
    {
        address = initial.begin();
        Node* N = *address; //select an unmarked node n
        topologicalSortingVisit(initial, N); //visit this node n
    }
}


//visit function:
//mark = 0 is its inital value;
//mark = 1 represents that it is a temporary mark;
//mark = 2 represents that it is a permanently mark.
void Circuit::topologicalSortingVisit(set<Node*> &initial, Node* N)
{
    if (N -> mark == 1) //node N has a temporary mark
    {
        cout << "Stop, it is not a DAG" << endl;
        exit(-1);
    }

    if (N -> mark == 2) //node N is permanently marked
    {
        return;
    }

    N -> mark = 1; //mark N temporarily

    for (unsigned int i = 0; i < N -> fanin.size(); i++)
    {
        Node* M = N -> fanin[i]; //for each node M with an edge from N to M
        topologicalSortingVisit(initial, M); //visit node M

    }
    N -> mark = 2; //mark N permanently
    initial.erase(N);
    L.push_back(N); //add N to L
}

//print topological order
void Circuit::print_topologicalSorting()
{
    cout << "*** Topological order:" << endl;
    for (unsigned int i = 0; i < L.size(); i++)
    {
        cout << L[i] -> name << ' ';
    }
    cout << endl;
}


//simulate outputs of the circuit
void Circuit::functionalSimulation(const string &filename)
{
    ifstream inFile;
    inFile.open(filename.c_str()); //open inputFile
    vector<vector<string> > inputs;
    string PIs, PIs_value;
    
    //read input names and its value,
    //then store them in 'inputs'
    while (inFile >> PIs >> PIs_value)
    {
        vector<string> contents;
        contents.push_back(PIs);
        contents.push_back(PIs_value);
        inputs.push_back(contents);
    }
    
    //initialize the values of input nodes;
    //waiting for calling topologicalSorting()
    //function to assign the real value
    for (unsigned int i = 0; i < inputs.size(); i++)
    {
        vector<string> &inputs_contents = inputs[i];
        string PI = inputs_contents[0]; //input name
        string PI_value = inputs_contents[1]; //input value
        
        int values;
        stringstream sstr(PI_value);
        sstr >> values;
        
        Node* node = findNode(PI);
        assert(node != NULL);
        node -> value = (truthType) values;
        
    }
    
    //call topologicalSorting() function
    topologicalSorting();
    
    //simulation
    for (unsigned int i = 0; i < L.size(); i++)
    {
        Node* n = findNode(L[i] -> name);
        assert(n != NULL);
        
        //assign value to each node of L
        if (n -> type == ZERO_NODE)
        {
            n -> value = ZERO;
        }
        
        else if (n -> type == ONE_NODE)
        {
            n -> value = ONE;
        }
        
        //assign value to internal nodes
        else if (n -> type != PRIMARY_INPUT)
        {
            vector<truthType> inner_input;
            for (unsigned int i = 0; i < n -> fanin.size(); i++)
            {
                Node* node = n -> fanin[i];
                inner_input.push_back(node -> value);
            }
            
            n -> value = (truthType) n -> tt.assignment(inner_input);
            inner_input.clear();
        }
    }
    
}

//print simulation results
void Circuit::print_functionalSimulation()
{
    cout << "*** Outputs:" << endl;
    vector<Node*> POs;
    POs = getPOs();
    unsigned int i;
    for (i = 0; i < POs.size() - 1; i++)
    {
        cout << POs[i] -> name << " = " << POs[i] -> value << ", ";
    }
    cout << POs[i] -> name << " = " << POs[i] -> value << endl;
}


/******************Circuit Implementation********************/

Circuit::~Circuit()
{
  clear();
}

vector<Node*> Circuit::getPIs()
{
  vector<Node*> PIs;
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    if (it->second->type == PRIMARY_INPUT)
      PIs.push_back(it->second);
  }
  return PIs;
}

vector<Node*> Circuit::getPOs()
{
  vector<Node*> POs;
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    if (it->second->type == PRIMARY_OUTPUT)
      POs.push_back(it->second);
  }
  return POs;
}

int Circuit::setPI(const string &input)
{
  Node *in = findNode(input);
  assert(in != NULL);

  in->type = PRIMARY_INPUT;
  return 0;
}

int Circuit::setPO(const string &output)
{
  Node *out = findNode(output);
  assert(out != NULL);

  out->type = PRIMARY_OUTPUT;
  return 0;
}

int Circuit::setPIs(const string &input, unsigned int startBit, unsigned int endBit)
{
  for (unsigned i = startBit; i < endBit; ++i)
  {
    stringstream sstr;
    sstr << i;

    string name = input + "[" + sstr.str() + "]";
    assert(findNode(name) != NULL);
    setPI(name);
  }
  return 0;
}

int Circuit::setPOs(const string &output, unsigned int startBit, unsigned int endBit)
{
  for (unsigned i = startBit; i < endBit; ++i)
  {
    stringstream sstr;
    sstr << i;

    string name = output + "[" + sstr.str() + "]";
    assert(findNode(name) != NULL);
    setPO(name);
  }
  return 0;
}

Node* Circuit::findNode(const string &nodeName)
{
  mapIter it = nodeMap.find(nodeName);
  return (it != nodeMap.end() ? it->second : NULL);
}

Node* Circuit::createNode(const string &nodeName)
{
  Node* newNode = findNode(nodeName);
  if (newNode == NULL)
  {
    newNode = new Node(nodeName);
    nodeMap.insert(make_pair(nodeName, newNode));
  }

  return newNode;
}

int Circuit::print()
{
  cout << "Circuit Name: " << name << " contains " << nodeMap.size() << " nodes." << endl;
  cout << "Primary Inputs: ";
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    if (it->second->type == PRIMARY_INPUT)
      cout << it->second->name << " ";
  }
  cout << endl;
  cout << "Primary Outputs: ";
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    if (it->second->type == PRIMARY_OUTPUT)
      cout << it->second->name << " ";
  }
  cout << endl;

  cout << "Nodes:" << endl;
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    it->second->print();
    cout << endl;
  }

  return 0;
}

int Circuit::readBLIF(const string &filename)
{
  cout << "reading file " << filename << "..." << endl;
  ifstream inFile(filename.c_str());
  if (!inFile.good())
  {
    cout << "ERROR in readBLIF() - cannot open "
         << filename << " for reading." << endl;
  }

  // clear circuit's contents
  clear();

  string line;
  while (getline(inFile, line))
  {
//    cout << "processing line: " << line << endl;
    stringstream sstr;
    // skip empty lines
    if (line.length() == 0) continue;

    sstr << line;
    string word;
    vector<string> words;

    // parse each line
    while (sstr >> word)
    {
      words.push_back(word);
    }

    // parse non-empty lines only
    if (!words.empty())
    {
      // skip any comments
      if (words[0].find("#") != string::npos) continue;

      // parse model name
      if (words[0] == ".model")
      {
        for (unsigned i = 1; i < words.size(); ++i)
          name += words[i];
      }

      // parse primary inputs
      else if (words[0] == ".inputs")
      {
        for (unsigned i = 1; i < words.size(); ++i)
        {
          Node* inNode = createNode(words[i]);
          // node already exists
          if (inNode == NULL)
          {
            cout << "ERROR in readBLIF() (inputs) - node " << words[i] << " already exists.";
            return -1;
          }
          inNode->type = PRIMARY_INPUT;
        }
      }

      // parse primary outputs
      else if (words[0] == ".outputs")
      {
        for (unsigned i = 1; i < words.size(); ++i)
        {
          Node* outNode = createNode(words[i]);
          // node already exists
          if (outNode == NULL)
          {
            cout << "ERROR in readBLIF() (outputs) - node " << words[i] << " already exists.";
            return -1;
          }
          outNode->type = PRIMARY_OUTPUT;
        }
      }

      // parse standard logic node
      else if (words[0] == ".names" && words.size() > 2)
      {
        // find/create output node
        Node* outNode = findNode(words[words.size()-1]);
        if (outNode == NULL) outNode = createNode(words[words.size()-1]);
        else
        {
          outNode->clearTT();
          outNode->clearFanin();
        }

        // find/create input nodes
        for (unsigned i = 1; i < words.size()-1; ++i)
        {
          Node* inNode = findNode(words[i]);
          if (inNode == NULL) inNode = createNode(words[i]);
          outNode->addFanin(inNode);
        }
        outNode->tt.setNumVars(words.size()-2);

        // read truth table entries
        char c;
        do
        {
          string entry, result;
          inFile >> entry >> result;
//          cout << entry << " " << result << endl;
          if (entry.length() != outNode->getNumFanin())
          {
            cout << "ERROR in readBLIF() - number of input entries "
                 << "does not match number of fanin nodes" << endl;
            cout << "entry: " << entry << " ; vs numFanins = " << outNode->getNumFanin() << endl;
            return -1;
          }
          // only parse '1' result
          if (result == "1")
          {
            if (outNode->tt.addEntry(entry))
            {
              cout << "ERROR in readBLIF() - cannot add entry " << entry
                   << " to truth table." << endl;
              return -1;
            }
          }
          getline(inFile, result);
          c = inFile.peek(); // look at next character
//          cout << "c: " << c << endl;
        } while (c == '1' || c == '0' || c == '-');
      }

      // parse logic ZERO and ONE nodes
      else if (words[0] == ".names" && words.size() == 2)
      {
        Node* outNode = findNode(words[1]);
        if (outNode == NULL) outNode = createNode(words[1]);
        else
        {
          outNode->clearTT();
          outNode->clearFanin();
        }
        // if next line is '1', then logic 1
        if (inFile.peek() == '1')
        {
          outNode->type = ONE_NODE;
          inFile.get();  // throw away '1'
        }
        else
        {
          outNode->type = ZERO_NODE;
        }
      }

      // parse end of file
      else if (words[0] == ".end") break;

      // error
      else
      {
        cout << "ERROR in readBLIF() - invalid line " << words[0] << endl;
        cout << "Possibly wrong format?" << endl;
        return -1;
      }
    }
  }

  cout << "file " << filename << " successfully read." << endl;
  inFile.close();
  return 0;
}

int Circuit::writeBLIF(const string &filename)
{
  ofstream outFile(filename.c_str());
  if (!outFile.good())
  {
    cout << "ERROR in writeBLIF() - cannot open "
         << filename << " for writing." << endl;
    return -1;
  }

  // print model name
  outFile << ".model " << name << endl << endl;

  // print primary inputs
  outFile << ".inputs ";
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    if (it->second->type == PRIMARY_INPUT)
      outFile << it->first << " ";
  }
  outFile << endl << endl;

  // print primary outputs
  outFile << ".outputs ";
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    if (it->second->type == PRIMARY_OUTPUT)
      outFile << it->first << " ";
  }
  outFile << endl << endl;

  // print nodes
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    // must explicitly write 0 node
    if (it->second->type == ZERO_NODE)
    {
      outFile << ".names " << it->second->name << endl;
      outFile << endl;
    }
    // must explicitly write 1 node
    else if (it->second->type == ONE_NODE)
    {
      outFile << ".names " << it->second->name << endl;
      outFile << "1" << endl << endl;
    }
    else if (it->second->type != PRIMARY_INPUT)
    {
      outFile << ".names ";
      for (unsigned i = 0; i < it->second->getNumFanin(); ++i)
        outFile << it->second->getFanin()[i]->name << " ";
      outFile << it->second->name << endl;

      const TruthTable &theTT = it->second->tt;
      for (unsigned i = 0; i < theTT.logic.size(); ++i)
      {
        for (unsigned j = 0; j < theTT.logic[i].size(); ++j)
        {
          switch(theTT.logic[i][j])
          {
            case ZERO: outFile << "0"; break;
            case ONE:  outFile << "1"; break;
            case DC:   outFile << "-"; break;
          }
        }
        outFile << " 1" << endl;
      }
      outFile << endl;
    }
  }

  outFile << ".end" << endl;
  outFile.close();

  cout << "File " << filename << " successfully written." << endl;

  return 0;
}

int Circuit::clear()
{
  for (mapIter it = nodeMap.begin(); it != nodeMap.end(); it++)
  {
    if (it->second != NULL)
      delete it->second;
  }
  nodeMap.clear();
  return 0;
}

