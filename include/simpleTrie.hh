#ifndef SIMPLETRIE_HH
# define SIMPLETRIE_HH

struct SimpleTrieNode
{
  SimpleTrieNode();
  ~SimpleTrieNode();
  SimpleTrieNode(const SimpleTrieNode& base);
  SimpleTrieNode& operator=(const SimpleTrieNode& base);

  const std::vector<std::string> GetKeys() const;
  const SimpleTrieNode* GetChild(const std::string& key) const;

  std::map<std::string, SimpleTrieNode*> edges;
  int freq = 0;
  bool isOutNode = false;
};

class SimpleTrieBuilder
{
public:
  SimpleTrieBuilder(const std::vector<Entry>& dict) : _dict(dict) {};
  void Build();
  void Compact();
  void* Serialize(const std::string& outputPath="");
  const SimpleTrieNode& GetRoot() { return _root; };
private:
  std::vector<Entry> _dict;
  SimpleTrieNode _root;
};

#endif /* !SIMPLETRIE_HH */
