#ifndef SIMPLETRIE_HH
# define SIMPLETRIE_HH

/**
 * Structure representing a node in the SimpleTrie.
 *
 */
struct SimpleTrieNode
{
  SimpleTrieNode();
  ~SimpleTrieNode();
  SimpleTrieNode(const SimpleTrieNode& base);
  SimpleTrieNode& operator=(const SimpleTrieNode& base);

  /**
   * Function to get all the keys from the node. In practice, it represents
   * all the edges that come from the current node, and that go to another node.
   *
   * @return Vector containing all the keys
   */
  const std::vector<std::string> GetKeys() const;

  /**
   * Function to get the node reached by following the edges containing the
   * string key.
   *
   * @return SimpleTrieNode child from key
   */
  const SimpleTrieNode* GetChild(const std::string& key) const;

  std::map<std::string, SimpleTrieNode*> edges; /**< map containing all the edges that go from the current node to other node. The keys corresponds to the letters to get to the other node */
  int freq = 0; /**< Frequence of the current node. It is by default at 0, and when we are on a leaf node, the value corresponds to the frequency of the word that can be created if we sum up all the letters we passed to get to that node */
  bool isOutNode = false; /**< Boolean to know whether we are on a lead node or not */
};

/**
 * Naive implementation of our Trie. This Patricia Trie is a tree of maps, each
 * keys represents a letter or letters to get to another node. If we sum up all
 * the letters to go to a leaf node, we get a word.
 *
 */
class SimpleTrieBuilder
{
public:
  SimpleTrieBuilder(const std::vector<Entry>& dict) : _dict(dict) {};
  /**
   * Build creates the SimpleTrieBuilder from the dictionnary. For each entry, we
   * iterate on each letters and create or go through the tree adding edges and
   * node if it doesn't exist yet. A simple Trie is created.
   *
   */
  void Build();
  /**
   *  This function compacts the simple Trie to get a Patricia Trie. It goes
   *  from the root, and check for each node if he has one or more children.
   *  If one node is not a leaf node and has only one son, then they are merged.
   *
   */
  void Compact();
  /**
   * This function create a Trie, which is the implementation of our Patricia
   * Trie in a different way, to fit in less space.
   *
   */
  Trie Serialize(const std::string& outputPath="");
  const SimpleTrieNode& GetRoot() { return _root; };
private:
  std::vector<Entry> _dict; /**< Dictionnary, vector of Entry */
  SimpleTrieNode _root; /**< The root of the Trie, which is a tree of map */
};

#endif /* !SIMPLETRIE_HH */
