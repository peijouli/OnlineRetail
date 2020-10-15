#include "mydatastore.h"
#include <iostream>
#include <set>
#include <map>
#include <vector> 
#include <iterator>  
#include <algorithm>    
#include "product.h"
#include "heap.h"


using namespace std;
	
MyDataStore::MyDataStore(){

}
MyDataStore::~MyDataStore(){
	for (vector<Product*>::iterator it = v_products.begin(); it != v_products.end(); ++it) {
		delete *it; 
	}
	// std::map<std::string, User*> v_users;
	map<string, User*>::iterator it1 = v_users.begin();
	for (; it1!=v_users.end(); ++it1){
		delete it1->second;
	}

}
/**
 * Adds a product to the data store
 */
void MyDataStore::addProduct(Product* p){
	v_products.push_back(p);
	// create a set of keywords 
	set<string> my_keywords = p->keywords();
	set<Product*> set_product; 
	set_product.insert(p);

	/* check if keyword already exist and then insert into the map 
	If a product is added to a cart twice, treat them as separate items 
	and store them in your cart twice
	(i.e. don't try to store it once with a "quantity" of 2). */

	for (std::set<string>::iterator it=my_keywords.begin(); it!=my_keywords.end(); ++it){
		// case one: cannot find
		if ( map_product.find(*it) == map_product.end()){
		
			map_product.insert(make_pair(*it,set_product));
		}
		// case two: (insert into exisiting product set)
		else {
			map_product.find(*it)->second.insert(p);
		}
	}
}

/**
 * Adds a user to the data store
 */
void MyDataStore::addUser(User* u) {
	// exist 
	v_users.insert(make_pair(u->getName(), u));

}

/**
 * Performs a search of products whose keywords match the given "terms"
 *  type 0 = AND search (intersection of results for each term) while
 *  type 1 = OR search (union of results for each term)
 Your search must be implemented "efficiently". You should not have to iterate over 
 ALL products to find the appropriate matches. 
 Some kind of mapping between keywords and products that match 
 that keyword should be implemented.

 */
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
	std::vector<Product*> v_hits;
	if (terms.size()== 0) {
		return v_hits; 
	}
	// check if term is inside map
	//create a new set
	set<Product*> new_set; 
	// iterate my terms -- 

	for ( int i = 0; i < (int) terms.size(); i ++){
		if ( map_product.find(terms[i])!= map_product.end()){
			// if type 1 = intersection 
    		if (type == 0 ){
    			if (new_set.size()== 0){
    				new_set = map_product[terms[i]]; 
    			}
    			new_set = setIntersection(map_product[terms[i]], new_set); 
    		}
    		else if (type == 1){
    			new_set = setUnion(map_product[terms[i]], new_set); 
    		}
		}
	}
	// convert to vector 
	std::vector<Product*> v_hits2(new_set.begin(), new_set.end());
	return v_hits2; 
}

/**
 * Reproduce the database file from the current Products and User values
 */
void MyDataStore::dump(std::ostream& ofile) {
	//output into the ofile 
	std::map<string,User*>::iterator it;
	std::map<string, vector<Review*>>::iterator it1;
	ofile << "<products>" << endl;
	for(unsigned int i=0; i<v_products.size(); i++) {
		v_products[i]->dump(ofile);
	}
	ofile << "</products>" << endl;
	ofile << "<users>" << endl;
	for( it = v_users.begin();it!=v_users.end();++it)
	{
		(it->second)->dump(ofile); //User*
	}
	ofile << "</users>" << endl;

	ofile << "<reviews>" << endl;
	for( it1 = p_reviews.begin();it1!=p_reviews.end();++it1)
	{

			for (int i = 0; (unsigned int) i < (it1->second.size()); i ++){
				(it1->second[i])->dump(ofile);
			}

	}
	ofile << "</reviews>" << endl;
	return; 

}

