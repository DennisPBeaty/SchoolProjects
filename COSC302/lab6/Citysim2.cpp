//DennisPrestonBeaty
//3/28/2021
//Lab 6.2
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <set>
#include <limits>
#include <stack>

using namespace std;

const long double PI = 3.141592653589793238L;
//City Class
class city{

public:
  string get_name(){
    return name;
  }
  string get_type(){
    return type;
  }
  int get_zone(){
    return zone;
  }
  int get_population(){
    return population;
  }
  float get_longitude(){
    return longitude;
  }
  float get_latitude(){
    return latitude;
  }
  friend istream & operator>>(istream &, city &);
  friend ostream & operator<<(ostream &, const city &);

private:
  string name;
  string type;
  int zone;
  int population;
  float longitude;
  float latitude;
};
//Friend Overloadoperator
istream & operator>>(istream &in, city &cityin) {
  in >> cityin.zone>>cityin.name>>cityin.type>>cityin.latitude>>cityin.longitude>>cityin.population;
  cityin.latitude= cityin.latitude*(PI/180);
  cityin.longitude= cityin.longitude*(PI/180);
  return in;
}

ostream & operator<<(ostream &out, const city &cityin) {
  out << setw(18) <<  left << cityin.name << setw(12) << cityin.type << setw(2) << cityin.zone << setw(10) << right << cityin.population << setw(8) << fixed << setprecision(2)
  << cityin.latitude*(180/PI)  << fixed << setprecision(2) << setw(8) << cityin.longitude*(180/PI);
  return out;
}
//Citysim1 V2
class costtable{
  public:
    float operator () (string mode, int i, int j){
      int buf;
      if (mode== "DISTANCE") {
        if (i<j) {
          buf=i;
          i=j;
          j=buf;
        }
        return distance_ct[i*(i+1)/2+j];
      }
      if (mode=="TIME") {
        if (i<j) {
          buf=i;
          i=j;
          j=buf;
        }
        return time_ct[i*(i+1)/2+j];
      }
      else{
        return 0;
      }
    }
    //create ct
    costtable(vector<city> &cities){
      int N= cities.size();
      distance_ct.resize(N*(N+1)/2);
      time_ct.resize(N*(N+1)/2);
//distance calculations
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
          float deltaphi= abs(cities.at(i).get_latitude()-cities.at(j).get_latitude());
          float deltalambda= abs(cities.at(i).get_longitude()-cities.at(j).get_longitude());
          float deltasigma= 2*asin(sqrt(powf(sin(deltaphi/2),2)+cos(cities.at(i).get_latitude())*cos(cities.at(j).get_latitude())*powf(sin(deltalambda/2),2)));
          float distance=3982*deltasigma;
          distance = 10.0*round(distance/10.0);
          distance_ct[i*(i+1)/2+j]= distance;
        }
      }
      //time calculations
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
          //Ground
          if (cities.at(i).get_type()=="REGIONAL" && cities.at(j).get_type()=="REGIONAL") {
            time_ct[i*(i+1)/2+j]= distance_ct[i*(i+1)/2+j]/60;
          }
          //Air
          else if (cities.at(i).get_type()=="GATEWAY" || cities.at(j).get_type()=="GATEWAY") {
            time_ct[i*(i+1)/2+j]= distance_ct[i*(i+1)/2+j]/570;
          }
        }
      }
    }
  private:
    vector<float> distance_ct;
    vector<float> time_ct;
};

void read_cityinfo(string filename, vector<city> &cities) {
  ifstream ifs;
  ifs.open(filename.c_str());
  if (!ifs.is_open()) {
    std::cerr << "error: couldn't open ./city_list.txt" << '\n';
  }
  city buff;
  string line;

  while(getline(ifs, line)){
    if (line =="" || line.at(0)=='#') {
      continue;
    }
    stringstream sstring;
    sstring << line;
    sstring>>buff;
    cities.push_back(buff);
  }
  ifs.close();
}
void write_cityinfo(vector<city> &cities) {
  string fileout= "city_info.txt";
  ofstream os;
  os.open(fileout.c_str());
  os  << "CITY INFO (N=" << cities.size() << "):\n\n";
  for (size_t i = 0; i < cities.size(); i++) {
    os << setw(3) << right << i << " " << cities.at(i) << '\n';
  }
  os.close();
}

void write_distancect(vector<city> &cities, costtable ct) {
  int N= cities.size();
  string fileout= "city_distancect.txt";
  ofstream os;
  os.open(fileout.c_str());
  os  << "DISTANCE ct:"  << "\n";
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {

      os << setw(3) << right << i << " " << left << setw(38) << setfill('.') <<  cities.at(i).get_name() + " to " + cities.at(j).get_name() + " " << setw(7) <<right << setfill(' ') << fixed << setprecision(1) <<
      ct("DISTANCE", i, j) << " miles\n";
    }
    os << '\n';
  }
  os.close();
 }
