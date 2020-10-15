#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "product.h"
#include "msort.h"
#include "util.h"
#include <iostream>
#include <set>
#include <map>
#include <vector> 

struct AlphaStrComp {
  bool operator() (const std::string& lhs, const std::string& rhs)
  { // Uses string's built in operator<
    // to do lexicographic (alphabetical) comparison
    return (lhs < rhs);
  }
};

// Comparitor for numbers
struct DigitComp {
  bool operator()(const double& lhs, const double& rhs) 
  { 
    return lhs > rhs; 
  }
};
// ADD ANOTHER DATE COMPARTOR 
struct DateComp{
  bool operator() (const std::string& lhs, const std::string& rhs)
  { 
    return (lhs > rhs);
  }

};

struct StableComp{
  bool operator() (const std::pair<std::string, double>& lhs
    , const std::pair<std::string, double>& rhs )
  { 
    return (lhs.second > rhs.second);
  }

};
// new struct comp that takes in Product 
// take in the product->getAverage

class MyDataStore: public DataStore {
public: 
	
	MyDataStore();
	~MyDataStore();
    /**
     * Adds a product to the data store
     */
    void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    std::vector<Product*> search(std::vector<std::string>& terms, int type) ;

    /**
     * Reproduce the database file from the current Products and User values
     */
    void dump(std::ostream& ofile);

	// function that will be used in amazon.cpp 
	void add_cart(std::string username, Product* p);
	void buy_cart(std::string username); 
	void view_cart(std::string username);
	
    //function for HW5 (Review function)
  void addReview(const std::string& prodName,
             int rating,
             const std::string& username,
             const std::string& date,
             const std::string& review_text);

  void viewReview(std::string productName); 

  std::vector<Review*> getReview(std::string s);
  std::vector<Product*> ratingSort(std::vector<Product*>& p) ;
  std::vector<Product*> alphaSort(std::vector<Product*>& p) ;
  // DATE SORT
  std::vector<Review*> dateSort(std::vector<Review*>& r);
  double getAverage(Product* p);
  bool get_login();
  void set_login(bool l); 
  std::string get_curr_username();
  User* getUser(std::string username);
  void set_curr_username(std::string username); 
  bool user_exist(std::string username); 
  void viewReview(std::vector<Review*>& r);
  unsigned long long hash_function(std::string password); // implement this inside .cpp
  double calc_similarities(User* user_a, User* user_b); 
  void calc_refined(User* u); 
  std::vector<std::pair<std::string, double> > makeSuggestion(std::string currentUser);
private:
	std::vector<Product*> v_products; // a vector of products 
	std::map<std::string, std::set<Product*>> map_product; // use this in search!
	std::map<std::string, User*> v_users;
 
	std::map<std::string, std::vector<Product*>> cart;  // link username - product
  std::map<std::string, std::vector<Review*>> p_reviews; // link prodName with reviews 
  std::map<Product*, std::vector<Review*>> product_review_map; 
  //std::vector<Review*> vector_of_reviews;
  std::string curr_username; 
  bool login;
  std::map<User*, double> refined_result;


};

#endif