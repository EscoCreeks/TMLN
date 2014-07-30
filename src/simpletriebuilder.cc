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

int CountTrie(SimpleTrieNode* root)
{
  int count = root->edges.size();
  for (std::pair<std::string, SimpleTrieNode*> edge : root->edges)
    count += CountTrie(edge.second);
  return count;
}

struct pNode {
  int count;
  TrieElement* trieElt;
};
void* TrieWriter(void* buff, SimpleTrieNode* root)
{
  // pNode* base = static_cast<pNode*>(buff);
  // base->count = root->edges.size();
  if (root->edges.size() == 0)
    return buff;

  for (std::pair<std::string, SimpleTrieNode*> edge : root->edges)
  {
    buff = TrieWriter(buff, edge.second);
    SimpleTrieNode* it = static_cast<SimpleTrieNode*>(buff) - 1;
    TrieElement* elt = new (it) TrieElement();
    elt->SetStrId(42);
    elt->SetTrieOffset(43);
  }
  buff -= sizeof(pNode::count);
  *static_cast<int*>(buff) = 40;
  return buff;
}

Trie SimpleTrieBuilder::Serialize()
{
  /*
   * Count number of trie node element
   * Count number of trie node
   */
  int count = CountTrie(&_root);
  int sizeToAlloc = (sizeof(int)+sizeof(TrieElement))*count;
  std::cout << "(" << count << ")"
            << " allocate " << sizeToAlloc/1024/1024 << "meg" << std::endl;
  void* buff = malloc(sizeToAlloc);
  std::cout << "malloc \t" << buff << std::endl;
  std::cout << "malloc end \t" << buff + sizeToAlloc << std::endl;
  std::cout << "rtn buff" << TrieWriter(buff + sizeToAlloc, &_root) << std::endl;
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
