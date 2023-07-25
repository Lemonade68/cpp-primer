#ifndef TEST_TEXTQUERY_H
#define TEST_TEXTQUERY_H

#include <memory>
using std::shared_ptr;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <set>
using std::set;

#include <fstream>
using std::ifstream;

#include "QueryResult.h"

/* this version of the query classes includes two
 * members not covered in the book:
 *      cleanup_str: which removes punctuation and
 *                   convert all text to lowercase
 *      display_map: a debugging routine that will print the contents
 *                   of the lookup map
 */

class QueryResult;  // declaration needed for return type in the query function
class TextQuery {
public:
	typedef vector<string>::size_type line_no;
	TextQuery(ifstream&);
	QueryResult query(const string&) const;
	void display_map();     // debugging aid: print the map
private:
	shared_ptr<vector<string>> file;     // input file
	// maps each word to the set of the lines in which that word appears
	map<string, shared_ptr<set<line_no>>> wm;
	// canonicalizes text: removes punctuation and makes everything lower case
	static string cleanup_str(const string&);
};

#endif //TEST_TEXTQUERY_H
