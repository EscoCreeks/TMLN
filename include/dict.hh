#ifndef DICT_HH
# define DICT_HH

#include <common.hh>

struct Entry
{
  std::string str;
  int freq;
};

std::vector<Entry> ParseDict(std::ifstream& filePath);

#endif /* !DICT_HH */
