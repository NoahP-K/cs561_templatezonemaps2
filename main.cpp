#include <fstream>
#include <vector>
#include <utility>
#include <iostream>
#include <random>
#include <set>
#include <algorithm>
#include <functional>
#include <chrono>
#include "zonemaps.h"
#include "zonemaps.cpp"

// Specify your path of workload file here
std::string kInputDataPath = "./workload.dat";
std::string kPointQueriesPath = "./point_queries.txt";
std::string kRangeQueriesPath = "./range_queries.txt";
const uint32_t kRuns = 3;
using namespace std;

void loadPointQueries(std::string & input_queries_path, std::vector<int> & queries)
{
  queries.clear();
  std::ifstream infile(input_queries_path, ios::in);
  int tmp;
  while (infile >> tmp) {
	  queries.push_back(tmp);
  }
  // shuffle indexes
  std::random_shuffle(queries.begin(), queries.end());

}

void loadRangeQueries(std::string & input_queries_path, std::vector<std::pair<int, int>> & queries)
{
  //Range queries are stored with the lower bound on the first line and the upper bound on the second.
  queries.clear();
  std::ifstream infile(input_queries_path, ios::in);
  int tmp;
  int tmp2;
  while (infile >> tmp) {
    infile >> tmp2;
    pair<int, int> pr(tmp, tmp2);
	  queries.push_back(pr);
  }
  // shuffle indexes
  std::random_shuffle(queries.begin(), queries.end());

}

int main(int argc, char **argv)
{ 

  // read data
  std::ifstream ifs;
  std::vector<int> data;

  ifs.open(kInputDataPath, std::ios::binary);
  ifs.seekg(0, std::ios::end);
  size_t filesize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  data.resize(filesize / sizeof(int));
  ifs.read((char *)data.data(), filesize);

  //1. ----------------------------- initialize zonemap and build -----------------------------
  //build zonemap
  //std::cout << "ZONE SIZE: " <<  (uint)data.size() / 100 << std::endl;
  zonemap<int> zones(data, (uint)data.size() / 10);

  //2. ----------------------------- point queries -----------------------------
  std::vector<int> queries;
  loadPointQueries(kPointQueriesPath, queries); 

  auto start_pq = std::chrono::high_resolution_clock::now();
  for (size_t r = 0; r < kRuns; r++) {
    size_t ptotal = 0;
    for (int pq: queries) {
      // query from zonemaps here 
      ptotal += zones.query(pq);
    }
    std::cout << "Point query test #" << r << ": " << ptotal << " total matches." << endl;

    // shuffle indexes
    std::random_shuffle(queries.begin(), queries.end());
  }
  auto stop_pq = std::chrono::high_resolution_clock::now();
  auto duration_pq = std::chrono::duration_cast<std::chrono::microseconds>(stop_pq - start_pq);
  unsigned long long point_query_time = duration_pq.count();
  std::cout << "Time taken to perform point queries from zonemap = " << point_query_time*1.0/kRuns << " microseconds" << endl;
    
  //3. ----------------------------- range queries -----------------------------
  unsigned long long range_query_time = 0;
  // Your code starts here ...
  std::vector< std::pair<int, int> > rqueries;
  loadRangeQueries(kRangeQueriesPath, rqueries);

  auto start_rq = std::chrono::high_resolution_clock::now();
  for(size_t r = 0; r < kRuns; r++) {
    int rtotal = 0;
    for(pair<int, int> rq: rqueries) {
      rtotal += zones.query(rq.first, rq.second).size();
    }
    std::cout << "Range query test #" << r << ": " << rtotal << " total matches." << endl;

    std::random_shuffle(rqueries.begin(), rqueries.end());
  }

  auto stop_rq = std::chrono::high_resolution_clock::now();
  auto duration_rq = std::chrono::duration_cast<std::chrono::microseconds>(stop_rq - start_rq);
  range_query_time = duration_rq.count();

  std::cout << "Time taken to perform range query from zonemap = " << range_query_time*1.0/kRuns << " microseconds" << endl;
  return 0;
}
