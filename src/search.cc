#include <trie.hh>
#include <search.hh>

inline bool ResultElement::operator<(const ResultElement& res) const
{
  if (dist > res.dist)
    return true;
  if (dist == res.dist)
  {
    if (freq < res.freq)
      return true;
    if (freq == res.freq)
      return (strcmp(str,res.str) > 0);
  }
  return false;
}

inline bool ResultElementVectorized::operator<(const ResultElementVectorized& res) const
{
  if (dist > res.dist)
    return true;
  if (dist == res.dist)
  {
    if (freq < res.freq)
      return true;
    if (freq == res.freq)
      return (strcmp(str,res.str) > 0);
  }
  return false;
}

template <typename T1>
void AddResult(T1& results, char** stack, TrieElement& trieElt, int err)
{
  std::cerr << "Add not specialized" << std::endl;
}

template <>
void AddResult<std::priority_queue<ResultElement>>(std::priority_queue<ResultElement>& results, char** stack, TrieElement& trieElt, int err)
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
  str[0] = '\0';
  while (count)
  {
    ++stack;
    strcat(str, *stack);
    --count;
  }
#ifndef NDEBUG
  std::cerr << "found pq " << str << std::endl;
#endif
  results.emplace(str, trieElt.GetFreq(), err);
}

template <>
void AddResult<std::vector<std::priority_queue<ResultElementVectorized>>>(std::vector<std::priority_queue<ResultElementVectorized>>& results, char** stack, TrieElement& trieElt, int err)
{
  if (results.size() == 0)
    results = std::vector<std::priority_queue<ResultElementVectorized>>(3, std::priority_queue<ResultElementVectorized>());
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
  str[0] = '\0';
  while (count)
  {
    ++stack;
    strcat(str, *stack);
    --count;
  }
#ifndef NDEBUG
  std::cerr << "found vpq" << str << std::endl;
#endif
  results[err].emplace(str, trieElt.GetFreq(), err);
}

template <>
void AddResult<std::vector<ResultElement>>(std::vector<ResultElement>& results, char** stack, TrieElement& trieElt, int err)
{
  if (results.size() == 0)
    results = std::vector<ResultElement>();
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
  str[0] = '\0';
  while (count)
  {
    ++stack;
    strcat(str, *stack);
    --count;
  }
#ifndef NDEBUG
  std::cerr << "found v" << str << std::endl;
#endif
  results.emplace_back(str, trieElt.GetFreq(), err);
}

template <typename T1>
void PrintResults(T1& results)
{
  std::cout << "Print not specialized" << std::endl;
}

template<>
void PrintResults<std::priority_queue<ResultElement>>(std::priority_queue<ResultElement>& results)
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
    if (!results.empty())
      std::cout << ",";
  }
}

template<>
void PrintResults<std::vector<std::priority_queue<ResultElementVectorized>>>(std::vector<std::priority_queue<ResultElementVectorized>>& results)
{
  if (results.size() == 0)
    return;
  for (int i = 0; i < 3; ++i)
  {
    while(!results[i].empty())
    {
      const ResultElementVectorized& elt = results[i].top();
      std::cout << "{\"word\":\"" << elt.str
        << "\",\"freq\":" << elt.freq
        << ",\"distance\":" << static_cast<int>(elt.dist)
        << "}";
      delete[] elt.str;
      results[i].pop();
    }
  }
}

bool CompareResultElement(const ResultElement& left, const ResultElement& right)
{
  if (left.dist > right.dist)
    return true;
  if (left.dist == right.dist)
  {
    if (left.freq < right.freq)
      return true;
    if (left.freq == right.freq)
      return (strcmp(left.str,right.str) > 0);
  }
  return false;
}

template<>
void PrintResults<std::vector<ResultElement>>(std::vector<ResultElement>& results)
{
  if (results.size() == 0)
    return;
  std::sort(results.begin(), results.end(), CompareResultElement);
  for (auto& elt : results)
  {
    std::cout << "{\"word\":\"" << elt.str
      << "\",\"freq\":" << elt.freq
      << ",\"distance\":" << static_cast<int>(elt.dist)
      << "}";
    delete[] elt.str;
  }
}

template<typename T1>
void StartSearch(Trie& trie, char* word, int max_err)
{
  char** stack = new char*[1024];
  stack[0] = nullptr;
  //std::priority_queue<ResultElement> results;
  T1 results;
  for (int i = 0; i < trie.GetElementCount(); ++i)
  {
    stack[1] = trie.GetElements()[i].GetStr();
    Search(results, trie, trie.GetElements()[i], word, stack[1], 0, max_err, stack+1);
  }
  PrintResults(results);
  delete[] stack;
}

template<typename T1>
void Search(T1& results, Trie trie, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (err > limit)
    return;
  if (*buff == '\0' && *word == '\0' && trieElt.IsOutNode())
    AddResult(results, stack, trieElt, err);
  if (*buff == '\0' && !trieElt.IsLeaf())
  {
    trie = trieElt.GetTrie();
    TrieElement* trieElts = trie.GetElements();
    for (int i = 0; i < trie.GetElementCount(); ++i)
    {
      buff = trieElts[i].GetStr();
      ++stack;
      *stack = buff;
      Search(results, trie, trieElts[i], word, buff, err, limit, stack);
      --stack;
    }
  }
  else
  {
    SearchOk(results, trie, trieElt, word, buff, err, limit, stack);
    SearchInsert(results, trie, trieElt, word, buff, err, limit, stack);
    SearchRemove(results, trie, trieElt, word, buff, err, limit, stack);
    SearchSwap(results, trie, trieElt, word, buff, err, limit, stack);
    SearchSubstitute(results, trie, trieElt, word, buff, err, limit, stack);
  }
}

template<typename T1>
void SearchOk(T1& results, Trie trie, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (*word != '\0' && *buff != '\0' && *buff == *word)
    Search(results, trie, trieElt, word+1, buff+1, err, limit, stack);
}

template<typename T1>
void SearchInsert(T1& results, Trie trie, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (*buff != '\0' && *word != *buff)
    Search(results, trie, trieElt, word, buff+1, err+1, limit, stack);
}

template<typename T1>
void SearchRemove(T1& results, Trie trie, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (*word != '\0' && *buff != '\0' && *word != *buff)
    Search(results, trie, trieElt, word+1, buff, err+1, limit, stack);
}

template<typename T1>
void SearchSubstitute(T1& results, Trie trie, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (*word != '\0' && *buff != '\0' && *word != *buff) // not ok
    Search(results, trie, trieElt, word+1, buff+1, err+1, limit, stack);
}

template<typename T1>
void SearchSwap(T1& results, Trie trie, TrieElement& trieElt, char* word, char* buff, int err, int limit, char** stack)
{
  if (*word != '\0' && *(word+1) != '\0')
  {
    std::swap(*word, *(word+1));
    Search(results, trie, trieElt, word, buff, err+1, limit, stack);
    std::swap(*word, *(word+1));
  }
}
