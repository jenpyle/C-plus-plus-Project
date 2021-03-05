#include "Enemy.h"
#include "Gallery.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdexcept>
#include <utility>
#include <regex>

using namespace std;


//Constructor
//enemy b "keys" is parameterized ctor
Enemy::Enemy(const string keyfile) : show_names(true),
                                     show_links(true),
                                     show_others(true) {//reads in the keyfile given in ctor and stores into a private variable with all the keys in it
    string key;
    ifstream ifs (keyfile);
        while(getline(ifs, key)){
            for (char c : key) {
                if(!isalnum(c) || isspace(c))  
                    throw runtime_error("bad content in key file" + keyfile);
            }
        all_keys.push_back(key);
    }
}
//enemy object b
//in constructor when have an attribute(pass a value)
//copy constructor define b here^^ somewhere?
Enemy::Enemy(const Enemy &b) : enemies(b.enemies),
                               gallery(b.gallery),
                               show_names(b.show_names),
                               show_links(b.show_links),
                               show_others(b.show_others),
                               pairs(b.pairs) {
}

//destructor
Enemy::~Enemy(){}

//Assignment operator
Enemy &Enemy::operator=(const Enemy &b) {
	enemies = b.enemies;
	gallery = b.gallery;
	show_names = b.show_names;
	show_links = b.show_links;
	show_others = b.show_others;
	return *this;
}

bool Enemy::read(istream &iss) {//declared istream so we can call it on the input stream lair
    
    vector<string> lines;
    string line;
    //vector<vector<string>> formatted_lines;
    while (getline(iss, line)) { //gets every line in the file
        if(line.find_first_of(" \n\t\r\n") == 0 && lines.empty()){
            throw runtime_error("bad content in enemy file");
        }
        if ((line.empty() || line.find_first_not_of(" \n\t\r\n")==string::npos) && lines.empty()){
            continue;
        }
        if ((line.empty() || line.find_first_not_of(" \n\t\r\n")==string::npos) && !lines.empty()){
            break;
        }
        lines.push_back(line);
    }
    if (lines.empty()) {return false;}
    formatted_lines = format_lines(lines);
    enemies = seperate_enemies(formatted_lines);
    make_pairs();

//making a keys vector for the specific enemy
    for (vector<string> kv : enemies[0]){
        string key = kv[0];
        key_vector.push_back(key);
    }

    if (!all_keys.empty()) {
        check_enemies(all_keys, enemies);
    }
    return true;
}

void Enemy::write(ostream &ost) const {
    output_enemy(ost);
}

void Enemy::write(const string filename) const {
    ofstream file(filename);
    if (file.good()) {
    	write(file);
    } else {
        throw runtime_error(filename);
    }
}

string Enemy::field(const string key) const {
    if (empty()) {
        throw range_error(key);
    }
    for (vector<string> line : enemies[0]) {
        string value = line[1];
        value.erase(0, value.find_first_not_of(" \n\r\t\f\v"));
        value.erase(value.find_last_not_of(" \n\r\t\f\v") + 1);
        if (key == line[0]) {
            return value;
        }
    }
    throw range_error(key);
}
//ADDED CONST TO ALL OF THESE
void Enemy::show_name(const bool x) {
    show_names = x; 
}
void Enemy::show_name() {//if it's missing
    show_name(true);
}

void Enemy::show_other(const bool y) {
    show_others = y;
}
void Enemy::show_other() {
    show_other(true);
}

void Enemy::show_link(const bool z) {
    show_links = z;
}
void Enemy::show_link() {
    show_link(true);
}

void Enemy::clear() {
   enemies[0].clear();
   pairs.clear();
}

size_t Enemy::size() const {
    if (enemies.empty() || enemies[0].empty()) {
	return 0;
    } else {
    	size_t count = 0;
    	for (int i = 0; i < int(enemies[0].size()); ++count, ++i);
    	return count;
    }
}


bool Enemy::empty() const {
    if (enemies.empty()) {
        return true;
    } else {
        return enemies[0].empty();
    }
}

//GET KV PAIR
vector<string> Enemy::get_kv_pair(const string line) const {
    istringstream iss(line);
	string key, value, word; 
	vector<string> output;

	for (int i = 0; iss >> word; ++i) {
		if (i == 0) {
			key = word;}
		else {
			value = value + ' ' + word;}
	}
	output.push_back(key);
	output.push_back(value);
	return output;
}
//FORMATT LINES
vector<vector<string>> Enemy::format_lines(const vector<string> lines) const {
    vector<string> unsplit_lines;
	string full_line = "";

	for (string line : lines) {
		if (line[0] == ' ' || line[0] == '\t') {
			full_line += line;}
        else {
			unsplit_lines.push_back(full_line);
			full_line = line;}
	}
	unsplit_lines.push_back(full_line);
	vector<vector<string>> split_lines;
	for (string unsplit_line : unsplit_lines) {
		split_lines.push_back(get_kv_pair(unsplit_line));
	}
	return split_lines;
}