// function that will be used in amazon.cpp 
void MyDataStore::add_cart(string username, Product* p){
	if ( v_users.find(username) != v_users.end()){
		if (cart.find(username) != cart.end()){ // if user has a cart alrdy
			cart.find(username)->second.push_back(p);
			//user_products.push_back(p);
			//.push_back(p); 
		}
		else { // user doesnt hv a cart
			// create a vector of product - push back product in it 
			// make pair of a username and product vector 
			std::vector<Product*> temp_product;
			temp_product.push_back(p);
			cart.insert(make_pair(username,temp_product)); 

		}
	}
	else {
		cout << "Invalid username" << endl; 
	}
}
void MyDataStore::buy_cart(string username){
	// change qty and price 
	// check if the username has a cart 
	if ( v_users.find(username) != v_users.end()){
		if (cart.find(username)!=cart.end()){ // if user has a cart alrdy
			//std::vector<Product*> cart_product =  cart.find(username)->second;
			for ( int i = 0 ; i < (int)cart.find(username)->second.size(); i ++ ){
				double product_price = cart.find(username)->second[i]->getPrice();
				if (cart.find(username)->second[i]->getQty()!=0 && // item in stock 
				    v_users.find(username)->second->getBalance() - product_price > 0){
					cart.find(username)->second[i]->subtractQty(1); // decrement the qty of stock
					std::vector<Product*>::iterator it = cart.find(username)->second.begin();
					// changing user's balance
					v_users.find(username)->second->deductAmount(product_price);  
					cart.find(username)->second.erase(it+i);
					i--;					
				}
				else {
					continue; 
				}
			}
		}
	}
	else {
		cout << "Invalid username" << endl ;
	}

}


/*print the products in username's cart at the current time. 
The items should be printed with some ascending index number 
so it is easy to tell how many items are in the cart. 
If the username provided is invalid,
print Invalid username to the screen and do not process the command.*/

void MyDataStore::view_cart(string username){
	if ( v_users.find(username) == v_users.end()){ // user does not exist!!!
		cout << "Invalid username" << endl;
	}
	else if (cart.find(username)!=cart.end()){		
		for (int i = 0; i < (int)cart.find(username)->second.size(); i ++){
			cout << "Item " << i+1 << endl;
			cout << cart.find(username)->second[i]->displayString() << endl; 
		}

	}
	else {
		cout << "The cart is empty" << endl; 
	}
	
}

double MyDataStore::getAverage(Product* p){
	int num = 0;
	std::map<string,vector<Review*>>::iterator it;
	it = p_reviews.find(p->getName());
	if(it != p_reviews.end()){
		vector<Review*> temp = it->second;
		num = temp.size();
	} 
	double sum = 0; 
	for ( int i  = 0 ; i < num; i ++){
		sum += p_reviews.find(p->getName())->second[i]->rating;
	}
	if ( num == 0){
		return 0;
	}
	else {
		p->setAverage(sum/num) ; 
		return sum/num; 
	}

	
}
// NEW PART FOR HW 5 (Problem 3)
void MyDataStore::addReview(const std::string& prodName,int rating,
               const std::string& username,
               const std::string& date,
               const std::string& review_text){
// check if that product exist
	Review* r = new Review(prodName, rating, username, date, review_text);
	if ( p_reviews.find(prodName) != p_reviews.end()){ 
		//push back a vector of review 	
		vector<Review*> new_vec = getReview(prodName);
		new_vec.push_back(r);
		p_reviews.at(prodName) = new_vec;
	}
	else {
		//map_product.insert(make_pair(*it,set_product));
		std::vector<Review*> review_vector; 
		review_vector.push_back(r);
		p_reviews.insert(make_pair(prodName, review_vector)); 
	}
	for (int i=0; (unsigned int) i < v_products.size(); i++) {
		getAverage(v_products[i]);
	}
	for ( int i = 0 ; i <(int) v_products.size(); i ++){
		if ( v_products[i]->getName() == prodName){
			product_review_map[v_products[i]].push_back(r);
		}
	}
}

// so that product has direct access to the review
vector<Review*> MyDataStore::getReview (std::string product_name){
	if ( p_reviews.find(product_name) != p_reviews.end()){
		return p_reviews.find(product_name)->second; 
	}
	std::vector<Review*> vec;
	return vec;
}


/* Problem 5 - SORTING 
 r: sorted based on review scores (high to low)
 n: sorted by name (std:string comparison) */ 
//n: sorted by names
vector<Product*> MyDataStore::alphaSort(vector<Product*>& p) 
{
	vector<string> product_name_string;
	for(unsigned int i=0; i<p.size();i++)
	{
		product_name_string.push_back(p[i]->getName());
	}
	AlphaStrComp comp_alpha;
	vector<Product*> new_product_output;
	mergeSort(product_name_string, comp_alpha);
	for(unsigned int j=0;j<product_name_string.size();j++)
	{
		for(unsigned int k=0; k<p.size();++k)
		{
			if(p[k]->getName()==product_name_string[j])
			{
				new_product_output.push_back(p[k]);
				break;
			}
		}
	}
	return new_product_output;


}