void write_timect(vector<city> &cities, costtable ct) {
  int N= cities.size();
  string fileout= "city_timect.txt";
  ofstream os;
  os.open(fileout.c_str());
  os  << "TIME ct:"  << "\n";
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {

      os << setw(3) << right << i << " " << left << setw(38) << setfill('.') <<  cities.at(i).get_name() + " to " + cities.at(j).get_name() + " "<< setw(5) <<right << setfill(' ') << fixed << setprecision(1) <<
      ct("TIME", i, j) << " hours\n";
    }
    os << '\n';
  }
  os.close();

}

void create_citygraph(vector<set<int> > &adjacent, vector<city> cities, costtable ct) {
  int N = cities.size();
  adjacent.resize(N);
  vector<int> types[2];
  for(int i = 0; i < N; i++){
    if(cities[i].get_type() == "REGIONAL"){
      types[0].push_back(i);
    }
    else{
      types[1].push_back(i);
    }
  }
  for(unsigned int i = 0; i < types[0].size(); i++){
    for(unsigned int j = i + 1; j < types[0].size(); j++){
      if(cities[types[0][i]].get_zone() == cities[types[0][j]].get_zone()){
        adjacent[types[0][i]].insert(types[0][j]);
        adjacent[types[0][j]].insert(types[0][i]);
      }
    }
    bool GateFound = 0;
    int closestGate;
    for(unsigned int j = 0; j < types[1].size(); j++)
    {
      if(cities[types[0][i]].get_zone() != cities[types[1][j]].get_zone()){
        continue;
      }
      if(!GateFound){
        closestGate = j;
        GateFound = 1;
      }
      else{
        if(ct("DISTANCE", types[0][i], types[1][j]) < ct("DISTANCE", types[0][i], types[1][closestGate])){
          closestGate = j;
        }
      }
    }
    adjacent[types[0][i]].insert(types[1][closestGate]);
    adjacent[types[1][closestGate]].insert(types[0][i]);
  }
  for(unsigned int i = 0; i < types[1].size(); i++){
    vector<float> closestdistance(6,numeric_limits<float>::max());
    vector<int> savej(6,-1);
    for(unsigned int j = 0; j < types[1].size(); j++){
      if(i == j){
        continue;
      }
      if(cities[types[1][i]].get_zone() == cities[types[1][j]].get_zone()){
        adjacent[types[1][i]].insert(types[1][j]);
        adjacent[types[1][j]].insert(types[1][i]);
        continue;
      }
      if(ct("DISTANCE", types[1][i], types[1][j]) <= 5200){
        if(ct("DISTANCE", types[1][i], types[1][j])<closestdistance.at(cities[types[1][j]].get_zone()-1)){
          closestdistance.at(cities[types[1][j]].get_zone()-1)=ct("DISTANCE", types[1][i], types[1][j]);
          savej.at(cities[types[1][j]].get_zone()-1)=types[1][j];
        }
      }
    }
    for (size_t k = 0; k < 6; k++) {
      if (savej.at(k)!=-1) {
        adjacent[types[1][i]].insert(savej.at(k));
        adjacent[savej.at(k)].insert(types[1][i]);/* code */
      }
    }

  }
}
void write_citygraph(vector<set<int> > adjacent, vector<city> cities, costtable ct) {
	string filen= "city_graph.txt";
	ofstream os;
	os.open(filen.c_str());

	os << "CITY GRAPH:\n\n";

	for(unsigned int i = 0; i < cities.size(); i++){
		os << ' ' << setw(2) << right << i << ' ' << cities[i].get_name() << '\n';
		for(set<int>::iterator it = adjacent[i].begin(); it != adjacent[i].end(); it++){
			os << "  " << setw(4) << right << *it << ' ' << setw(19) << left << cities[*it].get_name() << setw(7) << right << fixed
      << setprecision(1) << ct("DISTANCE",i,*it) << " miles" << setw(6) << fixed << setprecision(1) << ct("TIME",i,*it) << " hours\n";
		}
		os << '\n';
	}
 }

