#ifndef SEARCH_HH
# define SEARCH_HH

struct ResultElement
{
  ResultElement(char* str_, int freq_, int dist_)
    : str(str_), freq(freq_), dist(dist_)
  {}

  char* str;
  int freq;
  char dist;
};

void Search(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* temp, int err);
void SearchOk(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* temp, int err);
void SearchInsert(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* temp, int err);
void SearchRemove(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* temp, int err);
void SearchSwap(std::priority_queue<ResultElement>& results, Trie trie, char* word, char* temp, int err);

#endif /* !SEARCH_HH */
