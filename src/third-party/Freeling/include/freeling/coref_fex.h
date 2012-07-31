//////////////////////////////////////////////////////////////////
//    Class for the feature extractor.
//////////////////////////////////////////////////////////////////

#ifndef CORE_FEX_H
#define CORE_FEX_H

#include <list>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#include "fries.h"
#include "freeling/semdb.h"

//////////////////////////////////////////////////////////////////
///    Struct that contains one sample
//////////////////////////////////////////////////////////////////

class SAMPLE {
 public:
	int sent;
	int numde;
	int posbegin;
	int posend;
	node *node1;
	std::string text;
	std::vector<std::string> texttok;
	std::vector<std::string> tags;
};

//////////////////////////////////////////////////////////////////
///    Struct of an example (contains two samples)
//////////////////////////////////////////////////////////////////

class EXAMPLE {
 public:
	int sent;
	int numde;
	SAMPLE sample1;
	SAMPLE sample2;
};

#define COREFEX_TYPE_TWO	0
#define COREFEX_TYPE_THREE	1

#define COREFEX_DIST		0x00000001
#define COREFEX_IPRON		0x00000002
#define COREFEX_JPRON		0x00000004
#define COREFEX_IPRONM		0x00000008
#define COREFEX_JPRONM		0x00000010
#define COREFEX_STRMATCH	0x00000020
#define COREFEX_DEFNP		0x00000040
#define COREFEX_DEMNP		0x00000080
#define COREFEX_NUMBER		0x00000100
#define COREFEX_GENDER		0x00000200
#define COREFEX_SEMCLASS	0x00000400
#define COREFEX_PROPNAME	0x00000800
#define COREFEX_ALIAS		0x00001000
#define COREFEX_APPOS		0x00002000

//////////////////////////////////////////////////////////////////
///    Class for the feature extractor.
//////////////////////////////////////////////////////////////////

class coref_fex{
	private:
		// semantic database to check for semantic properties
		semanticDB * semdb;
		// active features
		int vectors;

		int jump(const std::vector<std::string> &);
		int get_dist(const EXAMPLE &);
		int get_numdedist(const EXAMPLE &);
		int get_dedist(const EXAMPLE &);
		int get_i_pronoum(const EXAMPLE &);
		int get_j_pronoum(const EXAMPLE &);
		int get_i_pronoum_p(const EXAMPLE &);
		int get_j_pronoum_p(const EXAMPLE &);
		int get_i_pronoum_d(const EXAMPLE &);
		int get_j_pronoum_d(const EXAMPLE &);
		int get_i_pronoum_x(const EXAMPLE &);
		int get_j_pronoum_x(const EXAMPLE &);
		int get_i_pronoum_i(const EXAMPLE &);
		int get_j_pronoum_i(const EXAMPLE &);
		int get_i_pronoum_t(const EXAMPLE &);
		int get_j_pronoum_t(const EXAMPLE &);
		int get_i_pronoum_r(const EXAMPLE &);
		int get_j_pronoum_r(const EXAMPLE &);
		int get_i_pronoum_e(const EXAMPLE &);
		int get_j_pronoum_e(const EXAMPLE &);
		int get_str_match(const EXAMPLE &);
		int get_def_np(const EXAMPLE &);
		int get_dem_np(const EXAMPLE &);
		int get_number(const EXAMPLE &);
		int get_semclass(const EXAMPLE &);
		int get_gender(const EXAMPLE &);
// 		int get_proper_name(const EXAMPLE &);
		int get_proper_name_i(const EXAMPLE &);
		int get_proper_name_j(const EXAMPLE &);
		bool check_word(string, string);
		int check_acronim(const EXAMPLE &);
		int check_fixesleft(const EXAMPLE &);
		int check_fixesright(const EXAMPLE &);
		int check_order(const EXAMPLE &);
		int get_alias_acro(const EXAMPLE &);
		int get_alias_fixleft(const EXAMPLE &);
		int get_alias_fixright(const EXAMPLE &);
		int get_alias_order(const EXAMPLE &);
		int get_appositive(const EXAMPLE &);
		int get_i_inquotes(const EXAMPLE &);
		int get_j_inquotes(const EXAMPLE &);
		int get_i_inparenthesis(const EXAMPLE &);
		int get_j_inparenthesis(const EXAMPLE &);
		int get_i_thirtperson(const EXAMPLE &);
		int get_j_thirtperson(const EXAMPLE &);

		void put_feature(int, std::vector<int> &);
	public:
		int typeVector;

		coref_fex(const int, const int, const std::string&, const std::string&);
		coref_fex();
		~coref_fex();
		void setVectors(int);
		std::vector<std::string> tokenize(const std::string &, const std::string &);
		void extract(EXAMPLE &, std::vector<int> &);
};
#endif
