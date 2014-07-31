#include <trie.hh>

TrieElement::TrieElement(int strId, int freq)
  : _strId(strId), _freq(freq), _trieOffset(0)
{
}

void TrieElement::SetTrieOffset(int off)
{
  assert(off >= 0);
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
char* TrieElement::GetStr()
{
  return Trie::_strBase_ + _strId;
}

bool TrieElement::IsLeaf()
{
  return _trieOffset == 0;
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
