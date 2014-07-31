#ifndef SEARCH_HH
# define SEARCH_HH

void Search(std::priority_queue& results, Trie trie, char* word, char* temp, int err);
void SearchOk(std::priority_queue& results, Trie trie, char* word, char* temp, int err);
void SearchInsert(std::priority_queue& results, Trie trie, char* word, char* temp, int err);
void SearchRemove(std::priority_queue& results, Trie trie, char* word, char* temp, int err);
void SearchSwap(std::priority_queue& results, Trie trie, char* word, char* temp, int err);

#endif /* !SEARCH_HH */
