#include <trie.hh>
#include <tbb/parallel_for.h>


LocklessTrieBuilder::LocklessTrieBuilder(const std::vector<Entry>& dict)
  : TrieBuilder(dict)
{
}

void AddTrie(LocklessTrieNode& root, const Entry& entry)
{
  LocklessTrieNode* node = &root;
  for (const char symb : entry.str)
  {
    decltype(node->edges)::accessor acc;
    node->edges.insert(acc, std::string(1, symb));
    if (acc->second == nullptr)
      node = (acc->second = new LocklessTrieNode());
    else
      node = acc->second;
    acc.release();
  }
  node->isOutNode = true;
}

void LocklessTrieBuilder::Build()
{
  tbb::parallel_for(tbb::blocked_range<size_t>(0,_dict.size()),
      [&](const tbb::blocked_range<size_t>& r){
        for (size_t i = r.begin(); i != r.end(); ++i)
          AddTrie(_root, _dict[i]);
      });
}


LocklessTrieNode::LocklessTrieNode()
  : edges()
{
}

LocklessTrieNode::~LocklessTrieNode()
{
  for (auto edge : edges)
    delete edge.second;
}

LocklessTrieNode::LocklessTrieNode(const LocklessTrieNode& base)
{
  tbb::concurrent_hash_map<std::string, LocklessTrieNode*> nedges;
  for (auto edge : base.edges)
  {
    decltype(nedges)::accessor acc;
    nedges.insert(acc, edge.first);
    acc->second = new LocklessTrieNode(*edge.second);
  }
  edges = nedges;
  isOutNode = base.isOutNode;
}

LocklessTrieNode& LocklessTrieNode::operator=(const LocklessTrieNode& base)
{
  tbb::concurrent_hash_map<std::string, LocklessTrieNode*> nedges;
  for (auto edge : base.edges)
  {
    decltype(nedges)::accessor acc;
    nedges.insert(acc, edge.first);
    acc->second = new LocklessTrieNode(*edge.second);
  }
  edges = nedges;
  isOutNode = base.isOutNode;
  return *this;
}

const std::vector<std::string> LocklessTrieNode::GetKeys() const
{
  std::vector<std::string> keys;
  for (auto edge : edges)
  {
    keys.push_back(edge.first);
  }
  return keys;
}

const LocklessTrieNode* LocklessTrieNode::GetChild(const std::string& key) const
{
  decltype(edges)::accessor acc;
  if (edges.find(acc, key))
    return acc->second;
  else
    return nullptr;
}

void ParallelCompactNode(LocklessTrieNode& prec, const std::string keyFather, LocklessTrieNode* curr, bool& toDelete)
{
  static std::mutex mutex;
  if (curr->edges.size() == 1 && !curr->isOutNode)
  {
    LocklessTrieNode* nnode = curr;
    LocklessTrieNode* precnode = &prec;
    std::string newKey = keyFather;
    std::string precKey = keyFather;
    do
    {
      mutex.lock();
      std::cout << "newKey " << newKey << std::endl;
      mutex.unlock();
      decltype(prec.edges)::accessor accessor;
      // Ce test permet de savoir si on arrive d'un appel récursif (toDelete est à faux, et il faut le mettre à true pour indiquer
      // à l'appelant qu'il faudra qu'il delete le noeud avec son argument keyFather en key.
      if (!toDelete)
        toDelete = true;
      // Ici on est passé une fois par la boucle vu que toDelete est à true.
      else {
        mutex.lock();
        std::cout << "trying to access " << precKey << std::endl;
        mutex.unlock();
        // Ici on hang. Je ne sais pas pourquoi. Le code de la boucle n'active pas d'accessor, y a pas de lock sur la hash_map.
        // C'est l'opération find qui hang
        if (precnode->edges.find(accessor,precKey)){
          accessor->second = nullptr;
          precnode->edges.erase(precKey);
          accessor.release();
        }
      }
      precnode = nnode;
      precKey = precnode->edges.begin()->first;
      newKey += nnode->edges.begin()->first;
      nnode = nnode->edges.begin()->second;
    } while (nnode->edges.size() == 1 && !nnode->isOutNode);
    decltype(curr->edges)::accessor accessor;
    if (prec.edges.insert(accessor,newKey)){
      accessor->second = nnode;
      toDelete = true;
      bool toDeleteRec = false;
      ParallelCompactNode(prec, newKey, accessor->second, toDeleteRec);
      if (toDeleteRec)
        prec.edges.erase(accessor);
    }
  }
  else {
    std::vector<std::string> keys;
    for (auto item : curr->edges)
      keys.push_back(item.first);
    tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
        [=,&curr](const tbb::blocked_range<size_t>& r){
        for (size_t i = r.begin(); i != r.end(); ++i)
        {
          bool toDeleteRec = false;
          decltype(curr->edges)::accessor accessor;
          if(curr->edges.find(accessor, keys[i])){
            ParallelCompactNode(*curr, keys[i], accessor->second, toDeleteRec);
            if (toDeleteRec)
              curr->edges.erase(accessor);
          }
        }
        });
  }
}

void LocklessTrieBuilder::Compact()
{
  std::vector<std::string> keys;
  for (auto& item : _root.edges)
    keys.push_back(item.first);

  LocklessTrieNode& root = _root;
  tbb::parallel_for(tbb::blocked_range<size_t>(0,keys.size()),
      [=,&root](const tbb::blocked_range<size_t>& r){
      for (size_t i = r.begin(); i != r.end(); ++i)
      {
        decltype(_root.edges)::accessor accessor;
        if(root.edges.find(accessor, keys[i])){
          bool toDelete = false;
          ParallelCompactNode(root, keys[i], accessor->second, toDelete);
          if (toDelete)
            root.edges.erase(accessor);
        }
      }
      });
}

