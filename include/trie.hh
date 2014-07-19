#ifndef TRIE_HH
# define TRIE_HH

#include <common.hh>
#include <dict.hh>

class Trie
{
public:
  Trie(std::vector<Entry> dictEntry);
  Trie(std::string serializedPath);

  void Merge();
  void Compact();
  void Save(std::ostream out);
};

#endif /* !TRIE_HH */
