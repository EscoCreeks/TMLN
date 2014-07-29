#include <common.hh>
#include <dict.hh>
#include <trie.hh>

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

  std::ifstream input(inputDictPath);
  std::vector<Entry> dict = ParseDict(input);
  input.close();

  SimpleTrieBuilder tb(dict);
  tb.Build();
  tb.Compact();
  Trie trie = tb.Serialize();
  std::ofstream output(outputFilePath);
  trie.Save(output);
  output.close();

  return 0;
}
