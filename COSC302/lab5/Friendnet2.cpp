//Dennis Preston Beaty
//Lab 5 V2
//3/13/2021
//COSC302
#include <vector>
#include <string>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>
#include <cstring>
#include <algorithm>
using namespace std;

void set_oldfriends(vector <string> &name, vector<vector<int> > &friends, int M0, int M1) {
  int N = (int)name.size();
  vector<int>::iterator Uijp;
  //initialize 2D-vector-array friends (hint: use vector::assign())
  //initialize N rows of vector strings, second arg is the vector<string>
  friends.resize(N);
  for (int i=0; i<N; i++) {
    //declare std::set called doknow
  set<int> doknow;
    //initialize M (random number of friends: function of M0, M1)
  int M = M0 + rand() % M1;
    //while ((int)doknow.size() < M) {
  while ((int)doknow.size()<M) {
      //compute j (random friend index: hint j != i)
      int j= rand() % N;
      if (j != i) {
        doknow.insert(j);
        //update the corresponding pairs of friends entries
        friends[i].push_back(j);
        friends[j].push_back(i);
        //sort i
        sort(friends[i].begin(), friends[i].end());
        Uijp = unique(friends[i].begin(), friends[i].end());
        friends[i].resize(Uijp - friends[i].begin());
        //sort j
        sort(friends[j].begin(), friends[j].end());
        Uijp = unique(friends[j].begin(), friends[j].end());
        friends[j].resize(Uijp - friends[j].begin());
      }
    }
  }
}

void set_newfriends(vector<vector<int> > &friends, vector<vector<int> > &new_friends) {
  int N = (int)friends.size();
  vector<int>::iterator Uijp;
  //initialize 2D-vector-array new_friends (hint: use vector::assign())
  new_friends.resize(N);
  for (int i=0; i<N; i++) {
	//for (each adjacent friend: friends[i][j] == 1) {
  for (size_t j = 0; j < friends[i].size(); j++) {
    for (vector<int>::iterator iter = friends[friends[i].at(j)].begin(); iter!= friends[friends[i].at(j)].end(); iter++) {
      if (*iter!=i && find(friends[i].begin(), friends[i].end(), *iter) == friends[i].end()) {
        new_friends[i].push_back(*iter);
        new_friends[*iter].push_back(i);
        sort(new_friends[i].begin(), new_friends[i].end());
        Uijp = unique(new_friends[i].begin(), new_friends[i].end());
        new_friends[i].resize(Uijp - new_friends[i].begin());
        sort(new_friends[*iter].begin(), new_friends[*iter].end());
        Uijp = unique(new_friends[*iter].begin(), new_friends[*iter].end());
        new_friends[*iter].resize(Uijp - new_friends[*iter].begin());
      }
    }
    }
  }
}

void writetofile(const char *fname, vector<string> &name, vector<vector<int> > &friends) {
  ofstream os;
  os.open(fname);

  int N = (int)name.size();
  //determine max name length
  int max_length=0;
  for (size_t i = 0; i < name.size(); i++) {
    if ((unsigned)max_length<name.at(i).length()) {
      max_length=name.at(i).length();
    }
  }
  for (int i=0; i<N; i++) {
    int count=0;

  //for (each adjacent friend: friends[i][j] == 1) {
  for (vector<int>::iterator iter = friends[i].begin(); iter!= friends[i].end(); iter++) {
	  //pretty-print name[i] and name[j] (see assignment)
    //if 8 names have already been listed new line
    if ((count%8)==0) {
      if (i != 0 || (count % 8 == 0 && count != 0)) {
        os <<'\n';
      }
      os << setw(max_length)  << left << name.at(i) << " :";
    }
      os << ' ' << setw(max_length) << left <<name.at(*iter);
      count++;

  }
}
  os << "\n";
  os.close();

}

int main(int argc, char *argv[]) {
  //parse argc, argv arguments
  //print usage message and exit if invalid
  if (argc!=1 && argc!=3) {
    cerr << "  :   -seed usage:   [-seed N]" << '\n';
    return 1;
  }
  //seed random number generator
  if (argc==3) {
    srand (atoi(argv[2]));
  }
  else{
    time(NULL);
  }
  //vector<string> name;
  vector<string> name;
  string add="";
  //read strings from stdin into name vector
  while (cin >> add) {
    name.push_back(add);
  }
  int M0 = 1; // min number of friends
  int M1 = 3; // max number of friends

  //declare 2D-vector-array called friends
  vector<vector<int> > friends;
  //declare 2D-vector-array called new_friends
  vector<vector<int> > new_friends;
  set_oldfriends(name, friends, M0, M1);
  writetofile("doknow2.txt", name, friends);
  set_newfriends(friends, new_friends);
  writetofile("mightknow2.txt", name, new_friends);
}
