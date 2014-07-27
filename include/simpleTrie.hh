#ifndef SIMPLETRIE_HH
# define SIMPLETRIE_HH

struct SimpleTrieNode
{
  ~SimpleTrieNode();

  std::map<std::string, SimpleTrieNode*> edges;
  bool isOutNode = false;
};

class SimpleTrieBuilder
{
public:
  SimpleTrieBuilder(const std::vector<Entry>& dict) : _dict(dict) {};
  void Build();
  void Compact();
private:
  std::vector<Entry> _dict;
  SimpleTrieNode _root;
};

#endif /* !SIMPLETRIE_HH */
