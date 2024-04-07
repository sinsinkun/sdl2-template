#include <string>
#include <iomanip>
#include "util.h"

using namespace Util;

/// @brief convert float to string
/// @param n 
/// @return string form of float with 2 decimal points
std::string Util::floatToString(float n) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2) << n;
  std::string str = ss.str();
  return str;
};

/// @brief convert float to string
/// @param n 
/// @param precision 
/// @return string form of float with specific precision
std::string Util::floatToString(float n, int precision) {
  // handle precision exceptions
  int p = 8;
  if (precision < p) p = precision;
  else if (precision < 0) p = 0;

  std::stringstream ss;
  ss << std::fixed << std::setprecision(precision) << n;
  std::string str = ss.str();
  return str;
};
