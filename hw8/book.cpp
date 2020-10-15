#include "book.h"
#include <iostream>
#include <set>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

using namespace std; 

Book::Book(const std::string category, const std::string name, 
	double price, int qty,
   	std::string ISBN, std::string author)
	: Product(category, name, price, qty), isbn_(ISBN), author_(author)
{

};

//destructor 
Book::~Book(){

};
/**
* Returns the appropriate keywords that this product should be associated with
*/
std::set<std::string> Book::keywords() const{
	set<string> set_of_name = parseStringToWords(name_);
	set<string> set_of_author = parseStringToWords(author_);
	set<string> set_of_isbn;
	set_of_isbn.insert(isbn_);
	set<string> book_info = setUnion(set_of_author,set_of_isbn);
	set<string> final_keywords = setUnion(book_info, set_of_name);

	return final_keywords; 
}
/**
* Returns a string to display the product info for hits of the search
*/
std::string Book::displayString() const{
	/*<name>
	Author: <author> ISBN: <isbn>
	<price> <quantity> left.
	*/
	double average_rating_temp =  abs(average_rating);
	string display_string; 
	stringstream ss1;
	ss1 << std::fixed << std::showpoint << std::setprecision(2) << price_; 

	stringstream ss;
	ss << std::fixed << std::showpoint << std::setprecision(2) << average_rating_temp; 

	display_string = name_ + "\nAuthor: " + author_ +  " ISBN: " + isbn_ 
	+ " \n" + ss1.str() +" " + to_string(qty_) + " left." 
	+ "\n" + "Rating: " + ss.str() ;
	return display_string;
}

/**
* Outputs the product info in the database format
*/
void Book::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ 
		<< "\n" << isbn_ << "\n" << author_ << "\n"; 
}


