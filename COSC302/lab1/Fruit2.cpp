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
  //overload operators <, <=, ==, and +=
  bool operator<(const fruit &rhs) const{
    if(name<rhs.name){
      return true;
    }
    return false;
  }
  bool operator<=(const fruit &rhs) const{
    if(name<rhs.name||name==rhs.name){
      return true;
    }
    return false;
  }
  bool operator==(const fruit &rhs) const{
    if(name==rhs.name){
      return true;
    }
    return false;
  }
  void operator+=(const fruit &rhs){
    amount=amount+rhs.amount;
  }

  //data declarations
  string name;
  float amount;
  float price;
};

struct node {
  //constructor
  node(fruit);
  node();
  //data declarations
  node *next;
  fruit data;
};

node::node(fruit n_data) {
 data = n_data;
 next = NULL;
}

node::node(){
  next=NULL;
  data.name="";
}

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
  //declare linked_list<fruit>
  fruit blank;
  blank.name= "";
  blank.amount=0;
  blank.price=0;
  node *header= new node();

  string line;
  //open file
  node *track= header;
  node *track2;
  ifstream filestream;
  filestream.open(filename.c_str());
    //while (reading more data)
    if (mode=="INORDER") {
      /* code */
    while (getline(filestream, line)) {
      fruit temp;
      istringstream stringstream(line);
      stringstream >> temp.name >> temp.amount >> temp.price;
        //store data INORDER
      if(header->data.name==""){
        header->data= temp;
      }
      else{
        track->next= new node(temp);
        track= track->next;
      }
    }
    }
    //Sort lexicographically
    if (mode=="SORTALL" || mode=="SORTONE") {
    while (getline(filestream, line)) {
      fruit temp;
      istringstream stringstream(line);
      stringstream >> temp.name >> temp.amount >> temp.price;
      if(header->data.name == ""){
				header->data = temp;
			}
			else if(temp < header->data){
				track2= header;
				header= new node(temp);
				header->next = track2;
			}
			else{
				bool add = false;
				while(track->next != NULL){
					if(temp < track->next->data){
						track2 = track->next;
						track->next = new node(temp);
						track->next->next = track2;

						add = true;
						track = header;
						break;
					}
					track = track->next;
				}
				if(add == false){
					track->next = new node(temp);
				}
				track = header;
			}
      track= header;
      }
    }

    filestream.close();

    track = header;
    //node *p2= new node();
  	//store data INORDER | SORTALL | SORTONE
  	if(mode == "SORTONE"){
  		while(track->next != NULL){
  			if(track->data == track->next->data){
  				track->data += track->next->data;
          node *track3= track->next;
          track->next= (track3->next);
          track3->next= NULL;
          delete track3;

  			}
  			else{
  				track = track->next;
  			}
  		}
  	}
  //pretty-print array list content to stdout

  track = header;
  float totalreal= 0.0;
  while(track != NULL){
  cout << setfill('.') << setw(20) << left << track->data.name;
  cout << ' ' << fixed << right << setprecision(2) << setfill(' ') << setw(5) << track->data.amount;
  cout << " x " << setw(4) << track->data.price << " = ";

  float totalcurr = track->data.amount * track->data.price;
  totalreal = totalreal+totalcurr;
  cout << setw(7) << totalcurr << "  : " << setw(8) << totalreal << '\n';
    track = track->next;
}

 track = header;

  while (header) {
    node *delet = header;
    header = header->next;
    delete delet;
  }
}
