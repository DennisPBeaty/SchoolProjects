// include header file(s) needed
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
using namespace std;

#include "Slist.h"

// modify the data class and the associated input
// and output operators to store, set and print a
// line number identifier called ID

//class data { copy from QsortA.cpp };
int IDposition=1;
class data {
  public:

    friend istream & operator>>(istream &, data &);
    friend ostream & operator<<(ostream &, const data &);

    bool operator<(const data &rhs) const{
      if(lastname<rhs.lastname){
        return true;
      }
      if (lastname==rhs.lastname) {
        if (firstname<rhs.firstname) {
          return true;
        }
        if (firstname==rhs.firstname) {
          if (phonenum<rhs.phonenum) {
            return true;
          }
          else{
            return false;
          }
        }
        else{
          return false;
        }
      }
      else{
        return false;
      }
    }

  private:
    string firstname;
    string lastname;
    string phonenum;
    int id;
};
//istream & operator>> (...) { copy from Qsort.cpp }
istream & operator>>(istream &in, data &r) {
  // write this to read data object data
  in >> r.firstname;
  in >> r.lastname;
  in >> r.phonenum;
  r.id= IDposition;
  IDposition++;
  return in;
}
//ostream & operator<< (...) { copy from Qsort.cpp }
ostream & operator<<(ostream &out, const data &r) {
  // write this to write data object data
  out << " " << setw(24) << left << r.lastname  + " " + r.firstname << r.phonenum
  <<  setw(9) << right << r.id;
  return out;
}
//void printlist(...) { copy from QsortA.cpp }
template <typename Tdata>
void printlist(Tdata begin, Tdata end) {
  for (auto it= begin; it != end; ++it){

    cout  << *it << '\n';
  }
}

int main(int argc, char *argv[]) {
  // copy command-line check from Qsort.cpp,
  // then modify to apply to this assignment
  string filename= "";
  if (argc!=3) {
    cerr << "usage: ./Slist -mergesort|quicksort|qsort_r3 file.txt\n";
    return 1;
  }
    string mode= argv[1];
  if (mode=="-mergesort" || mode=="-quicksort" || mode=="-qsort_r3") {
    filename= argv[2];
  }
  else{
    cerr << "usage: ./Slist -mergesort|quicksort|qsort_r3 file.txt\n";
    return 1;
  }

  // open input file
  ifstream filestream;
  filestream.open(filename.c_str());
  slist<data> A;

  data din;
  string line;
  //while (reading more data)
  while (getline(filestream, line)) {
    istringstream stringstream(line);
    stringstream >> din;
      //store data INORDER
    A.push_back(din);
  }

  // close input file
filestream.close();
  A.sort(mode);

  printlist(A.begin(), A.end());
}
