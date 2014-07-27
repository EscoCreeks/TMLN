#include <trie.hh>
#include <tbb/parallel_for.h>

typedef tbb::concurrent_hash_map<std::string, TbbTrieNode> trieNodeMap;

void AddTrie(TbbTrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  TbbTrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    trieNodeMap::accessor accessor;
    node->edges.insert(accessor, std::string(1,word[i]));
    node = &accessor->second;
  }
  node->isOutNode = true;
}

void TbbParallelTrieBuilder::Build()
{
  {
    time_guard tg("tbb construction: ");
    TbbTrieNode& root = _tbbRoot;
    tbb::parallel_for(tbb::blocked_range<size_t>(0,_dict.size()),
        [=,&root](const tbb::blocked_range<size_t>& r){
          for (size_t i = r.begin(); i != r.end(); ++i)
            AddTrie(root, _dict[i]);
        });
  }
  {
    time_guard tg("tbb compaction: ");
    Compact();
  }
}

void ParallelCompactNode(TbbTrieNode& prec, const std::string keyFather, TbbTrieNode& curr, bool& toDelete)
{
  if (curr.edges.size() == 1 && !curr.isOutNode)
  {
    std::string newKey = keyFather + curr.edges.begin()->first;
    {
      trieNodeMap::accessor accessor;
      if (prec.edges.insert(accessor, newKey)){
        accessor->second = curr.edges.begin()->second;
      }
    }

    toDelete = true;
    trieNodeMap::accessor accessor;
    if (prec.edges.find(accessor, newKey)){
      bool toDeleteRec = false;
      ParallelCompactNode(prec, newKey, accessor->second, toDeleteRec);
      if (toDeleteRec)
        prec.edges.erase(accessor);
    }
  }
  else {
    std::vector<std::thread> threads;
    std::vector<std::string> keys;
    for (auto item : curr.edges)
      keys.push_back(item.first);
    bool toDeleteRec = false;
    tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
        [=,&curr, &toDeleteRec](const tbb::blocked_range<size_t>& r){
        for (size_t i = r.begin(); i != r.end(); ++i)
        {
          trieNodeMap::accessor accessor;
          if(curr.edges.find(accessor, keys[i])){
            ParallelCompactNode(curr, keys[i], accessor->second, toDeleteRec);
            if (toDeleteRec)
              curr.edges.erase(accessor);
          }
        }
        });
  }
}

void TbbParallelTrieBuilder::Compact()
{
  std::vector<std::string> keys;
  for (auto& item : _tbbRoot.edges)
    keys.push_back(item.first);

  TbbTrieNode& root = _tbbRoot;
  tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
      [=,&root](const tbb::blocked_range<size_t>& r){
      for (size_t i = r.begin(); i != r.end(); ++i)
      {
        trieNodeMap::accessor accessor;
        if(root.edges.find(accessor, keys[i])){
          bool toDelete = false;
          ParallelCompactNode(root, keys[i], accessor->second, toDelete);
          if (toDelete)
            root.edges.erase(accessor);
        }
      }
      });
}

void NodeToGraphViz(std::ofstream& os, std::string prec, const TbbTrieNode& node, int nbPrec, int& nbCurr)
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

void TbbParallelTrieBuilder::TbbToGraphViz(std::string fileName)
{
  std::ofstream file(fileName, std::ios::out | std::ios::trunc);
  if (file.is_open()){
    file << "digraph triebuilder {" << std::endl;
    int nb = 0;
    for (auto item : _tbbRoot.edges){
      NodeToGraphViz(file, item.first, item.second, 0, ++nb);
    }
    file << "}";
  }
  else
    std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}
