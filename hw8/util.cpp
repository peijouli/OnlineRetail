#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <set> 
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{
    //CONVERT EVERYTHING INTO LOWER CASE
    rawWords = convToLower(rawWords);
    int count = 0; 
    string word = "";
    std::set<string> set_of_words;
   // std::set<int>::iterator it;

    for (int i = 0; i < (int) rawWords.length(); i++)
    {
            if ( isalpha(rawWords[i]) ){
                word+=rawWords[i];
                if (!ispunct(rawWords[i+1])){
                        while(!isspace(rawWords[i+1]) && isalpha(rawWords[i+1])){
                        word+= rawWords[i+1];
                        count ++; 
                        i++;
                    }
                }
                else {
                    continue; 
                }
                if (count >= 1){
                    set_of_words.insert(word);
                }
                word ="";
                count = 0;

            }
            else if (ispunct(rawWords[i])){
               // if (isalpha(rawWords[i+1]) ){
                    while (!(isalpha(rawWords[i+1]) || i == (int)rawWords.length()-1)){
                        count++; 
                        word += rawWords[i+1];
                        i++; 
                    }
                    if ( count >=2){
                        set_of_words.insert(word);
                      
                    }
                    word = "";
                    count = 0 ; 
                   

             //   }
                // else {
                //     continue;
                // }
                

            }
            else if (isspace(rawWords[i])){
                continue;
            } 

    }
   //  for (std::set<string>::iterator it=set_of_words.begin(); it!=set_of_words.end(); it++)
   //  std::cout << *it << " " ;

   // std::cout << '\n';

    return set_of_words; 


}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}


// int main(){
   
//     //string input = "men'lll"; 
//     string input = "data structure's i hate this"; 
//     parseStringToWords(input);

//     return 1; 

// }