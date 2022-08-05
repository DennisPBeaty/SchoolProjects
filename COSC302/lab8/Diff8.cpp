//Dennis Preston Beaty
//Lab 8
//4/20/2021
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stack>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stack>
using namespace std;

template <typename T> class matrix {
    size_t Nrows, Ncols;
    T **   data;
  public:
    matrix() : data(NULL) {}
    ~matrix() {
        if (!data)
            return;
        delete[] * data;
        delete[] data;
        data = NULL;
    }
    void assign(size_t _Nrows, size_t _Ncols, const T &value = T()) {
        this->Nrows               = _Nrows;
        this->Ncols               = _Ncols;
        *(data = new T *[_Nrows]) = new T[_Nrows * _Ncols];
        std::fill(*data, *data + _Nrows * _Ncols, value);
        for (T **beg = data + 1; beg < data + _Nrows; ++beg) *beg = *(beg - 1) + _Ncols;
    }
    size_t   get_Nrows() const { return Nrows; }
    size_t   get_Ncols() const { return Ncols; }
    T *      operator[](size_t i) { return data[i]; }
    const T *operator[](size_t i) const { return data[i]; }
};

class LCS {
  public:
    //store text in to text1 buffer
    void text1_push_back(string filename){
      string line;
      ifstream myfile(filename.c_str());

      if(!myfile){
          std::cerr << "Error Opening File Argv[1]" << '\n';
          exit(EXIT_FAILURE);
      }

      while (getline(myfile, line)){
          text1.push_back(line);
      }

      myfile.close();
    }
    //store text in to text2 buffer
    void text2_push_back(string filename){
      string line;
      ifstream myfile(filename.c_str());

      if(!myfile){
          std::cerr << "Error Opening File Argv[1]" << '\n';
          exit(EXIT_FAILURE);
      }

      while (getline(myfile, line)){
          text2.push_back(line);
      }

      myfile.close();
    }

	  void compute_alignment(){
      m= text1.size();
      n= text2.size();
      cost.assign(m+1,n+1);
      link.assign(m+1,n+1);

      for(int i = 0; i <= m; i++){
        cost[i][0] = i;
        link[i][0] = 1;
      }
      for(int j = 0; j <= n; j++){
        cost[0][j] = j;
        link[0][j] = 2;
      }

      link[0][0]=0;

      for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
          if (text1[i]==text2[j]) {
            cost[i+1][j+1]= cost[i][j];
          }
          else{
            if(cost[i][j+1]< cost[i+1][j]){
              cost[i+1][j+1]= cost[i][j+1]+ 1;
            }
            else{
              cost[i+1][j+1]= cost[i+1][j]+ 1;
            }
          }
        }
      }
      for (int k = 1; k <= m; k++) {
        for (int l = 1; l <= n; l++) {
          if(text1[k-1]==text2[l-1]){
            link[k][l]=4;
          }
          else if (cost[k][l]-1==cost[k][l-1]) {
            link[k][l]=2;
          }
          else if (cost[k][l]-1==cost[k-1][l]) {
            link[k][l]=1;
          }
          else{
            break;
          }
        }
      }
    }

    void report_difference(){
      m = text1.size();
      n = text2.size();
      report_difference(m, n);
    }

    int report_difference(int one, int two){
      if (one==0 && two==0) {
        int deleter=0;
        int inserter=0;
        int x= 0, y= 0;

        for (int k = (int)trace.size(); k >=0; k--) {
          if (trace[k]==4) {
            print(x,y,deleter,inserter);
            deleter=0;
            inserter=0;
            x=x+1;
            y=y+1;
          }
          else if(trace[k]==1){
            deleter=deleter+1;
            x= x+1;
          }
          else if(trace[k]==2){
            inserter=inserter+1;
            y=y+1;
          }
        }
      print(x,y,deleter,inserter);
    }

    int direction= link[one][two];

    if (direction==4) {
      trace.push_back(direction);
      report_difference(one-1,two-1);
    }
    if (direction==2) {
      trace.push_back(direction);
      report_difference(one,two-1);
    }
    if (direction==1) {
      trace.push_back(direction);
      report_difference(one-1,two);
    }
    return 0;
    };
    void print(int i, int j, int remove, int add){
      int one;
      int two;
      one = i - remove;
      two = j - add;
      if(add!= 0&&remove== 0){
        if(add== 1){
          cout << i << "a" << j << "\n" << "> " << text2[two] << "\n";
        }
        else{
          cout << i << "a" << j - add + 1 << "," << j << "\n";
          for(int k= 0; k < add; k++){
            cout << "> " << text2[two] << "\n";
            two=two+1;
          }
        }
      }
      else if(remove!= 0 &&add== 0){
        if(remove== 1){
          cout << i << "d" << j << "\n" << "< " << text1[one] << "\n";
        }
        else{
          cout << i - remove + 1 << "," << i << "d" << j << "\n";
          for(int k= 0;k< remove;k++){
            cout << "< " << text1[one] << "\n";
            one=one+1;
          }
        }
      }
      else if(remove!= 0&&add!= 0){
        if(remove== 1){
          cout << i;
        }
        else{
          cout << i-remove+1 << "," << i;
        }
        cout << "c";
        if(add== 1){
          cout << j;
        }
        else{
          cout << j-add+1 << "," << j;
        }
        cout << "\n";
        for(int k=0; k<remove;k++){
          cout << "< " << text1[one] << "\n";
          one=one+1;
        }
        cout << "---\n";
        for(int k= 0; k<add;k++){
          cout << "> " << text2[two] << "\n";
          two=two+1;
        }
      }
    };
  private:
    int m, n;
    vector<int> trace;
    // text1 buffer needs to be a vector
    vector<string> text1;
    // text2 buffer needs to be a vector
    vector<string> text2;

    matrix<int> cost; // edit costs
    matrix<int> link; // alignment info
};

int main(int argc, char *argv[])
{
  // check two input files are specified on command line
  if (argc!=3) {
    std::cerr << "Incorrect number of input files specified" << '\n';
    return 1;
  }

  string text1file= argv[1];
  string text2file= argv[2];
  LCS lcs;  // instantiate your "LCS based diff" object

  // read the text from file1 into the lcs.text1 buffer
  lcs.text1_push_back(text1file);
  // read the text from file2 into the lcs.text2 buffer
  lcs.text2_push_back(text2file);

  lcs.compute_alignment();
  lcs.report_difference();
}
/*
Report Diff Pseudo Code
if(base case) at start of link matrix {
  loop through trace

  keep track of # of deletes and inserts

  if no delete then a
  if no insert then d
  if both are nonzero c

  print at next match

  here use text 1 and text
}

what is stored link[i][j]

push to trace

change i and j

call recursive
*/