vector<Product*> MyDataStore::ratingSort(vector<Product*>& p) 
{
	vector<double> rating;
	for(unsigned int i=0; i<p.size();i++)
	{
		rating.push_back(p[i]->getAverage());
	}
	DigitComp comp_rating;
	vector<Product*> new_product_output;
	mergeSort(rating, comp_rating);
	for(unsigned int j=0;j<rating.size();j++)
	{	
		//std::cout << to_string(rating[j]) << std::endl;
		for(unsigned int k=0; k<p.size();++k)
		{
			if(p[k]->getAverage()==rating[j])
			{
				new_product_output.push_back(p[k]);
				break;
			}
		}
	}
	return new_product_output;
}
std::vector<Review*> MyDataStore::dateSort(std::vector<Review*>& r) {
	vector<string> date_temp;
	vector<Review*> temp = r;
	for(unsigned int i=0; i<r.size();++i)
	{
		date_temp.push_back(r[i]->date);
	}
	DateComp date_comp;
	mergeSort(date_temp, date_comp);
	vector<Review*> new_output;
	for(unsigned int j=0;j<date_temp.size();j++)
	{
		for(unsigned int k=0; k<temp.size();++k)
		{
			if(temp[k]->date==date_temp[j])
			{
				new_output.push_back(temp[k]);
				temp.erase(temp.begin()+k);
				break;
			}
		}
	}
	return new_output;
}

void MyDataStore::viewReview(vector<Review*>& r)
{
	std::vector<Review*> v_output;
	v_output = dateSort(r); 
	for ( int i = 0; (unsigned int) i < v_output.size(); i++){
		cout << v_output[i]->date << " " << v_output[i]->rating
		<< " " << v_output[i]->username << " "
		<< v_output[i]->reviewText << endl;
	}
}

bool MyDataStore::get_login(){
	return login; 
}
void MyDataStore::set_login(bool l){
	login = l;
}

string MyDataStore::get_curr_username(){
	return curr_username;

}
void MyDataStore::set_curr_username(string username){
	curr_username = username; 
} 

bool MyDataStore::user_exist(string username){
	if ( v_users.find(username) != v_users.end()){
		return true;
	}
	else {
		return false; 
	}

}
unsigned long long MyDataStore::hash_function(std::string password){
	unsigned long long hashed = 0;
	int temp;
	unsigned long long exponent = 1;
	for(int i=1;i<= (int)password.length();i++)
	{
		exponent = 1;
		temp = (int)password[i-1];
		for(int k=0;k< (int)password.length()-i;++k)
		{
			exponent *= 128;
		}
		hashed += exponent*temp;
	}
	unsigned int digits[4];
	for(int i=3;i>=0;--i)
	{
		digits[i] = hashed%65521;
		hashed = hashed/65521;
	}
	hashed = ( digits[0]*45912 +  digits[1]*35511  +  digits[2]*65169  + 4625 * digits[3]) % 65521;
	return hashed;

}

double MyDataStore::calc_similarities(User* user_a, User* user_b){
	
	map<Product*, vector<Review*> >::iterator it= product_review_map.begin();
	set<Product*> userA_product;
	set<Product*> userB_product;
	set<Product*> a_b_intersection;
	set<Product*>::iterator it_product;

	if ( product_review_map.empty()) {
		return 1.0; 
	}
	//finding product for A & B 
	for (; it != product_review_map.end(); ++it) {
		for (unsigned int i = 0; i < (it->second).size(); ++i) {
			if (user_a->getName() == (it->second)[i]->username) {
				userA_product.insert(it->first);
			}
			else if (user_b->getName() == (it->second)[i]->username) {
			    userB_product.insert(it->first);
			}
		}
	}
	//find intersection between A and B 
	a_b_intersection = setIntersection(userA_product,userB_product);
	if (a_b_intersection.empty()){
		return 1.0; 
	}
	double rating_result = 0; 
		int count = 0 ; 
		// findind the actual rating 
		for ( it_product = a_b_intersection.begin(); 
			it_product!=a_b_intersection.end();++it_product){
			std::vector<Review*> v_temp = product_review_map[*it_product];
			Review* a_review = NULL; 
			Review* b_review = NULL; 
			for (int i = 0 ; i <  (int) v_temp.size(); i ++){
				
				if (v_temp[i]->username == user_a->getName()){
					a_review = v_temp[i];
				}
				else if (v_temp[i]->username == user_b->getName()){
					b_review = v_temp[i];
				}
			}
			rating_result += abs(a_review->rating - b_review->rating) / 4.0; 
			count ++; 
		}

	if (count == 0.0) return 1.0;
	return rating_result / count; 
}


