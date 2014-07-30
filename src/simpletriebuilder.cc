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
  if (root->edges.size() == 0)
    return std::make_pair(0,1);
  std::pair<int,int> count = std::make_pair(root->edges.size(), 0);
  for (std::pair<std::string, SimpleTrieNode*> edge : root->edges)
  {
    std::pair<int,int> res = CountTrie(edge.second);
    count.first += res.first;
    count.second += res.second;
  }
  return count;
}

struct pNode {
  int count;
  TrieElement trieElt[];
};

int ToAlloc(SimpleTrieNode* root)
{
  std::pair<int,int> res = CountTrie(root);
  return sizeof(pNode::count)*(1+res.first-res.second) +
    sizeof(TrieElement)*(res.first);
}

void* TrieWriter(void* buff, SimpleTrieNode* root)
{
  if (root->edges.size() == 0)
    return buff;
  // one layer
  pNode* node = static_cast<pNode*>(buff);
  node->count = root->edges.size();
  int i = 0;
  for (auto edge : root->edges)
  {
    TrieElement* elt = new (&node->trieElt[i]) TrieElement();
    elt->SetStrId(0x42);
    ++i;
  }
  // split
  void* buffEnd = &node->trieElt[i];
  i = 0;
  for (auto edge : root->edges)
  {
    int offset = static_cast<char*>(buffEnd) - reinterpret_cast<char*>(&node->trieElt[i]);
    node->trieElt[i].SetTrieOffset(offset);
    buffEnd = TrieWriter(buff, edge.second);
  }
  return buffEnd;
}

void* SimpleTrieBuilder::Serialize(const std::string& outputPath)
{
  std::pair<int,int> count = CountTrie(&_root);
  std::cout << "elt count " << count.first
            << " leaf count " << count.second
            << " to alloc " << ToAlloc(&_root) << std::endl;
  int buffSize = ToAlloc(&_root);
  void* buff = malloc(buffSize);
  std::cout << "diff " << static_cast<char*>(buff) - static_cast<char*>(TrieWriter(buff, &_root)) << std::endl;
  if (outputPath != "")
  {
    std::ofstream output(outputPath, std::ios::out | std::ios::binary);
    output.write(static_cast<char*>(buff), buffSize);
    output.close();
  }
  return buff;
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
