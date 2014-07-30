#include <trie.hh>

TrieElement::TrieElement(int strId, int freq)
  : _strId(strId), _freq(freq)
{
}

void TrieElement::SetTrieOffset(int off)
{
  _trieOffset = off;
}

Trie TrieElement::GetTrie()
{
  assert(_trieOffset > 0 && "if this fail, you tried to get a trie node from a leaf");
  return Trie(reinterpret_cast<char*>(this) + _trieOffset);
}

int TrieElement::GetFreq()
{
  return _freq;
}

bool TrieElement::IsOutNode()
{
  return _freq != 0;
}

Trie::Trie(std::string serializedPath)
{
  NOT_IMPLEMENTED();
}

Trie::Trie(void* raw)
  : _raw(raw)
{
}

int Trie::GetElementCount()
{
  return static_cast<pNode*>(_raw)->count;
}

TrieElement* Trie::GetElements()
{
  return static_cast<TrieElement*>(_raw+sizeof(pNode::count));
}

char* Trie::_strBase_ = nullptr;

Trie::Trie(void* strBase, void* raw)
{
  _strBase_ = static_cast<char*>(strBase);
  _raw = raw;
}

TrieBuilder::TrieBuilder(const std::vector<Entry>& dict)
  : _dict(dict)
{
}

void TrieBuilder::Merge()
{
  NOT_IMPLEMENTED();
}

void TrieBuilder::Compact()
{
  // TO OVERRIDE
}

void TrieBuilder::Build()
{
  // TO OVERRIDE
}

void NodeToGraphViz(std::ofstream& os, std::string prec, const TrieNode& node, int nbPrec, int& nbCurr)
{
  if (node.isOutNode){
    os << ++nbCurr << " [shape=doublecircle]" << std::endl;
    os << nbPrec << " -> " << nbCurr << " [label=" << prec << "]" << std::endl;
  }
  else {
    os << nbPrec << " -> " << ++nbCurr << " [label=" << prec << "]" << std::endl;
  }
    int n = nbCurr;
  for (auto item : node.edges){
    NodeToGraphViz(os, item.first, item.second, n, ++nbCurr);
  }
}

void TrieBuilder::ToGraphViz(std::string fileName)
{
  std::ofstream file(fileName, std::ios::out | std::ios::trunc);
  if (file.is_open()){
    file << "digraph triebuilder {" << std::endl;
    int nb = 0;
    for (auto item : _root.edges){
      NodeToGraphViz(file, item.first, item.second, 0, ++nb);
    }
    file << "}";
  }
  else
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}

const TrieNode& TrieBuilder::GetRoot()
{
  return _root;
}

std::vector<std::string> TrieNode::GetKeys()
{
  std::vector<std::string> rtn;
  for (auto edge : edges)
    rtn.push_back(edge.first);
  return rtn;
}

const TrieNode* TrieNode::GetChild(std::string symb)
{
  decltype(edges)::iterator it = edges.find(symb);
  if (it == edges.end())
    return nullptr;
  else
    return &it->second;
}

const TbbTrieNode& TbbParallelTrieBuilder::GetRoot()
{
  return _tbbRoot;
}

std::vector<std::string> TbbTrieNode::GetKeys()
{
  std::vector<std::string> rtn;
  for (auto edge : edges)
    rtn.push_back(edge.first);
  return rtn;
}

const TbbTrieNode* TbbTrieNode::GetChild(std::string symb)
{
  decltype(edges)::accessor accessor;
  if (edges.find(accessor, symb))
    return &accessor->second;
  else
    return nullptr;
}

bool CheckDistance(char* querry, char* test, int* error)
{
  for(; *querry == *test; ++querry, ++test)
  {
    if (*test == 0)
      return true;
    if (*querry != *test)
      (*error)--;
    if (*error < 0)
      return false;
  }
  while (*test)
  {
    (*error)--;
    test++;
  }
  return (*error) >= 0;
}
