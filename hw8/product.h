#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include "review.h"

class Product {
public:
    Product(const std::string category, const std::string name, double price, int qty);
    virtual ~Product();

    /**
     * Returns the appropriate keywords that this product should be associated with
     */
    virtual std::set<std::string> keywords() const = 0;

    /**
     * Allows for a more detailed search beyond simple keywords
     */
    virtual bool isMatch(std::vector<std::string>& searchTerms) const;

    /**
     * Returns a string to display the product info for hits of the search
     */
    virtual std::string displayString() const = 0;

    /**
     * Outputs the product info in the database format
     */
    virtual void dump(std::ostream& os) const;

    /**
     * Accessors and mutators
     */
    double getPrice() const;
    std::string getName() const;
    std::string get_name();
    int getQty() const;
    void subtractQty(int num);
    void setAverage(double d);
        // average_rating = d; 
    
    double getAverage() const;
    std::vector<Review*> get_vector();


protected:
    std::string name_;
    double price_;
    int qty_;
    std::string category_;
    // vector of review
    std::vector<Review*> review_vector; 
    double average_rating; 

};
#endif
