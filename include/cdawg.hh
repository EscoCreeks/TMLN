#ifndef CDAWG_HH
# define CDAWG_HH

class CDAWG
{
public:
  CDAWG(std::vector<Entry> dictEntry); // construct
  CDAWG(std::string serializedCDAWGPath); // load
  void Save(std::ostream out);
};

#endif /* !CDAWG_HH */
