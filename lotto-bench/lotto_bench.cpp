#include <iostream>

#include "armadillo"

using namespace arma;
using namespace std;


int main(int argc, char** argv)
{
  cout << "Armadillo version: " << arma_version::as_string() << endl;
  
  return 0;
}

