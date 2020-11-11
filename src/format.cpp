#include "format.h"
#include <iomanip>
#include <iostream>
#include <string>

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) {
  long h = seconds / (60 * 60);
  long s_remainder = seconds % (60 * 60);
  long m = s_remainder / 60;
  long s_final = s_remainder % 60;

  string h_str = string(2 - to_string(h).length(), '0').append(to_string(h));
  string m_str = string(2 - to_string(m).length(), '0').append(to_string(m));
  string s_str =
      string(2 - to_string(s_final).length(), '0').append(to_string(s_final));

  std::cout << h_str << ":" << m_str << ":" << s_str << "\n";

  return h_str + ":" + m_str + ":" + s_str;
}
