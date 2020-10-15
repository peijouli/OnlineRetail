#include "user.h"
using namespace std;

User::User() : name_("unknown"), balance_(0.0), type_(1)
{

}
User::User(std::string name, double balance, int type, unsigned long long hash_password) :
    name_(name), balance_(balance), type_(type), hash_password_(hash_password)
{

}

User::~User()
{

}


std::string User::getName() const
{
    return name_;
}

double User::getBalance() const
{
    return balance_;
}

void User::deductAmount(double amt)
{
    balance_ -= amt;
}

void User::dump(std::ostream& os)
{
    os << name_ << " "  << balance_ << " " << type_ << endl;
}

unsigned long long User::getPassword(){
	//cout << hash_password_; 
	return hash_password_; 
}

bool User::check_password(unsigned long long password){
	
	if ( password == hash_password_){
		return true ;
	}
	else return false; 
}

 User* User::get_pred(){
 	return pred_;
 }
double User::get_dist(){
	return dist_;
}

void User::set_pred(User* pred){
	pred_ = pred; 
}
void User::set_dist(double dist){
	dist_ = dist;
}