#include "movie.h"
#include <iostream>
#include <set>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

using namespace std; 

Movie::Movie(const std::string category, const std::string name, double price, int qty,
   	std::string genre, std::string rating)
	: Product(category, name, price, qty), genre_(genre), rating_(rating)
{

};

Movie::~Movie(){

};
/**
* Returns the appropriate keywords that this product should be associated with
*/
std::set<std::string> Movie::keywords() const{
	set<string> set_of_name = parseStringToWords(name_);
	set<string> set_of_genre = parseStringToWords(genre_);
	set<string> set_of_rating = parseStringToWords(rating_);
	
	set<string> extra_info = setUnion(set_of_genre,set_of_rating);
	set<string> final_keywords = setUnion(extra_info, set_of_name);

	return final_keywords; 
}
/**
* Returns a string to display the product info for hits of the search
*/
std::string Movie::displayString() const{
	/*<name>
	Genre: <genre> Rating: <rating>
	<price> <quantity> left.
	*/
	string display_string;
	double average_rating_temp =  abs(average_rating);

	
	stringstream ss1;
	ss1 << std::fixed << std::showpoint << std::setprecision(2) << price_; 

	stringstream ss;
	ss << std::fixed << std::showpoint << std::setprecision(2) << average_rating_temp; 
	
	display_string = name_ + "\nGenre: " + genre_ +  " Rating: " + rating_ 
	+ " \n" + ss1.str() + " " + to_string(qty_) + " left."
	+  " \n" + "Rating: " + ss.str() ;
	return display_string;
}

/**
* Outputs the product info in the database format
*/
void Movie::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ 
		<< "\n" << genre_ << "\n" << rating_ << "\n"; 
}
