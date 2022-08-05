//Dennis Preston Beaty
//January 26th, 2021
//Lab1
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

struct fruit {
  //overload operator <
  bool operator<(const fruit &rhs) const{
    if(name<rhs.name){
      return true;
    }
    return false;
  }

  string name;
  float amount;
  float price;
};

int main(int argc, char *argv[])
{

    //prog -inorder|sortall|sortone file.txt
    string filename = argv[2];
    string mode;
    string sortype= argv[1];


    //set program mode from command line arg
    if (sortype=="-inorder") {
      mode= "INORDER";
    }
    if (sortype=="-sortall") {
      mode= "SORTALL";
    }
    if (sortype=="-sortone") {
      mode= "SORTONE";
    }
    if (argc!=3 || (mode!= "SORTONE" && mode!= "SORTALL" && mode!= "INORDER")) {
      std::cerr << "" << '\n';
      return 1;
    }
  //declare array_list<fruit>
  std::vector<fruit> array;

  //open file
  ifstream filestream;
  filestream.open(filename.c_str());

  string line;
  //while (reading more data)
  while (getline(filestream, line)) {
    fruit temp;
    istringstream stringstream(line);
    stringstream >> temp.name >> temp.amount >> temp.price;
      //store data INORDER
    array.push_back(temp);
  }

  filestream.close();



  //if (mode == SORTALL || mode == SORTONE)
  if (mode=="SORTALL" || mode=="SORTONE") {
    //apply std::stable_sort
    std::stable_sort (array.begin(), array.end());
  }


  //if (mode == SORTONE)
  if (mode=="SORTONE"){
    //combine fruit quanties with same name
    for (size_t i = 0; i < array.size(); i++) {
      if (i==array.size()-1) {
        break;
      }
      for (size_t j = 0; j < array.size(); j++) {
        if (i!=j) {
        if (array.at(i).name==array.at(j).name) {
          array.at(i).amount+=array.at(j).amount;
            //free array list memory as necessary
          array.erase(array.begin()+j);
          j--;
      }
    }
    }
    }
  }


  //pretty-print array list content to stdout
  float total= 0.0;
  for (size_t i = 0; i < array.size(); i++){
    std::cout << setfill('.') << setw(20) << left << array.at(i).name << ' ' << fixed << right << setprecision(2) << setfill(' ') << setw(5) <<
    array.at(i).amount << " x " << setw(4) << array.at(i).price << " = ";
    float addtotal= array.at(i).amount*array.at(i).price;
    total= total+addtotal;
    std::cout << setw(7) << addtotal << "  : " << setw(8) << total << '\n';
  }

}
