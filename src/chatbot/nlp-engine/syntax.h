#ifndef LVK_NLP_SYNTAX_H
#define LVK_NLP_SYNTAX_H

#define STAR_OP             "*"
#define PLUS_OP             "+"
#define VAR_DECL_REGEX      "\\[(\\w+)\\]"
#define RECURSION_REGEX     "[rR]" VAR_DECL_REGEX
#define PREDICATE_REGEX     "([^}]*)(==|!=|>=|<=|<|>)([^}]*)"
#define IF_REGEX            "\\{\\s*if\\s+" PREDICATE_REGEX "\\s*\\}([^{]*)"
#define ELSE_REGEX          "\\{\\s*else\\s*\\}(.+)"

#endif // LVK_NLP_SYNTAX_H
