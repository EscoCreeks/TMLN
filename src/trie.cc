#include <trie.hh>
#include <bench_tool.hh>

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

void AddTrie(TrieNode& root, const Entry& entry)
{
  std::string word = entry.str;
  TrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    node = &node->edges[std::string(1,word[i])];
  }
  node->isOutNode = true;
}

void ParallelAddTrie(TrieNode& root, const Entry& entry)
{
  static std::mutex mutex;
  std::string word = entry.str;
  TrieNode* node = &root;
  for (int i = 0; i < word.size(); ++i){
    mutex.lock();
    node = &node->edges[std::string(1,word[i])];
    mutex.unlock();
  }
  node->isOutNode = true;
}

void TrieBuilder::Build()
{
  {
    time_guard tg("construction: ");
    TrieNode& root = _root;
    for (const Entry& entry : _dict)
    {
      AddTrie(root, entry);
    }
  }
  {
    time_guard tg("compaction: ");
    Compact();
  }
}

void TrieBuilder::ParallelBuild()
{
  TrieNode& root = _root;
    std::vector<std::thread> threads;
    for (const Entry& entry : _dict)
    {
      threads.push_back(std::thread([=,&root](){
            ParallelAddTrie(root, entry);
            }));
    }
    for (auto& th: threads)
      th.join();
    ParallelCompact();
}

void TrieBuilder::Merge()
{
  NOT_IMPLEMENTED();
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

void ParallelCompactNode(TrieNode& prec, const std::string keyFather, TrieNode& curr)
{
  if (curr.edges.size() == 1 && !curr.isOutNode)
  {
    std::string newKey = keyFather + curr.edges.begin()->first;
    prec.edges[newKey] = curr.edges.begin()->second;
    prec.edges.erase(keyFather);
    ParallelCompactNode(prec, newKey, prec.edges[newKey]);
  }
  else {
    std::vector<std::thread> threads;
    std::vector<std::string> keys;
    for (auto item : curr.edges)
      keys.push_back(item.first);
    for (int i = 0; i < keys.size(); ++i)
    {
      const std::string myKey = keys[i];
      threads.push_back(std::thread([=,&curr](){
            ParallelCompactNode(curr, myKey, curr.edges.at(myKey));
            }));
    }
    for (auto& th : threads)
      th.join();
  }
}

void TrieBuilder::Compact()
{
  std::map<std::string, TrieNode>::iterator it;
  std::vector<std::string> keys;
  for (it = _root.edges.begin(); it != _root.edges.end(); ++it)
    keys.push_back(it->first);

  for (int i = 0; i < keys.size(); ++i)
  {
    CompactNode(_root, keys[i], _root.edges[keys[i]]);
  }
}

void TrieBuilder::ParallelCompact()
{
  std::map<std::string, TrieNode>::iterator it;
  std::vector<std::thread> threads;
  std::vector<std::string> keys;
  for (it = _root.edges.begin(); it != _root.edges.end(); ++it)
    keys.push_back(it->first);

  for (int i = 0; i < keys.size(); ++i)
  {
    const std::string myKey = keys[i];
    threads.push_back(std::thread([=](){
          ParallelCompactNode(_root, myKey, _root.edges[myKey]);
          }));
  }
  for (auto& th : threads)
    th.join();
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

void TrieBuilder::ToGraphViz()
{
  std::ofstream file("builder.dot", std::ios::out | std::ios::trunc);
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
