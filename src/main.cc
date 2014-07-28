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

  std::cout << std::endl << "***** Simple Build ****" << std::endl;
  {
    SimpleTrieBuilder trie = SimpleTrieBuilder(entries);
    {
      time_guard tg ("simple build: ");
      trie.Build();
    }
    {
      time_guard tg ("simple compact: ");
      trie.Compact();
    }
  }
  std::cout << std::endl << "***** Locked Build ****" << std::endl;
  {
    LockedTrieBuilder trie(entries);
    {
      time_guard tg ("locked build: ");
      trie.Build();
    }
    {
      time_guard tg ("locked compact: ");
      trie.Compact();
    }
  }
  std::cout << std::endl << "***** Tbb pointer Build ****" << std::endl;
  {
    LocklessTrieBuilder trie = LocklessTrieBuilder(entries);
    {
      time_guard tg ("tbb pointer build: ");
      trie.Build();
    }
    {
      time_guard tg ("tbb pointer compact: ");
      trie.Compact();
    }
  }
  std::cout << std::endl << "***** Tbb copy Build ****" << std::endl;
  {
    TbbParallelTrieBuilder trie = TbbParallelTrieBuilder(entries);
    {
      time_guard tg ("tbb copy build: ");
      trie.Build();
    }
    {
      time_guard tg ("tbb copy compact: ");
      trie.Compact();
    }
  }

  return 0;
}
