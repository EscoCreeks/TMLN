#ifndef SEARCH_HH
# define SEARCH_HH

/**
 * This structure represents a result from the querry. It contains
 * the word found, its frequence and the distance from the word searched.
 *
 */

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

struct ResultElementVectorized
{
  ResultElementVectorized(char* str_, int freq_, int dist_)
    : str(str_), freq(freq_), dist(dist_)
  {}
  bool operator<(const ResultElementVectorized& res) const;
  char* str;
  int freq;
  char dist;
};

/**
 * The start of the search. It will call all the others search to propagate the
 * search throughough all the Trie, respecting the max_err.
 *
 */
template<typename T1>
void StartSearch(Trie& trie, char* word, int max_err);

template<typename T1>
void Search(T1& results, Trie trie,
            TrieElement& trieElt,  char* word, char* temp,
            int err, int limit, char** stack);
/**
 * Check if the two letters are identical.
 *
 */
template<typename T1>
void SearchOk(T1& results, Trie trie,
              TrieElement& trieElt, char* word, char* temp,
              int err, int limit, char** stack);
/**
 * Check if the word got a letter more.
 *
 */
template<typename T1>
void SearchInsert(T1& results, Trie trie,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
/**
 * Check if the worde got a letter less.
 *
 */
template<typename T1>
void SearchRemove(T1& results, Trie trie,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
/**
 * Check if the word has a letter substitued with another.
 *
 */
template<typename T1>
void SearchSubstitute(T1& results, Trie trie,
                  TrieElement& trieElt, char* word, char* temp,
                  int err, int limit, char** stack);
/**
 * Check if the word has a letter in the place of another.
 *
 */
template<typename T1>
void SearchSwap(T1& results, Trie trie,
                TrieElement& trieElt, char* word, char* temp,
                int err, int limit, char** stack);

#endif /* !SEARCH_HH */
