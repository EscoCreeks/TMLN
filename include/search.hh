#ifndef SEARCH_HH
# define SEARCH_HH

struct ResultElement
{
  ResultElement(char* str_, int freq_, int dist_)
    : str(str_), freq(freq_), dist(dist_)
  {}
  bool operator<(const ResultElement& res) const;
  char* str;
  int freq;
  char dist;
};

void Search(std::priority_queue<ResultElement>& results,
            Trie trie, char* word, char* temp, int err, int limit);
void SearchOk(std::priority_queue<ResultElement>& results,
              Trie trie, char* word, char* temp, int err, int limit);
void SearchInsert(std::priority_queue<ResultElement>& results,
                  Trie trie, char* word, char* temp, int err, int limit);
void SearchRemove(std::priority_queue<ResultElement>& results,
                  Trie trie, char* word, char* temp, int err, int limit);
void SearchSwap(std::priority_queue<ResultElement>& results,
                Trie trie, char* word, char* temp, int err, int limit);

#endif /* !SEARCH_HH */
