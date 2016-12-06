/**
 * Michał Kuźba, Bartosz Bruski, Jakub Kuklis
 *
 * Our assumption is that correct years are in (1000,2999) range.
 * We consider "Stamp 1000 1000 1000" to be a correct stamp,
 * with value=1000, year=1000 and post="1000".
 */

#include <iostream>
#include <regex>
#include <tuple>
#include <vector>
#include <algorithm>
#include <utility>

// using namespace std;

using std::string;
using std::pair;
using std::make_pair;
using std::tuple;
using std::make_tuple;
using std::get;
using std::regex;
using std::regex_match;
using std::regex_search;
using std::regex_replace;
using std::smatch;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::getline;
using std::vector;

/**
 * Stamp is represented as a tuple of
 * (year, post, value, name, parsed information for printing).
 */
typedef tuple<int, string, long double, string, string> stamp;

typedef pair<int, int> paII;
typedef pair<bool, stamp> paBT;

/**
 * Constant variables used in function parse()
 * to denote an incorrect line.
 */
const stamp error_stamp = make_tuple(0, "", 0, "", "");
const paBT error_pair = make_pair(false, error_stamp);

/**
 * Replace multiple space with one.
 */
string clear_empty_space(string text) {
  regex re("\\s+");
  return regex_replace(text, re, " ");
}

/**
 * Check if the given string contains a correct range of years.
 */
bool is_range(string line) {
  regex re("(\\s*)([0-9]{4})(\\s+)([0-9]{4})(\\s*)");

  smatch m;

  bool match = regex_match(line, m, re);

  if (!match) {
    return false;
  }

  int left, right;

  left = stoi(m[2]);
  right = stoi(m[4]);

  if (left > right)
    return false;

  return true;
}

bool is_year(int year_cand) {
  return (1000 <= year_cand && year_cand < 3000);
}

/**
 * Parse a line with information about stamps.
 * Returns a pair - (correctnes of the input, stamp).
 */
paBT parse(string line) {
  bool found = false;
  string name, value, year, post;
  int year_num;
  long double value_num;

  // Regexes to match consecutively: release year, prefix ending
  // before the year and suffix starting after the year.
  smatch year_sm;
  regex year_re("(\\s+)([0-9]{4})");

  smatch pref_sm;
  regex pref_re("(\\s*)(.+?)(\\s+)([0-9]+[.,][0-9]+|[0-9]+)");

  smatch suff_sm;
  regex suff_re("(\\s+)(.+\\S)(\\s*)");

  string str = line;

  while (regex_search(str, year_sm, year_re)) {

    int year_cand = stoi(year_sm[0]);
    bool correct_year = is_year(year_cand);

    // Prefix of line that ends before the year candidate
    int curr_pref_length = year_sm.prefix().str().size();
    string pref = line.substr(0, line.size() - str.size() + curr_pref_length);
    bool correct_pref = regex_match(pref, pref_sm, pref_re);

    string suff = year_sm.suffix().str();
    bool correct_suff = regex_match(suff, suff_sm, suff_re);

    if (correct_year && correct_pref && correct_suff) {
      found = true;
      name = clear_empty_space(pref_sm[2]);
      value = pref_sm[4];
      year = year_sm[2];
      post = clear_empty_space(suff_sm[2]);
      year_num = year_cand;
    }

    str = year_sm.suffix().str();
  }

  if (found) {
    string parsed_info = year + " " + post + " " + value + " " + name;

    // Normalization of notation
    regex comma_dot(",");
    value = regex_replace(value, comma_dot, ".");

    value_num = stold(value);

    stamp res = make_tuple(year_num, post, value_num, name, parsed_info);

    return make_pair(true, res);
  }
  return error_pair;
}

/**
 * Returns the year interval. Function assumes the line includes a correct
 * range.
 */
paII get_interval(string line) {
  smatch year;

  regex re("[0-9]{4}");

  regex_search(line, year, re);
  int from = stoi(year[0]);

  string rest = year.suffix().str();

  regex_search(rest, year, re);
  int to = stoi(year[0]);

  return make_pair(from, to);
}

/**
 * Find all stamps from the given time interval.
 */
void query(vector<stamp> &stamps, paII interval) {

  stamp from = make_tuple(interval.first, "", 0., "", "");

  // First stamp out of the range, 'to', has year set
  // to first year following the end of interval.
  stamp to = make_tuple(interval.second + 1, "", 0., "", "");

  auto it_from = lower_bound(stamps.begin(), stamps.end(), from);
  auto it_to = lower_bound(stamps.begin(), stamps.end(), to);

  // Tuple's 4th index contains the whole name of the stamp.
  for_each(it_from, it_to, [](stamp &p) { cout << get<4>(p) << endl; });
}

/**
 * Primary function. Contains program's essential logic.
 */
void process_input() {
  int line_number = 1;
  bool queries_mode = false;

  vector<stamp> stamps;

  string line;

  while (getline(cin, line)) {

    if (queries_mode == false && is_range(line)) {
      // Stamps are sorted once - before answering queries.
      sort(stamps.begin(), stamps.end());
      queries_mode = true;
    }

    if (!queries_mode) {
      if (parse(line).first == false) {
        cerr << "Error in line " << line_number << ":" << line << endl;
      } else {
        stamps.push_back(parse(line).second);
      }
    } else {
      if (is_range(line)) {
        paII interval = get_interval(line);
        query(stamps, interval);
      } else {
        cerr << "Error in line " << line_number << ":" << line << endl;
      }
    }

    line_number++;
  }
}

int main() {

  process_input();

  return 0;
}