void dijkstra_route(string mode, string from, string to, vector<city> cities, vector<set<int> > adjacent, costtable ct) {
  int source=-1;
  int sink=-1;
  for (size_t i = 0; i < cities.size(); i++) {
    if (cities[i].get_name()==from) {
      source=i;
    }
    if (cities[i].get_name()==to) {
      sink=i;
    }
  }
  if (source==-1||sink==-1) {
    std::cerr << "Nonexistent City Entered" << '\n';
    return;
  }
  //V population
  vector<city> V= cities;
  //E population
  vector<vector<int> > E;
  int N = cities.size();
  E.resize(N);
  for (int i = 0; i < N; i++) {
    for(set<int>::iterator it = adjacent[i].begin(); it != adjacent[i].end(); it++){
      E[i].push_back(*it);
    }
  }

  //declarations
  vector<string> vcolor;
  vector<float> vdist;
  vector<int> vlink;

  //Handout code
  vcolor.assign(V.size(), "WHITE");
  vdist.assign(V.size(), numeric_limits<float>::max());
  vdist[source] = 0;  vlink.assign(V.size(), -1);
  vlink[source] = source;  while (1) {
    int next_i = -1;
    float mindist = numeric_limits<float>::max();
    for (int i=0; i<(int)vcolor.size(); i++) {
      if (vcolor[i] == "WHITE" && mindist > vdist[i]) {
        next_i = i;
        mindist = vdist[i];
      }
    }
    int i = next_i;
    if (i == -1)
    return;
    vcolor[i] = "BLACK";
    if (i == sink)
    break;
    for (int k=0; k<(int)E[i].size(); k++) {
      float wij;
      int j = E[i][k];
      if (mode=="-distance") {
        wij = ct("DISTANCE", i, j);
      }
      if (mode=="-time") {
        wij = ct("TIME", i, j);
      }

      if (vcolor[j] == "WHITE") {
        if (vdist[j] > vdist[i] + wij) {
          vdist[j] = vdist[i] + wij;
          vlink[j] = i;

        }
      }

    }
  }
  if (vdist[sink] == numeric_limits<float>::max()) {
    std::cout << "No path from " << V[source]
              << " to " << V[sink] << "\n";
              return;
  }
  int w=0;
  for (int i = 0; i < (int)V.size(); i++) {
    if (w < (int)V[i].get_name().length()) {
      w= V[i].get_name().length();
    }
  }
  //show path
  vector<int> path;
  vector<city> visited;
  stack<int> S;
  for (int i = sink; i !=source; i=vlink[i]) {
    S.push(i);
    S.push(source);

    while (!S.empty()) {
      int i=S.top();
      S.pop();
      path.push_back(vdist[i]);
      visited.push_back(V[i]);
    }
  }
  reverse(path.begin(),path.end());
  reverse(visited.begin(),visited.end());
  for (size_t i = 0; i < path.size(); i++) {
    if (path.at(i)==0) {
      path.erase(path.begin()+i);
    }
  }
for (size_t i = 0; i < visited.size(); i++) {
  if (visited.at(i).get_name()==from) {
    visited.erase(visited.begin()+i);
  }
}
vector<int> visited_index;
visited_index.push_back(source);
for (size_t i = 0; i < visited.size(); i++) {
  for (size_t j = 0; j < cities.size(); j++) {
    if (visited.at(i).get_name()==cities.at(j).get_name()) {
      visited_index.push_back(j);
      break;
    }
  }
}
//print path
float count_distance=0.0;
float count_time=0.0;
std::cout << setw(7) << right << fixed<< setprecision(1)<< 0.0 << " miles "<< setw(5) << right << fixed << setprecision(1) << 0.0 <<
" hours : " << source << " " << from << '\n';
  //Testing Output
  int savej;
  int time_i;
  int time_j;
  float timesaver=0.0;
  float distancesaver=0.0;
  for (size_t i = 0; i < path.size(); i++) {
    for (size_t j = 0; j < cities.size(); j++) {
      if (cities.at(j).get_name()==visited.at(i).get_name()) {
        savej=j;
      }
    }
    time_i=i;
    time_j=i+1;
    count_time=count_time+ct("TIME",visited_index.at(time_i),visited_index.at(time_j));
    count_distance=count_distance+ct("DISTANCE",visited_index.at(time_i),visited_index.at(time_j));
    std::cout << setw(7) << right << fixed<< count_distance << " miles "<< setw(5) << right << fixed << setprecision(1) << count_time <<
    " hours :" << setw(3) << right  << savej << " " << setw(20) << left << visited.at(i).get_name() << right <<setw(6) << fixed <<setprecision(1)<<
    (count_distance-distancesaver)<< " miles" << setw(6) << fixed << setprecision(1) << (count_time-timesaver) << " hours" <<  '\n';
    timesaver=count_time;
    distancesaver=count_distance;
  }
std::cout  << '\n';
}

int main(int argc, char *argv[])
{
  //commandline option decoding
  if (argc!=2) {
    std::cerr << " : zone ./city_list.txt" << '\n';
    return 1;
  }
  string mode= argv[1];
  //city graph declarations
  vector<set<int> > adjacent;
  vector<city> cities;
  read_cityinfo("city_list.txt",cities);
  //set up costtables
  costtable ct(cities);

  create_citygraph(adjacent, cities, ct);
  if (mode == "-graphinfo") {

    write_cityinfo(cities);
    write_distancect(cities, ct);
    write_timect(cities, ct);
	  write_citygraph(adjacent, cities, ct);
}
//Version 2
if (mode == "-distance" || mode == "-time") {
  string line;
  std::cout << "Enter> ";
  while (std::getline(cin, line)) {
	//enter> from, to cities

    istringstream ss(line);
    string from;
    string to;
    ss >> from >> to;
    dijkstra_route(mode,from,to,cities, adjacent, ct);
      std::cout << "Enter> ";
  }
  std::cout  << '\n';
  }


	//return from program
  return 0;

}
