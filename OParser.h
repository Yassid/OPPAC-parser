#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif
#include <string>
#include <cstdint>
#include <cstring>
#include <string>
#include <typeinfo>
#include <unordered_map>
// Unsure which one is preferred (H5Cpp.h or hdf5.h)
// #include "H5Cpp.h"
#include "hdf5.h"
#include "hdf5_hl.h"

#include <vector>
#include <fstream>
#include <cassert>
#include <cmath>
#include <random>
#include <exception>
#include <utility>

//BOOST
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include "boost/multi_array.hpp"
#include <boost/filesystem.hpp>
#include "/usr/include/boost/regex.hpp"

#define cRED "\033[1;31m"
#define cYELLOW "\033[1;33m"
#define cNORMAL "\033[0m"
#define cGREEN "\033[1;32m"

#define MAX_NAME 1024

typedef boost::multi_array<double, 3> double3;

using MapFileType = std::map<std::string,std::pair<double,double>>;

template <typename T>
std::vector<T> make_vector(const T& init, const T& end, const T& step) {
  std::vector<T> v;
  for (T x = init; x < end; x += step)
    {v.push_back(x);}
  return v;
}

template <typename T>
std::vector<T> add_to_vector(
    std::vector<T> v, const T& init, const T& end, const T& step) {
  for (T x = init; x < end; x += step)
    {v.push_back(x);}
  return v;
}



herr_t write_h5(const char* const filename,MapFileType& map,const std::vector<double>& xpos,const std::vector<double>& ypos,const double3& histgrid,const std::vector<int>& diodes,const std::vector<int>& events);

template <class T1, class T2>
herr_t parse_sim_to_h5(MapFileType& map,const char* const hdf_output_filename,std::vector<T1>& xpos,std::vector<T2>& ypos,const std::vector<int>& diodes);


