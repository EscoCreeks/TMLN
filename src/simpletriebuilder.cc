#include <trie.hh>

void AddTrie(SimpleTrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  SimpleTrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    SimpleTrieNode*& nnode = node->edges[std::string(1,word[i])];
    if (nnode == nullptr)
      nnode = new SimpleTrieNode();
    node = nnode;
  }
  node->isOutNode = true;
}

void SimpleTrieBuilder::Build()
{
  SimpleTrieNode& root = _root;
  for (const Entry& entry : _dict)
  {
    AddTrie(root, entry);
  }
}

std::pair<int,int> CountTrie(SimpleTrieNode* root)
{
  std::pair<int,int> count = std::make_pair(1, root->edges.size());
  for (std::pair<std::string, SimpleTrieNode*> edge : root->edges)
  {
    std::pair<int,int> res = CountTrie(edge.second);
    count.first += res.first;
    count.second += res.second;
  }
  return count;
}

Trie SimpleTrieBuilder::Serialize()
{
  /*
   * Count number of trie node element
   * Count number of trie node
   */
  std::pair<int,int> count = CountTrie(&_root);
  int sizeToAlloc = sizeof(int)*count.first+sizeof(TrieElement)*count.second;
  std::cout << "(" << count.first << "," << count.second << ")"
            << " allocate " << sizeToAlloc/1024/1024 << "meg" << std::endl;
  void* buff = malloc(sizeToAlloc);
  NOT_IMPLEMENTED();
}

SimpleTrieNode::SimpleTrieNode()
  : edges()
{
}

SimpleTrieNode::SimpleTrieNode(const SimpleTrieNode& base)
{
  std::map<std::string, SimpleTrieNode*> nedges;
  for (auto edge : base.edges)
    nedges[edge.first] = new SimpleTrieNode(*edge.second);
  edges = nedges;
  isOutNode = base.isOutNode;
}

SimpleTrieNode& SimpleTrieNode::operator=(const SimpleTrieNode& base)
{
  std::map<std::string, SimpleTrieNode*> nedges;
  for (auto edge : base.edges)
    nedges[edge.first] = new SimpleTrieNode(*edge.second);
  edges = nedges;
  isOutNode = base.isOutNode;
  return *this;
}

SimpleTrieNode::~SimpleTrieNode()
{
  for (std::pair<std::string, SimpleTrieNode*> edge : edges)
    if (edge.second) delete edge.second;
}

const std::vector<std::string> SimpleTrieNode::GetKeys() const
{
  std::vector<std::string> keys;
  for (auto edge : edges)
    keys.push_back(edge.first);
  return keys;
}

const SimpleTrieNode* SimpleTrieNode::GetChild(const std::string& key) const
{
  decltype(edges)::const_iterator it = edges.find(key);
  if (it == edges.end())
    return nullptr;
  else
    return it->second;
}

void CompactNode(SimpleTrieNode& prec, std::string keyFather, SimpleTrieNode* curr)
{
  if (curr->edges.size() == 1 && !curr->isOutNode){
    SimpleTrieNode* nnode = curr;
    SimpleTrieNode* precnode = &prec;
    std::string newKey = keyFather;
    std::string precKey = keyFather;
    do
    {
      precnode->edges[precKey] = nullptr;
      precnode->edges.erase(precKey);
      precnode = nnode;
      precKey = precnode->edges.begin()->first;
      newKey += nnode->edges.begin()->first;
      nnode = nnode->edges.begin()->second;
    } while (nnode->edges.size() == 1 && !nnode->isOutNode);
    prec.edges[newKey] = nnode;
    CompactNode(prec, newKey, prec.edges[newKey]);
  }
  else {
    std::vector<std::string> keys;
    for (auto& item : curr->edges)
      keys.push_back(item.first);

    for (int i = 0; i < keys.size(); ++i)
      CompactNode(*curr, keys[i], curr->edges[keys[i]]);
  }
}

void SimpleTrieBuilder::Compact()
{
  std::vector<std::string> keys;
  for (auto& item : _root.edges)
    keys.push_back(item.first);

  for (int i = 0; i < keys.size(); ++i)
  {
    CompactNode(_root, keys[i], _root.edges[keys[i]]);
  }
}