//TRIM
string Enemy::trim(string str){
    const auto strBegin = str.find_first_not_of(" \n\t\r\n");
    if (strBegin == std::string::npos)
        return ""; // no content
    const auto strEnd = str.find_last_not_of(" \n\t\r\n");
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

//MAKE PAIRS
void Enemy::make_pairs(){
    if(pairs.size()==0){
        for(vector<string> line : formatted_lines){
            string key = trim(line[0]);
            string val = trim(line[1]);
            pair<string,string> p = make_pair(key, val);
            if (p.first.size()==0)
                continue;

            pairs.push_back(p);//makes vector of all pairs
        }
    }
}

//SEPERATE ENEMIES
vector<vector<vector<string>>> Enemy::seperate_enemies(const vector<vector<string>> lines) const {
	vector<vector<string>> enemy;
	vector<vector<vector<string>>> temp_enemies;
	vector<vector<vector<string>>> unsorted_enemies;
	vector<vector<vector<string>>> enemies_1;
	vector<vector<string>> names, others, links;

	for (long unsigned int i = 0; i < lines.size(); ++i) {
		if (lines[i][0].length() == 0) {
			temp_enemies.push_back(enemy);
			enemy.clear();}
        else {
			enemy.push_back(lines[i]);}
	}
	temp_enemies.push_back(enemy);
	for (long unsigned int i = 0; i < temp_enemies.size(); ++i) {
		if (!temp_enemies[i].empty()) {
			unsorted_enemies.push_back(temp_enemies[i]);}
	}
	for (vector<vector<string>> unsorted_enemy : unsorted_enemies) {
		vector<vector<string>> sorted_enemy;
		for (vector<string> line : unsorted_enemy) {
			if (line[0] == "Name") {names.push_back(line);}
			else if (line[0].substr(0, 4) == "Link") {links.push_back(line);}
			else {others.push_back(line);}}
		for (vector<string> line : names) {sorted_enemy.push_back(line);}
		for (vector<string> line : others) {sorted_enemy.push_back(line);}
		for (vector<string> line : links) {sorted_enemy.push_back(line);}
		enemies_1.push_back(sorted_enemy);
		names.clear();
		others.clear();
		links.clear();
	}
	return enemies_1;
}
//CHECK ENEMIES

void Enemy::check_enemies(const vector<string> allowed_keys, const vector<vector<vector<string>>> enemies_1) const {
    vector<string> enemy_keys;
	locale loc;
	for (vector<vector<string>> enemy : enemies_1) {
		bool has_name = false;
		for (vector<string> line : enemy) {
			if (line[0] == "Name") {
				has_name = true;}
		}
		if (!has_name) {
			throw runtime_error("keys: invalid enemy: 'Name' key required.\n");
		}
		for (vector<string> line : enemy) {
			string key = line[0];
			string value = line[1];
			if (key.length() == 0) {
				throw runtime_error("keys: invalid key: key is missing.\n");}
			if (value.length() == 0) {
				throw runtime_error("keys: invalid value: value is missing.\n");
			}
			bool allowed = false;
			for (string allowed_key : allowed_keys) {//goes through the allowed keys and if it is a valid key then it WONT throw an error
				if (key == allowed_key) {
					allowed = true;}
			}
			if (!allowed) {//if allowed is never set to TRUE then the program will throw an error because the key was invalid
				throw runtime_error("keys : invalid key: " + key + ", key is not in list of allowed keys.\n");}
			for (char c : key) {
				if (!isalnum(c, loc)) {
					throw runtime_error("keys : invalid key: " + key + ", contains non-alpha characters.\n");}
			}
			for (int i = 0; i < int(enemy_keys.size()); ++i) {
				if (enemy_keys[i] == key) {
					throw runtime_error("keys : invalid key: " + key+ ", duplicate key in this enemy.\n");}
			}
			enemy_keys.push_back(key);}
		enemy_keys.clear();//clears enemy_keys vector after it is done with enemy
	}
}

//GET PADDING
string Enemy::get_padding(const int longest_key, const int current_key_length) const {//function to make a string with the number of spaces needed to pad the output
	string padding = "";//empty string
	for (int i = 0; i < longest_key - current_key_length; ++i) {//adding the number of spaces it needs for this key
		padding += ' ';
	}
	return padding;
}
//PRINT OUTPUT
vector<int> Enemy::get_longest_keys() const {
	vector<int> output;
	long unsigned int longest_name = 4;
	long unsigned int longest_other = 0;
	long unsigned int longest_link = 0;

	for (vector<string> line : enemies[0]) {
		if (line[0] == "Name") {
			//longest name is always 4
		} else if (line[0].substr(0, 4) == "Link") {
			if (line[0].size() > longest_link) {longest_link = line[0].size();}
		} else {
			if (line[0].size() > longest_other) {longest_other = line[0].size();}
		}
	}

	output.push_back(int(longest_name));
	output.push_back(int(longest_other));
	output.push_back(int(longest_link));

	return output;
}
void Enemy::output_enemy(ostream &ost) const {//function prints formatted output
	string Link = "Link";

	vector<vector<string>> enemy = enemies[0];
	int longest_key = 0;

	vector<int> longest_keys = get_longest_keys();

	if (show_names) {
		if (show_others) {
			if (show_links) {longest_key = max(4, max(longest_keys[1], longest_keys[2]));}
			else {longest_key = max(4, longest_keys[1]);}
		} else if (show_links) {
			longest_key = max(4, longest_keys[2]); 
		} else {
			longest_key = 4;
		}
	}
	else if (show_others) {
		if (show_links) {
			longest_key = max(longest_keys[1], longest_keys[2]);
		} else {
			longest_key = longest_keys[1];
		}
	}
	else if (show_links) {
		longest_key = longest_keys[2];
	}
	else {
		longest_key = max(4, max(longest_keys[1], longest_keys[2]));
	}

	for (vector<string> line : enemy) {//goes through each line in current enemy
		string key = line[0];//gets the key out of that line
		string value = line[1];//gets the value out of the line
		string padding = get_padding(longest_key, key.length());//calculate the padding as a string


        if (show_names && key == "Name") {ost << key << padding << value << '\n';}
		if (show_links && key.substr(0, 4) == Link) {ost << key << padding << value << '\n';}
		if (show_others && key != "Name" && key.substr(0, 4) != Link) {ost << key << padding << value << '\n';}
		if (!show_names && !show_links && !show_others) {ost << key << padding << value << '\n';}
	}
}
//////////////////////////////////////////////////////
Enemy* Enemy::link(const string relation) const {
	//Ex relation = "Sister"
    string link;
    try {
        link = field("Link" + relation); //finds value of kv pair in this enemy where key = "LinkSister", Ex link = "Gretel";
    } catch (range_error& e) {
        throw runtime_error(e.what()); //if no line with key "LinkSister", throw runtime_error
    }
    for (int i = 0; i < int(gallery->size()); ++i) { //go through all enemies in this containing Gallery
        if (gallery->enemies[i].field("Name") == link) { //if an enemy's "Name" field is "Gretel"
            Enemy* point = &gallery->enemies[i];
            //return &gallery->enemies[i]; //return that enemy
            return point;
	}
    }
    throw range_error(link); //if it didn't find one named "Gretel" throw range_error
}

ostream &operator<<(ostream &stream, const Enemy &enemy){
    enemy.write(stream);
    return stream;
}
///////////////////////////
//Given a zero-based index, return a by value a std::pair containing, in order, the corresponding key & value.
pair<string,string> Enemy::operator[](const size_t x) const{
    if(x >= size() || x>=enemies[0].size()){
        throw range_error("Index out of bounds");
    }
    string key = enemies[0][x][0]; //first [0] - enemies vector only has one Enemy
    string value = enemies[0][x][1];//second [x] - index from function argument
					//third [0] or [1] - key or value from each line
    pair<string, string> p;

    key.erase(0, key.find_first_not_of(" \n\r\t\f\v"));
    key.erase(key.find_last_not_of(" \n\r\t\f\v") + 1);
    value.erase(0, value.find_first_not_of(" \n\r\t\f\v"));
    value.erase(value.find_last_not_of(" \n\r\t\f\v") + 1);

    p.first = key;
    p.second = value;
    return p;

}
//////////////////////////converts c-style string array to string
string Enemy::operator[](const char* key) const{
    string s(key);
    return field(s);
}
//key can be either a C-style string or a std::string
string Enemy::operator[](const string key) const{
    return field(key);
}
//Succeed if and only if the Enemy is not empty.
Enemy::operator bool() const{
    return !empty();
}
//////////////////////////////////////////////////////////////////////////////
//if current enemy is the same as enemy e return true
bool Enemy::operator==(const Enemy& e) const{
    //compares all data in the class
    //(data that can vary between Enemy class instances)
    if (key_vector != e.key_vector) {
	return false;
    } else if (enemies != e.enemies) {
        return false;
    } else if (gallery != e.gallery) {
        return false;
    } else if (show_names != e.show_names || show_links != e.show_links || show_others != e.show_others) {
        return false;
    }
    if(size() != e.size())
        return false;
    for (size_t i = 0; i<key_vector.size(); i++){
        string val;
        try{
            val = e.field(key_vector[i]);
        }
        catch(const range_error &f){
                return false;
            }
        if(field(key_vector[i]) != val){
            return false;
        }
    }
    return true;
    }

//////////////////////////////////////////////////////////////////////////////
bool Enemy::operator!=(const Enemy &e) const{//Succeed if and only if the Gallery is not empty
    return !(*this==e);
}

Enemy::iterator Enemy::begin() const {
    return pairs.begin();//using the begin function of vector to return the first Enemy in enemies(the gallery)
}

//returns an object that corresponds to one past the last Enemy in the Gallery
Enemy::iterator Enemy::end() const {
    return pairs.end();//use the end function of vector, returns an iterator referring to one past the last Enemy in enemies(the gallery) 
}

