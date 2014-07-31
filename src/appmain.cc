#include <trie.hh>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <search.hh>

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " compiled/dict" << std::endl;
    return 1;
  }
  char* inputDict = argv[1];
  int fd = open(inputDict, O_RDONLY);
  struct stat fileStat;
  fstat(fd, &fileStat);

  void* raw = mmap(nullptr, fileStat.st_size, PROT_READ, MAP_SHARED, fd, 0);
  int strbuffSize = *static_cast<int*>(raw);
  void* strbuff = raw + sizeof(int);
  void* buff = strbuff + strbuffSize;

  Trie trie(strbuff, buff);

  const int sizeMax = 4096;
  char inputLine[sizeMax];
  char *result;

  while((result = fgets(inputLine, sizeMax, stdin )) != NULL)
  {
    char *tok = strtok(inputLine, " ");
    if (strcmp(tok, "approx") == 0){
      std::cout << "[";
      int dist_max = atoi(strtok(NULL, " "));
      StartSearch(trie, strtok(NULL, "\n"), dist_max);
      std::cout << "]" << std::endl;
    }
  }
  if (ferror(stdin))
    perror("Error reading stdin.");
}
