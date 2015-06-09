/*
 * Contains the the apply function, the cofactors and quantification functions.
 *
 * For Project 1, implement
 * (1) apply, also handles probabilities
 * (2) negative_cofactor
 * (3) positive_cofactor,
 * (4) boolean_difference
 * (5) sort by influence
 */

#include "project1.h"

using namespace std;

// wrapper function to allow calling with the operation, i.e apply(bdd1, bdd2, "or")
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, string o)
{
  operation dop;
  if (!dop.set_operation(o))
  {
    return 0;
  }

  return apply(bdd1, bdd2, dop);
}

// apply implements an arbitrary operation (specified in op) on two BDDs
// bdd_tables is used to handle the book keeping (see bdd_tables.h).
//
// apply works recursively one the idea that given an arbitrary operation $,
// and functions f and g: f $ g = a'(fa' $ ga') + a(fa $ ga),
// where a is a variable, fa' is the negative cofactor etc.
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, operation &op)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();

  // ... your code goes here
  bdd_ptr res;
    
  // check to see if the res already exists
  res = tables.find_in_computed_table(op.get_operation(), bdd1, bdd2);
  if (res) // res exists
  {
    return res;
  }
    
  // perform binary operations
  res = op(bdd1, bdd2);
  if (res)
  {
    return res;
  }

  // choose a next var to split on
  char var = find_next_var(bdd1, bdd2);

  bdd_ptr left = apply(negative_cofactor(bdd1, var), negative_cofactor(bdd2, var), op);
  bdd_ptr right = apply(positive_cofactor(bdd1, var), positive_cofactor(bdd2, var), op);

  if (left != right)
  {
    // check to see if the res already exists
    res = tables.find_in_unique_table(var, left, right);
    if (res) // res exists
    {
        return res;
    }
    else // res doesn't exist, create the new node and add it to the unique table
    {
        res = tables.create_and_add_to_unique_table(var, left, right);
        // calculate the probability of Boolean function
        res->probability = (left->probability + right->probability)/2;
    }
  }
  else // left = right, no node need to be created
  {
    res = right;
  }

  // add the res to the computed table
  tables.insert_computed_table(op.get_operation(), bdd1, bdd2, res);

  // change the return value when you're finished
  return res;
}

// negative_cofactor takes the BDD pointed to by np,
// and returns the negative cofactor with respect to var.
bdd_ptr negative_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();

  //... your code goes here
  // check to see if np point to a terminal node
  if (np->is_zero()) // np point to terminal node zero
  {
    return np;
  }
  else if (np->is_one()) // np point to terminal node one
  {
    return np;
  }
  else if (np->var == var) // np point to a non-terminal node
  {
    return np->neg_cf; // negative cofactor
  }

  bdd_ptr left = negative_cofactor(np->neg_cf, var);
  bdd_ptr right = negative_cofactor(np->pos_cf, var);
  bdd_ptr res;

  if (left != right)
  {
    // check to see if the res already exists
    res = tables.find_in_unique_table(np->var, left, right);
    if (res) // res exists
    {
        return res;
    }
    else // res doesn't exist, create the new node and add it to the unique table
    {
        res = tables.create_and_add_to_unique_table(np->var, left, right);
        // calculate the probability of Boolean function
        res->probability = (left->probability + right->probability)/2;
    }
  }
  else // left = right, no node need to be created
  {
    res = right;
  }

  // once you add your code, remove this and return your own result
  return res;
}

// posative_cofactor takes the BDD pointed to by np,
// and returns the posative cofactor with respect to var.
bdd_ptr positive_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();

  //... your code goes here
  // check to see if np point to a terminal node
  if (np->is_zero()) // np point to terminal node zero
  {
    return np;
  }
  else if (np->is_one()) // np point to terminal node one
  {
    return np;
  }
  else if (np->var == var) // np point to a non-terminal node
  {
    return np->pos_cf; // positive cofactor
  }


  bdd_ptr left = positive_cofactor(np->neg_cf, var);
  bdd_ptr right = positive_cofactor(np->pos_cf, var);
  bdd_ptr res;

  if (left != right)
  {
    // check to see if the res already exists
    res = tables.find_in_unique_table(np->var, left, right);
    if (res) // res exists
    {
        return res;
    }
    else // res doesn't exist, create the new node and add it to the unique table
    {
        res = tables.create_and_add_to_unique_table(np->var, left, right);
        // calculate the probability of Boolean function
        res->probability = (left->probability + right->probability)/2;
    }
  }
  else // left = right, no node need to be created
  {
    res = right;
  }

  // once you add your code, remove this and return your own result
  return res;
}

// boolean_difference takes the BDD pointed to by np,
// and returns the boolean difference with respect to var.
bdd_ptr boolean_difference(bdd_ptr np, char var)
{
  // get reference to tables
  //bdd_tables& tables = bdd_tables::getInstance();

  //... your code goes here
  bdd_ptr res;
  // the negative cofactor XORed with the positive cofactor
  res = apply(negative_cofactor(np, var), positive_cofactor(np,var), "xor");

  // once you add your code, remove this and return your own result
  return res;
}

//check if two bdd are p equivalence, return true if they are p equivalence
//otherwise return false.
bool check_P_equivalence(bdd_ptr bdd1, bdd_ptr bdd2)
{
  bool is_P_equivalence = false;
  //... your code goes here
  // check to see if bdd1 and bdd2 have the same probability
  if (bdd1->probability == bdd2->probability)
  {
    is_P_equivalence = true;
  }
  // this function does not alter the current node, so np must be
  // returned at the end
  return is_P_equivalence;
}
//check if two bdds are SC equivalence, return true if they are;
bool check_SC_equivalence(bdd_ptr bdd1, bdd_ptr bdd2,int num_vars)
{
  bool is_SC_equivalence = false;
  //... your code goes here
  char var = find_next_var(bdd1, bdd2);

  if (num_vars == 0)
  {
    is_SC_equivalence = check_P_equivalence(bdd1, bdd2);
  }
  else
  {
      for (; num_vars >= 1; num_vars--)
      {
          bool check1 = check_P_equivalence(positive_cofactor(bdd1, var), positive_cofactor(bdd2, var));
          bool check2 = check_P_equivalence(negative_cofactor(bdd1, var), negative_cofactor(bdd2, var));
          if (check1 && check2)
          {
              is_SC_equivalence = true;
          }
      }

  }


  // this function does not alter the current node, so np must be
  // returned at the end
  return is_SC_equivalence;
}



