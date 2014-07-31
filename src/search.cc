#include <trie.hh>
#include <search.hh>

bool ResultElement::operator<(const ResultElement& res) const
{
  return dist < res.dist && freq > res.freq;
}

void Search(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* buff, int err, int limit)
{
  if (*buff == '\0')
  {
    TrieElement* trieElt = trie.GetElements();
    for (int i = 0; i < trie.GetElementCount(); ++i)
    {
      buff = trieElt[i].GetStr();
      SearchOk(results, trieElt->GetTrie(), word, buff, err, limit);
      SearchInsert(results, trieElt->GetTrie(), word, buff, err, limit);
      SearchRemove(results, trieElt->GetTrie(), word, buff, err, limit);
      SearchSwap(results, trieElt->GetTrie(), word, buff, err, limit);
    }
  }
  else
  {
    SearchOk(results, trie, word, buff, err, limit);
    SearchInsert(results, trie, word, buff, err, limit);
    SearchRemove(results, trie, word, buff, err, limit);
    SearchSwap(results, trie, word, buff, err, limit);
  }
}

void SearchOk(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* buff, int err, int limit)
{
  if (*buff != '\0' && *buff == *word)
    Search(results, trie, ++word, ++buff, err, limit);
}

void SearchInsert(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* buff, int err, int limit)
{
}

void SearchRemove(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* buff, int err, int limit)
{
}

void SearchSwap(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* buff, int err, int limit)
{
}

void PrintResults(std::priority_queue<ResultElement>& results)
{
  while(!results.empty())
  {
    const ResultElement& elt = results.top();
    std::cout << "{\"word\":\"" << elt.str
              << "\",\"freq\":" << elt.freq
              << ",\"distance\":" << elt.dist;
    results.pop();
  }
}
