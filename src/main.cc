#include <common.hh>
#include <trie.hh>
#include <dict.hh>

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cerr << "usage: " << argv[0]
              << " /path/to/word/freq.txt /path/to/output/dict.bin"
              << std::endl;
    return 1;
  }

  std::string inputDictPath = argv[1];
  std::string outputFilePath = argv[2];

  std::ifstream inputDict(inputDictPath);
  std::cout << "Parsing dict: " <<  inputDictPath << std::endl;
  std::vector<Entry> entries = ParseDict(inputDict);

  for (Entry e: entries)
  {
    std::cout << e.str << std::endl;
  }

  TrieBuilder trie = TrieBuilder(entries);
  trie.ParallelBuild();
  trie.ToGraphViz();

  return 0;
}
