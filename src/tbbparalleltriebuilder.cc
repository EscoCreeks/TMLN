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
    //Compact();
  }
}

void TbbParallelTrieBuilder::Compact()
{
  NOT_IMPLEMENTED();
}
