#include <trie.hh>

Trie::Trie(std::string serializedPath)
{
  NOT_IMPLEMENTED();
}

void Trie::Save(std::ostream out)
{
  NOT_IMPLEMENTED();
}

TrieBuilder::TrieBuilder(const std::vector<Entry>& dict)
  : _dict(dict)
{
}

void TrieBuilder::Build()
{
  NOT_IMPLEMENTED();
}

void PAddTrie(TrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  TrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i)
    node = &node->edges[std::string(1,word[i])];
}

void TrieBuilder::ParallelBuild()
{
  TrieNode& root = _root;
  for (const Entry& entry : _dict)
  {
    PAddTrie(root, entry);
  }
}

void TrieBuilder::Merge()
{
  NOT_IMPLEMENTED();
}
void TrieBuilder::Compact()
{
  NOT_IMPLEMENTED();
}