/*graph:
nodes = users
edge between each node = basic similarity 
use heap.h
*/
void MyDataStore::calc_refined(User* user){
	//map<User*, double> result;
	// clear the map 
	refined_result.clear(); 


	Heap<string> heap_;
	//Empty heap
	user->set_dist(0.0);
	user->set_pred(NULL);
	heap_.push(user->get_dist(), user->getName());
	map<User*, double>::iterator it_res;
	//if ( refined_result.find(user) == refined_result.end() ){
	refined_result.insert(make_pair(user,0.0));

	map<string, User*>::iterator it;
	//std::map<std::string, User*> v_users;
	for(it = v_users.begin(); it!= v_users.end(); ++it){
		User* temp = it->second;
		if(temp != user)
		{
			temp->set_dist(calc_similarities(user, temp)) ;
			temp->set_pred(user);
			heap_.push(temp->get_dist(), temp->getName());
			refined_result.insert(make_pair(temp,temp->get_dist()));
		}
	}
// store the distant <string, double> (refined)
	while(heap_.empty()==false){
		string v_ = heap_.top(); 
		heap_.pop();
		User* v_temp = getUser(v_);
		map<string, User*>::iterator it1;
		for (it1 = v_users.begin(); it1!= v_users.end(); ++it1){
			double weight = calc_similarities(v_temp, it1->second);
			if (v_temp->get_dist() + weight < it1->second->get_dist()){
				it1->second->set_pred(v_temp) ; 
				it1->second->set_dist(v_temp->get_dist() + weight) ;
				if ( refined_result.find(it1->second) != refined_result.end() ){
					//found: insert 
					refined_result[it1->second] = it1->second->get_dist(); 
				}
				else //not found : 
				{
					refined_result.insert(make_pair(it1->second, it1->second->get_dist()));

				}
				heap_.decreaseKey(it1->second->get_dist(), it1->second->getName());
			}
		}
		
	}
}


std::vector<std::pair<std::string, double> > MyDataStore::makeSuggestion(std::string currentUser){
	User* curr_user = (v_users.find(currentUser))->second; 
	std::vector<std::pair<std::string, double> > result; 

	std::map<std::string, std::vector<Review*>>::iterator it = p_reviews.begin();
	std::vector<Review*> not_reviewed_product;
	int count = 0; 
	for (; it!=p_reviews.end(); it++){
		bool reviewed = false;
		
		for (int i = 0; i < (int)it->second.size(); i++){
			if (it->second[i]->username == currentUser){
				reviewed = true;
				count++; 
			}
		}

		//cout << "COUNT" <<count << " " << p_reviews.size();
		if(count >= (int) p_reviews.size()) { 
    		cout << "No recommendations available" << endl;
    		return result;
    	}

		if (!reviewed) {
			calc_refined(curr_user); 
			double sum = 0.0;
			double weight_average = 0.0;
			double final_rating = 0.0; 
			// find all other uses who reviewed this book
			std::map<std::string, std::vector<Review*>>::iterator it2 = p_reviews.find(it->first);

			if ( it2 != p_reviews.end()){
				//find this product in the review
				not_reviewed_product = it2->second; 
				//loop through the review 
				for (int i = 0 ; i < (int)not_reviewed_product.size(); i ++){
					string new_user = not_reviewed_product[i]->username; 
					int rating = it->second[i]->rating; 
					User* temp =  getUser(new_user);
					double similarity = refined_result.find(temp)->second;
					sum += (1- similarity)* rating; 
					weight_average += (1 - similarity);

				}
				if (weight_average == 0 ){
					final_rating = 0.0; 
				}
				else {
					final_rating = sum / weight_average; 
				}

			}
			result.push_back(make_pair(it->first, final_rating)); 
			StableComp sc; 
			std::stable_sort(result.begin(), result.end(), sc);
		}
	}
	return result ;
}


User* MyDataStore::getUser(string user) {
	User* u = NULL;
	u = v_users.find(user)->second; 
	return u;
}