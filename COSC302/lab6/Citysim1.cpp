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

using namespace std;

const long double PI = 3.141592653589793238L;

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
    //not sure if overload operator declaration is correct below
    float operator () (string mode, int i, int j){
      int buf;
      if (mode== "DISTANCE") {
        if (i<j) {
          buf=i;
          i=j;
          j=buf;
        }
        return distance_table[i*(i+1)/2+j];
      }
      if (mode=="TIME") {
        if (i<j) {
          buf=i;
          i=j;
          j=buf;
        }
        return time_table[i*(i+1)/2+j];
      }
      else{
        return 0;
      }
    }
    costtable(vector<city> &cities){
      int N= cities.size();
      distance_table.resize(N*(N+1)/2);
      time_table.resize(N*(N+1)/2);

      for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
          float deltaphi= abs(cities.at(i).get_latitude()-cities.at(j).get_latitude());
          float deltalambda= abs(cities.at(i).get_longitude()-cities.at(j).get_longitude());
          float deltasigma= 2*asin(sqrt(powf(sin(deltaphi/2),2)+cos(cities.at(i).get_latitude())*cos(cities.at(j).get_latitude())*powf(sin(deltalambda/2),2)));
          float distance=3982*deltasigma;
          distance = 10.0*round(distance/10.0);
          distance_table[i*(i+1)/2+j]= distance;
        }
      }
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
          //Ground
          if (cities.at(i).get_type()=="REGIONAL" && cities.at(j).get_type()=="REGIONAL") {
            time_table[i*(i+1)/2+j]= distance_table[i*(i+1)/2+j]/60;
          }
          //Air
          else if (cities.at(i).get_type()=="GATEWAY" || cities.at(j).get_type()=="GATEWAY") {
            time_table[i*(i+1)/2+j]= distance_table[i*(i+1)/2+j]/570;
          }
        }
      }
    }
  private:
    vector<float> distance_table;
    vector<float> time_table;
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

void write_distancetable(vector<city> &cities, costtable ct) {
  int N= cities.size();
  string fileout= "city_distancetable.txt";
  ofstream os;
  os.open(fileout.c_str());
  os  << "DISTANCE TABLE:"  << "\n";
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {

      os << setw(3) << right << i << " " << left << setw(38) << setfill('.') <<  cities.at(i).get_name() + " to " + cities.at(j).get_name() + " " << setw(7) <<right << setfill(' ') << fixed << setprecision(1) <<
      ct("DISTANCE", i, j) << " miles\n";
    }
    os << '\n';
  }
  os.close();
 }
void write_timetable(vector<city> &cities, costtable ct) {
  int N= cities.size();
  string fileout= "city_timetable.txt";
  ofstream os;
  os.open(fileout.c_str());
  os  << "TIME TABLE:"  << "\n";
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {

      os << setw(3) << right << i << " " << left << setw(38) << setfill('.') <<  cities.at(i).get_name() + " to " + cities.at(j).get_name() + " "<< setw(5) <<right << setfill(' ') << fixed << setprecision(1) <<
      ct("TIME", i, j) << " hours\n";
    }
    os << '\n';
  }
  os.close();

}

//create_citygraph() { }
//write_citygraph() { }

//dijkstra_route() { }

int main(int argc, char *argv[])
{
  //commandline option decoding
  if (argc!=2) {
    std::cerr << " : zone ./city_list.txt" << '\n';
    return 1;
  }
  string mode= argv[1];
  //city graph declarations
  vector<city> cities;
  read_cityinfo("city_list.txt",cities);
  //set up costtables
  costtable ct(cities);
  //create_citygraph()

  if (mode == "-graphinfo") {

    write_cityinfo(cities);
    write_distancetable(cities, ct);
    write_timetable(cities, ct);
	//write_citygraph()

	//return from program
  return 0;
  }
//Version 2
  /*while (not done) {
	//enter> from, to cities
    string from;
    string to;
    dijkstra_route(mode,from,to);
  }*/
}
