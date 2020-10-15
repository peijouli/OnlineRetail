#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>

/**
 * Implements User functionality and information storage
 *  You should not need to derive anything from User at this time
 */
class User {
public:
    User();
    User(std::string name, double balance, int type, unsigned long long hash_password);
    virtual ~User();

    double getBalance() const;
    std::string getName() const;
    void deductAmount(double amt);
    virtual void dump(std::ostream& os);
    unsigned long long getPassword(); 
    bool check_password(unsigned long long password);
    User* get_pred(); 
    double get_dist();
    void set_pred(User* pred); 
    void set_dist(double dist);

private:
    std::string name_;
    double balance_;
    int type_;
    unsigned long long hash_password_; 
    User* pred_;
    double dist_;

};
#endif
