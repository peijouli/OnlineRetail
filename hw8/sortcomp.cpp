#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <locale>
#include <stdio.h>
#include <ctype.h>
#include <stdio.h> 
#include <stdlib.h> 

using namespace std;


struct Comp2 {
  bool operator()(std::string st1, std::string st2)
  {
    return st1 > st2;
  }
};

/* case insentive: use tolower()*/
struct Comp3 {
  bool operator()(std::string st1, std::string st2)
  {
    std::locale loc;
    int letter_value1 = 0;
    int letter_value2 = 0;
    string new1 = "";
    string new2 = ""; 
    for ( unsigned int i = 0; i < st1.length(); i++){
      new1 +=  tolower(st1[i], loc);
    }
    for ( unsigned int i = 0; i <  st2.length(); i++){
      new2 +=  tolower(st2[i], loc);
    }
    return new1 < new2;

  }
    
  
};
struct Comp4 {
  bool operator()(std::string st1, std::string st2)
  /// create a new a and new b = push into the new string if hit a character 
  // only compare the new string 
  {
    std::size_t s1 = st1.find_first_not_of(" ");
    
    std::size_t s2 = st2.find_first_not_of(" ");

    return st1.substr(s1) < st2.substr(s2);
  }
    
  
};

void outputWords(std::ostream& ostr, const std::vector<std::string>& words)
{
  for(unsigned i = 0; i < words.size()-1; i++){
    ostr << words[i] << ":";
  }
  if(words.size() > 0){
    ostr << words.back();
  }
  ostr << endl;
}

int main(int argc, char* argv[])
{
  if(argc < 2){
    cout << "Provide an output file name." << endl;
    return 1;
  }
  ofstream ofile(argv[1]);
  if(ofile.fail()) {
    cout << "Cannot open file for writing." << endl;
    return 1;
  }
  vector<string> mywords =
    { "abc", "Acc", "aBc", "Zxy", "zyx", "  efg" };

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z).  This should still be case sensitive.
  vector<string> order1 = mywords;
  std::sort(order1.begin(), order1.end());
  outputWords(ofile, order1);

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order but descending from largest to smallest
  // (i.e. z to a).  This should still be case sensitive
  vector<string> order2 = mywords;
  Comp2 c2;
  std::sort(order2.begin(), order2.end(), c2);
  outputWords(ofile, order2);


  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z) but case INSENSITIVE (i.e. abc < Abd)
  vector<string> order3 = mywords;
  Comp3 c3;
  std::sort(order3.begin(), order3.end(), c3);
  outputWords(ofile, order3);

  // Sort mywords in normal alphabetic/lexicographic string
  // comparison order.  Ascending from smallest to largest
  // (i.e. a to z), case sensitive and ignoring
  // spaces (only ' ') before the word.
  vector<string> order4 = mywords;
  Comp4 c4;
  std::sort(order4.begin(), order4.end(), c4);
  outputWords(ofile, order4);

  ofile.close();
  return 0;
}

