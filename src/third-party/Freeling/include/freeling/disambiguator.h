#ifndef _DISAMBIG
#define _DISAMBIG

#include <string>
#include <list>

#include "fries/language.h"
#include "freeling/semdb.h"

class disambiguator {
   public:
      /// Constructor
      disambiguator(const std::string &, const std::string &, double, int);
      /// Destructor
      ~disambiguator();
      /// word sense disambiguation for each word in given sentences
      void analyze(std::list<sentence> &) const;
      /// Return annotated copy (useful for perl/python/java APIs)
      std::list<sentence> analyze(const std::list<sentence> &) const;
      
   private:
      ///removal of trailing -a, -n, -v, or -r from synset code
      std::string convert_synset(const std::string &) const;
};

#endif



