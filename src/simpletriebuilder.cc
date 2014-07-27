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

void CompactNode(TrieNode& prec, std::string keyFather, TrieNode& curr)
{
  if (curr.edges.size() == 1 && !curr.isOutNode)
  {
    std::string newKey = keyFather + curr.edges.begin()->first;
    prec.edges[newKey] = curr.edges.begin()->second;
    prec.edges.erase(keyFather);
    CompactNode(prec, newKey, prec.edges[newKey]);
  }
  else {
    std::map<std::string, TrieNode>::iterator it;
    std::vector<std::string> keys;
    for (it = curr.edges.begin(); it != curr.edges.end(); ++it)
      keys.push_back(it->first);

    for (int i = 0; i < keys.size(); ++i)
      CompactNode(curr, keys[i], curr.edges[keys[i]]);
  }
}

SimpleTrieNode::~SimpleTrieNode()
{
  for (std::pair<std::string, SimpleTrieNode*> edge : edges)
    delete edge.second;
}

void SimpleTrieBuilder::Compact()
{
  NOT_IMPLEMENTED();
  // std::map<std::string, TrieNode>::iterator it;
  // std::vector<std::string> keys;
  // for (it = _root.edges.begin(); it != _root.edges.end(); ++it)
  //   keys.push_back(it->first);

  // for (int i = 0; i < keys.size(); ++i)
  // {
  //   CompactNode(_root, keys[i], _root.edges[keys[i]]);
  // }
}
