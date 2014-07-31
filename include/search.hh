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

void StartSearch(Trie& trie, char* word, int max_err);

void Search(std::priority_queue<ResultElement>& results,
            TrieElement& trieElt, char* word, char* temp,
            int err, int limit, char** stack);
void SearchOk(std::priority_queue<ResultElement>& results,
              TrieElement& trieElt, char* word, char* temp,
              int err, int limit, char** stack);
void SearchInsert(std::priority_queue<ResultElement>& results,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
void SearchRemove(std::priority_queue<ResultElement>& results,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
void SearchSwap(std::priority_queue<ResultElement>& results,
                TrieElement& trieElt, char* word, char* temp,
                int err, int limit, char** stack);

#endif /* !SEARCH_HH */
