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
