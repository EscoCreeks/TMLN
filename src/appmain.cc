#include <trie.hh>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

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

}
