#include <trie.hh>
#include <search.hh>

bool ResultElement::operator<(const ResultElement& res) const
{
  return dist < res.dist && freq > res.freq;
}

void AddResult(std::priority_queue<ResultElement>& results, char** stack, TrieElement& trieElt, int err)
{
  static const int usualSize = 15;
  int count = 0;
  int buffsize = 0;
  char* str = 0;

  while (*stack != nullptr)
  {
    // if (count % usualSize == 0)
    //   str = static_cast<char*>(realloc(str, count + usualSize));
    ++count;
    buffsize += strlen(*stack);
    --stack;
  }
  str = new char[buffsize + 1];
  while (count)
  {
    ++stack;
    strcat(str, *stack);
    --count;
  }
  results.emplace(str, trieElt.GetFreq(), err);
}

void PrintResults(std::priority_queue<ResultElement>& results)
{
  while(!results.empty())
  {
    const ResultElement& elt = results.top();
    std::cout << "{\"word\":\"" << elt.str
              << "\",\"freq\":" << elt.freq
              << ",\"distance\":" << static_cast<int>(elt.dist)
              << "}";
    delete[] elt.str;
    results.pop();
  }
}

void StartSearch(Trie& trie, char* word, int max_err)
{
  char* buff = "";
  char** stack = new char*[1024];
  stack[0] = nullptr;
  std::priority_queue<ResultElement> results;
  for (int i = 0; i < trie.GetElementCount(); ++i)
  {
    stack[1] = trie.GetElements()[i].GetStr();
    Search(results, trie.GetElements()[i], word, stack[1], 0, max_err, stack+1);
  }
  PrintResults(results);
  delete[] stack;
}

void Search(std::priority_queue<ResultElement>& results, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (*buff == '\0')
  {
    Trie trie = trieElt.GetTrie();
    TrieElement* trieElts = trie.GetElements();
    for (int i = 0; i < trie.GetElementCount(); ++i)
    {
      buff = trieElts[i].GetStr();
      ++stack;
      *stack = buff;
      SearchOk(results, trieElts[i], word, buff, err, limit, stack);
      SearchInsert(results, trieElts[i], word, buff, err, limit, stack);
      SearchRemove(results, trieElts[i], word, buff, err, limit, stack);
      SearchSwap(results, trieElts[i], word, buff, err, limit, stack);
      --stack;
    }
    if (trieElt.IsLeaf())
        return;
  }
  else
  {
    SearchOk(results, trieElt, word, buff, err, limit, stack);
    SearchInsert(results, trieElt, word, buff, err, limit, stack);
    SearchRemove(results, trieElt, word, buff, err, limit, stack);
    SearchSwap(results, trieElt, word, buff, err, limit, stack);
  }
}

void SearchOk(std::priority_queue<ResultElement>& results, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (*buff != '\0' && *buff == *word)
    Search(results, trieElt, ++word, ++buff, err, limit, stack);
  if (*buff == '\0' && *word == '\0' && trieElt.IsOutNode())
    AddResult(results, stack, trieElt, err);
}

void SearchInsert(std::priority_queue<ResultElement>& results, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
}

void SearchRemove(std::priority_queue<ResultElement>& results, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
}

void SearchSwap(std::priority_queue<ResultElement>& results, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
}
