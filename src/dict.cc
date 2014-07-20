#include <dict.hh>

std::vector<Entry> ParseDict(std::ifstream& input)
{
  std::vector<Entry> entries;

  while(true)
  {
    Entry e;
    input >> e.str >> e.freq;
    if (input.good())
      entries.push_back(e);
    else
      break;
  }
  return entries;
}
