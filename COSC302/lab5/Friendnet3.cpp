//Dennis Preston Beaty
//Lab 5 V3
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

void set_oldfriends(vector <string> &name, vector<set<int> > &friends, int M0, int M1) {
  int N = (int)name.size();
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
        friends[i].insert(j);
        friends[j].insert(i);
      }
    }
  }
}

void set_newfriends(vector<set<int> > &friends, vector<set<int> > &new_friends) {
  int N = (int)friends.size();
  //initialize 2D-vector-array new_friends (hint: use vector::assign())
  new_friends.resize(N);
  for (int i=0; i<N; i++) {
	//for (each adjacent friend: friends[i][j] == 1) {
  for (set<int>::iterator j = friends[i].begin(); j!= friends[i].end(); j++) {
    for (set<int>::iterator iter = friends[*j].begin(); iter!= friends[*j].end(); iter++) {
      if (*iter!=i && find(friends[i].begin(), friends[i].end(), *iter) == friends[i].end()) {
        new_friends[i].insert(*iter);
        new_friends[*iter].insert(i);
      }
    }
    }
  }
}

void writetofile(const char *fname, vector<string> &name, vector<set<int> > &friends) {
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
  for (set<int>::iterator iter = friends[i].begin(); iter!= friends[i].end(); iter++) {
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
  vector<set<int> > friends;
  //declare 2D-vector-array called new_friends
  vector<set<int> > new_friends;
  set_oldfriends(name, friends, M0, M1);
  writetofile("doknow3.txt", name, friends);
  set_newfriends(friends, new_friends);
  writetofile("mightknow3.txt", name, new_friends);
}
