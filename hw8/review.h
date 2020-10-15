#ifndef REVIEW_H
#define REVIEW_H
#include <string>

struct Review
{
    
    Review() :  prodName(), rating(), username(), date(), reviewText()
    { }
    Review(const std::string& prodName,
              int rating,
              const std::string& username,
              const std::string& date,
              const std::string& review_text) :
        prodName(prodName),
        rating(rating),
        username(username),
        date(date),
        reviewText(review_text)
    { }

    void dump(std::ostream& os){
      os<<prodName<<std::endl<<rating<<" "<<username
      <<" "<<date<<" "<<reviewText<<std::endl;
    }

    

    std::string prodName;
    int rating;
    std::string username;
    std::string date;
    std::string reviewText;
};

#endif
