#include "clothing.h"
#include <iostream>
#include <set>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

using namespace std; 

Clothing::Clothing(const std::string category, const std::string name, 
	double price, int qty,
   	std::string size, std::string brand)
	: Product(category, name, price, qty), size_(size), brand_(brand)
{

};

Clothing::~Clothing(){

};
/**
* Returns the appropriate keywords that this product should be associated with
*/
std::set<std::string> Clothing::keywords() const{
	set<string> set_of_name = parseStringToWords(name_);
	set<string> set_of_size = parseStringToWords(size_);
	set<string> set_of_brand = parseStringToWords(brand_);
	
	set<string> extra_info = setUnion(set_of_size,set_of_brand);
	set<string> final_keywords = setUnion(extra_info, set_of_name);

	return final_keywords; 
}
/**
* Returns a string to display the product info for hits of the search
*/
std::string Clothing::displayString() const{
	/*<name>
	Size: <size> Brand: <brand>
	<price> <quantity> left.
	*/
	string display_string; 
	double average_rating_temp =  abs(average_rating);

	stringstream ss1;
	ss1 << std::fixed << std::showpoint << std::setprecision(2) << price_; 

	stringstream ss;
	ss << std::fixed << std::showpoint << std::setprecision(2) << average_rating_temp; 
	
	display_string = name_ + "\nSize: " + size_ +  " Brand: " + brand_ 
	+ " \n" + ss1.str() + " " +to_string(qty_) + " left."
	 + " \n" + "Rating: " + ss.str();
	return display_string;
}

/**
* Outputs the product info in the database format
*/
void Clothing::dump(std::ostream& os) const{
	os << category_ << "\n" << name_ << "\n" << price_ << "\n" << qty_ 
		<< "\n" << size_ << "\n" << brand_ << "\n"; 
}
