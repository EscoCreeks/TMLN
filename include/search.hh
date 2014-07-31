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

template<typename T1>
void StartSearch(Trie& trie, char* word, int max_err);

template<typename T1>
void Search(T1& results, Trie trie,
            TrieElement& trieElt,  char* word, char* temp,
            int err, int limit, char** stack);
template<typename T1>
void SearchOk(T1& results, Trie trie,
              TrieElement& trieElt, char* word, char* temp,
              int err, int limit, char** stack);
template<typename T1>
void SearchInsert(T1& results, Trie trie,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
template<typename T1>
void SearchRemove(T1& results, Trie trie,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
template<typename T1>
void SearchSubstitute(T1& results, Trie trie,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
template<typename T1>
void SearchSwap(T1& results, Trie trie,
                TrieElement& trieElt, char* word, char* temp,
                int err, int limit, char** stack);

#endif /* !SEARCH_HH */
