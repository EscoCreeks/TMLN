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

SimpleTrieNode::~SimpleTrieNode()
{
  for (std::pair<std::string, SimpleTrieNode*> edge : edges)
    if (edge.second) delete edge.second;
}

const std::vector<std::string> SimpleTrieNode::GetKeys()
{
  std::vector<std::string> keys;
  for (auto edge : edges)
    keys.push_back(edge.first);
  return keys;
}

const SimpleTrieNode* SimpleTrieNode::GetChild(const std::string& key)
{
  decltype(edges)::iterator it = edges.find(key);
  if (it == edges.end())
    return nullptr;
  else
    return it->second;
}

void CompactNode(SimpleTrieNode& prec, std::string keyFather, SimpleTrieNode* curr)
{
  std::cout << "entering with : " << keyFather << std::endl;
  if (curr->edges.size() == 1 && !curr->isOutNode){
    SimpleTrieNode* nnode = curr;
    SimpleTrieNode* precnode = &prec;
    std::string newKey = keyFather;
    std::string precKey = keyFather;
    do
    {
      precnode->edges.erase(precKey);
      precKey = precnode->edges.begin()->first;
      std::cout << "precKey " << precKey << std::endl;
      newKey += nnode->edges.begin()->first;
      std::cout << "newkey " << newKey << std::endl;
      precnode = nnode;
      nnode = nnode->edges.begin()->second;
    } while (nnode->edges.size() == 1 && !nnode->isOutNode);
    std::cout << "final : " << newKey << " with last one : " << precKey << std::endl;
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
