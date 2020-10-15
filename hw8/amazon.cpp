/* THINGS TO FIX:
1. print out 3 instead of 3.00 (is this ok? )
2. when login twice (with diff user) - refined similarity is 
not updated and will just follow the previous ones 
*/
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"
#include "datastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

bool validDate(string date){
    // YYYY-MM-DD
    char delimiter = '-';
    vector<string> out ; 
    size_t found = date.find(delimiter); 
    if (found == string::npos) {
        return false;
    }
    stringstream ss(date);
    string s; 
    while ( getline(ss, s, delimiter)){
        out.push_back(s); 
    }

    /* out[0] = YYYY
       out[1] = MM
       out[2] = DD
    */
    //int year = stoi(out[0]);
    int month = stoi(out[1]);
    int day = stoi(out[2]); 
   
    if (out[1].size() != 2 || out[2].size() != 2 || out[0].size()!=4){
        //cout << "first" << endl;
        return false; 
    }
    if ((month == 1) || (month == 3) || (month == 5) || (month == 7) ||
        (month == 8) || (month == 10) || (month == 12)){
        //cout << "second" << endl;
        
        if (day <= 31){
            return true; 
        }
        else {
            return false; 
        }
    }

    else if ((month == 4) || (month == 6) || (month == 9) || (month == 11))
    {
        //cout << "third" << endl;
       
       if (day <= 30){
          return true;
       }
       else return false; 
    }
    else if ((month == 2)){
        if (day <=28){
            return true; 
        }
        else 
            return false; 
    }
    return false;
}



int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;
    string curr_login_user; 

    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;
    ReviewSectionParser* reviewSectionParser = new ReviewSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);
    parser.addSectionParser("reviews", reviewSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    
    cout << "=====================================" << endl;
    cout << "  LOGIN username password            " << endl;
    cout << "  LOGOUT                             " << endl;
    cout << "  AND r/n term term ...              " << endl; 
    cout << "  OR r/n term term ...               " << endl;
    cout << "  ADD search_hit_number              " << endl; 
    cout << "  VIEWCART                           " << endl; 
    cout << "  BUYCART                            " << endl; 
    cout << "  ADDREV seach_hit_number rating date review_text" << endl;
    cout << "  VIEWREV seach_hit_number           " << endl;
    cout << "  REC                                " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "==================================== " << endl;

    /*sorting:
    r = review ( high to low ) 
    n = name ( alphabet - use default std::string comparison)
    */
    vector<Product*> hits;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if ( cmd =="LOGIN"){
                string user_input; 
                ss >> user_input; 
                string inputPassword;
                ss >> inputPassword; 
                if ( user_input.size()== 0){
                    cout << "Invalid username"<< endl;
                }
                if (ds.user_exist(user_input)==false){
                    cout << "Invalid username"<< endl;
                }
                unsigned long long hashed = ds.hash_function(inputPassword);
               // cout << "hashed: " << hashed << endl; 
                User* tempUser = ds.getUser(user_input);

                if(hashed == tempUser->getPassword()){
                   cout << "User logged in" << endl; 
                   curr_login_user = user_input; 
                   ds.set_curr_username(user_input); 
                   ds.set_login(true); 
                }
                else {
                    cout << "Invalid login credentials" << endl; 
                }
            }
            else if (cmd == "LOGOUT"){
                ds.set_login(false);
            }

            else if( cmd == "AND") {
                string sort; 
                ss >> sort; 

                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                if (sort == "r" ){
                    // call rating sort function
                   // cout << "in rating " << endl; 
                    hits = ds.ratingSort(hits);
                    displayProducts(hits);

                }
                else if (sort == "n"){
                    // call name sort function
                   // cout << "in name " << endl;
                    hits = ds.alphaSort(hits);
                    displayProducts(hits);
                }
                else {
                    continue;
                }
            }
            else if ( cmd == "OR" ) {
                string sort;
                ss >> sort; 
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                if (sort == "r" ){
                    hits = ds.ratingSort(hits);
                    displayProducts(hits);

                }
                else if (sort == "n"){
                    hits = ds.alphaSort(hits);
                    displayProducts(hits);
                }
                else {
                    continue; 
                }
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }

            else if ( cmd == "ADD"){
                string username;  
                int hit_index; 
                if ( ds.get_login()==true){
                    username = ds.get_curr_username();
                    ss >> hit_index;
                    // edge check: if enough argument is passed in
                    if ( hit_index <= 0 || hit_index > (int) hits.size()) {
                        cout << "Invalid Hit Index" << endl;
                        continue;
                    }
                    ds.add_cart(username, hits[hit_index - 1]);
                }
                else{
                    cout << "No current user" << endl ;
                }
            }

            else if (cmd == "VIEWCART"){
                string username; 
                 if ( ds.get_login()==true){
                    username = ds.get_curr_username();
                     ds.view_cart(username);
                }
                else{
                    cout << "No current user" << endl ;
                }
               
            }

            else if (cmd == "BUYCART"){
                string username; 
                if ( ds.get_login()==true){
                    username = ds.get_curr_username();
                    ds.buy_cart(username);
                }
                else{
                    cout << "No current user" << endl ;
                }
            }

            else if (cmd == "ADDREV"){
                int search_hit_number;
                int rating ; 
                string date;
                string review_text; 
                stringstream ss1;
                
                string username; 
                ss >> search_hit_number >> rating >> date;
                getline(ss,review_text);
            //    ss1 << review_text; 
              //  cout << "reivew text" << review_text << endl;
                // Check if date is valid 
                if (validDate(date) == false){
                    cout << "Invalid Date" << endl;
                    continue;  
                }
                // check rating: 1-5 only 
                if ( (rating <= 0) || (rating > 5)){
                    cout << "Invalid rating" << endl;
                    continue;
                }
                // check the hit number 
                if ( search_hit_number <= 0 || search_hit_number > (int) hits.size()) {
                    cout << "Invalid Hit Index" << endl;
                    continue;
                }
                if ( ds.get_login()==true){
                    username = ds.get_curr_username();
                    string productName = hits[search_hit_number-1]->getName();
                    ds.addReview(productName,
                    rating,username,date,review_text);
                }
                else{
                    cout << "No current user" << endl ;
                }
            }
            else if ( cmd == "VIEWREV"){
                // TODO: Tcall DATESORT
                int hit_index;
                ss >> hit_index;
                // edge check: if enough argument is passed in
                if ( hit_index <= 0 || hit_index > (int) hits.size()) {
                    cout << "Invalid Hit Index" << endl;
                    continue;
                }
                if ( ds.get_login()==true){
                    string productName = hits[hit_index-1]->getName();
                    vector<Review*> temp = ds.getReview(productName);
                    ds.viewReview(temp);   
                }
                else {
                    cout << "No current user" << endl;
                }
            }
            else if (cmd == "REC"){
                // make sure the person login
                if (ds.get_login() == false){
                    cout << "No current user" << endl; 
                    continue;
                }                
                std::vector<std::pair<std::string, double> > temp; 
                temp = ds.makeSuggestion(ds.get_curr_username());
                std::ofstream ofs("rec.txt");
                if (!temp.empty()){
                    ofs << ds.get_curr_username() << endl; 
                    cout << ds.get_curr_username() << endl; 
                }
                for ( int i =0; i < (int) temp.size(); i ++){
                    ofs << std::setprecision(2) << temp[i].second <<
                    " " << temp[i].first << endl; 
                    cout.setf(ios::fixed,ios::floatfield);
                    cout << std::setprecision(2) << temp[i].second <<
                    " " << temp[i].first << endl; 
                }
                ofs.close();

            }

            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
   // std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}