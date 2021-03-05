#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED


#include <iostream>
#include <string>
#include <vector>
#include <iomanip>	//for setw
#include <string>	//for string
#include <vector>	//for vector (ArrayList)
#include <sstream>	//for stringstream
#include <locale>	//for isaplha() and locale
#include <fstream>  //ifstream()
#include <getopt.h> //getopt()
#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <utility>
#include <regex>

class Gallery;//pointer to Gallery

class Enemy{
    public:
        Enemy(const std::string keyfile);
        Enemy(const Enemy &);//copy constructor
        ~Enemy(); //note lack of arg name
        Enemy &operator=(const Enemy &);
 
        std::vector<std::string> key_vector;
        std::vector<std::vector<std::vector<std::string>>> enemies;
        std::vector<std::vector<std::string>> formatted_lines;
//        std::pair<std::string,std::string> xy;



	Gallery* gallery;
        bool show_names; 
        bool show_links;
        bool show_others;


	bool read(std::istream&);
	void make_pairs();
        void write(std::ostream&) const;               //Write the content of the Enemy to the ostream,
        void write(const std::string filename) const;  //Write the content of the Enemy to the given file
        std::string field(const std::string) const;   //Return the corresponding value for this key
        size_t size() const;                          //Return a size_t containing the number of key/value pairs in this Enemy.
        void show_name(const bool);                    //If the argument is true or missing, show Name key/value pairs when writing the Enemy
        void show_name();
        void show_link(const bool);
        void show_link();                            //If the argument is true or missing, show Link… key/value pairs when writing the Enemy
        void show_other(const bool);                  //If the argument is true or missing, show non-Name, non-Link… key/value pairs when writing the Enemy
        void show_other();	

        void clear();                        //Make this Enemy have no key/value pairs.
        bool empty() const;                  //Return true iff Enemy has no key/value pairs.
        Enemy* link(const std::string) const;//Returns a pointer to a related Enemy from the same Gallery.
        
        std::pair<std::string, std::string> operator[](const std::size_t ) const;
        std::string operator[](const char*) const;//Return a copy of the value for that key
        std::string operator[](const std::string) const;//Return a copy of the value for that key
        operator bool() const;//Succeed if and only if the Enemy is not empty.
        bool operator==(const Enemy &) const;
        bool operator!=(const Enemy &) const;
        
        
///////////////////////////////////////NEW
        std::string trim(std::string);

        using iterator =std::vector<std::pair<std::string,std::string>>::const_iterator;
        iterator begin() const;
        iterator end() const;

        
        
        
///////////////////////////////////////

        
    private:
        std::vector<std::string> all_keys;
        std::vector<std::vector<std::string>> format_lines(const std::vector<std::string> lines) const;
        std::vector<std::string> get_kv_pair(const std::string line) const;
        std::vector<std::vector<std::vector<std::string>>> seperate_enemies(const std::vector<std::vector<std::string>> lines) const;
        void check_enemies(const std::vector<std::string> allowed_keys, const std::vector<std::vector<std::vector<std::string>>> enemies) const;
        std::string get_padding(const int longest_key, const int current_key_length) const;
        
                std::vector<std::pair<std::string, std::string>> pairs;
	
        std::vector<int> get_longest_keys() const;
        void output_enemy(std::ostream &ost) const;
};
        std::ostream &operator<<(std::ostream &, const Enemy &);

#endif /* ENEMY_H_INCLUDED */
