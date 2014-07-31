#ifndef DICT_HH
# define DICT_HH

#include <common.hh>

/**
 * An entry in the dictionnary of words. This struct is used to stock all the
 * words from the file given to TextMiningCompiler. From the vector of Entry,
 * a TrieBuilder is then created.
 */
struct Entry
{
  std::string str; /**< Entry word. */
  int freq; /**< Entry frequence. */
};

/**
 * Parse the file and create a vector of Entry.
 * @return The vector of entries of the file
 */
std::vector<Entry> ParseDict(std::ifstream& filePath);

#endif /* !DICT_HH */
