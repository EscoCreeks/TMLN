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

Trie TrieBuilder::Build()
{
  NOT_IMPLEMENTED();
}
Trie TrieBuilder::ParallelBuild()
{
  TrieNode root;

  NOT_IMPLEMENTED();
}
void TrieBuilder::Merge()
{
  NOT_IMPLEMENTED();
}
void TrieBuilder::Compact()
{
  NOT_IMPLEMENTED();
}
