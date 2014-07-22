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

void PAddTrie(TrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  TrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i)
    node = &node->edges[std::string(1,word[i])];
}

void TrieBuilder::Build()
{
  TrieNode& root = _root;
  for (const Entry& entry : _dict)
  {
    PAddTrie(root, entry);
  }
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

void NodeToGraphViz(std::ofstream& os, std::string prec, const TrieNode& node, std::map<std::string, int>& occ)
{
  if (occ.count(prec) == 0)
    occ[prec] = 0;

  std::string father = "\"" + prec + "." + std::to_string(occ[prec]) + "\"";
  for (auto item : node.edges){
    if (occ.count(item.first) > 0)
      ++occ[item.first];
    else
      occ[item.first] = 0;
    os << father << " -> " << "\"" + item.first + "." + std::to_string(occ[item.first]) + "\"" << std::endl;
    NodeToGraphViz(os, item.first, item.second, occ);
  }
}

void TrieBuilder::ToGraphViz()
{
  std::ofstream file("builder.dot", std::ios::out | std::ios::trunc);
  if (file.is_open()){
    file << "digraph triebuilder {" << std::endl;
    std::map<std::string, int> occ;
    for (auto item : _root.edges){
      NodeToGraphViz(file, item.first, item.second, occ);
    }
    file << "}";
  }
  else
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}
