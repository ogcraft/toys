#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "armadillo"

using namespace arma;
using namespace std;

struct Lot_type {
  string name;
  int price;
  int count;
  int sum;
  imat prizes;
  void print(const char* m = "")
  {
    cout << m << "Lot: " << name << " price: "<< price << " count: " << count << " sum: " << sum << endl;
    prizes.print("Prizes:");
  }
};

typedef vector<Lot_type> lot_types_vec;

void init_data(lot_types_vec& lot_vec)
{
  Lot_type l;

  l.name  = "bigluck";
  l.price = 5;
  l.count = 2000000;
  l.sum = 651810;
  l.prizes << 5     << 320000 << endr
           << 10    << 290000 << endr 
           << 20    << 31000  << endr
           << 30    << 9000   << endr
           << 100   << 1600   << endr
           << 500   << 200    << endr
           << 25000 << 10     << endr;
  
  lot_vec.push_back(l);

  l.name  = "hg-babushka";
  l.price = 5;
  l.count = 2000000;
  l.sum = 307010;
  l.prizes  << 10     << 81000 << endr
            << 15     << 10000 << endr
            << 20     << 7000 << endr
            << 25     << 6000 << endr
            << 100    << 1000 << endr
            << 200    << 1000 << endr
            << 350    << 1000 << endr
            << 5000  << 10 << endr;
  
  lot_vec.push_back(l);

}
/////////////   End of data /////////////////////////
int fill_tickets(const Lot_type& l, ivec& tickets)
{
  int total = 0;
  imat::const_col_iterator i = l.prizes.begin_col(0);
  imat::const_col_iterator j = l.prizes.begin_col(1);
  ivec::iterator t = tickets.begin(); 
  for(; i != l.prizes.end_col(0); ++i, ++j)
  {
    int win = *i;
    int num = *j;
    cout << "win: " << win << " num:" << num << endl;
    for(int n = 0; n < num; n++)
    {
      tickets[total]=win;
      total++;
      ++t;
    }
  }
  std::fill(t,tickets.end(),0);
  cout << "Filled total: " << total << endl;
  return total;
}

int main(int argc, char** argv)
{
  cout << "Armadillo version: " << arma_version::as_string() << endl;
  
  arma_rng::set_seed_random();

  lot_types_vec lot_vec;
  init_data(lot_vec);
  
  for (lot_types_vec::iterator it = lot_vec.begin(); it != lot_vec.end(); it++) {
    it->print();
    //sum(prod(it->prizes,1)).print("s:");
    cout << "----------------------" << endl;
  }
  cout << "Total lot types: " << lot_vec.size() << endl << endl; 

  // prepare deck
  Lot_type& l = lot_vec[0];
/*  
  auto_ptr<ivec> pidxs(new ivec); 
  ivec& idxs = *pidxs;
  idxs = randi<ivec>(l.count, distr_param(0,l.count));
  
  cout << "Idxs: sz: " << idxs.size() << " max: " << idxs.max() << " min: " << idxs.min()<< endl;
*/
  auto_ptr<ivec> ptickets(new ivec(l.count)); 
  ivec& tickets = *ptickets;
  tickets.fill(-1);
  
  fill_tickets(l, tickets);

  cout << "Issued Tickets: " << tickets.size() << " max: " << tickets.max() << " min: " << tickets.min() << endl;
  

  return 0;
}

