#include "Gallery.h"
#include "Enemy.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <regex>

using namespace std;

//Gallery ctor that takes two to six files

Gallery::Gallery(const string s1,
		 const string s2,
		 const string s3,
		 const string s4,
		 const string s5,
		 const string s6) : a1(s1), a2(s2), a3(s3), a4(s4), a5(s5), a6(s6) {
    check_args();

    for (string filename : filenames) {
	read(filename, keyfile);
  
     } //cout << "CONSTRUCTOR PASSED" <<endl;
//	cout <<"SIZE " << size() << endl;
}

Gallery::Gallery(const Gallery& gallery) : filenames(gallery.filenames), 
					   enemies(gallery.enemies),
					   keyfile(gallery.keyfile) {}

Gallery &Gallery::operator=(const Gallery& rhs){
    filenames = rhs.filenames;
    enemies = rhs.enemies;
    keyfile = rhs.keyfile;
    return *this;
}

Gallery::~Gallery(){}


void Gallery::check_args() {
    int files = 0, keyfiles = 0;
    vector<string> args{a1, a2, a3, a4, a5, a6};
    
    for (string arg : args) {
        
        if (arg != "" && isKeyfile(arg)) {
            ++keyfiles;
            keyfile = arg; //this arg is a keyfile
//	    cout << "IN KEY IF " << endl;
        }else if(arg != "") {
	    ifstream x(arg);
	    if(x.is_open()){

		x.seekg(0,ios::end);
		size_t size = x.tellg();
		if(size == 0){
		    continue;
		}

	    }  
            ++files;
            filenames.push_back(arg); //if argument is not empty, put in filenames vector
            x.close();
	}
	
    }
//     if (files < 1) {
//         throw runtime_error("invalid file list");
//     }
    if (keyfiles != 1) {
	throw runtime_error("invalid keyfile");
    }
}

bool Gallery::isKeyfile(string filename) const {
    string line;
    ifstream file(filename);
   
	//if there is empty line or whitespace it's not a keyfile
    if(!file.good()){
        throw runtime_error(" can't open enemy file" + filename);
    }	
    getline(file, line);
    if (line == "") {
        return false;
    }
    while(getline(file, line)){
        for (char c : line) {
            if(!isalnum(c) || isspace(c))  
                return false;
        }
    }
    file.close();
    return true;
}

void Gallery::read(const string filename, const string keyfilename){
	ifstream file(filename);//reads all enemies from a single file and puts them into the enemies vector
   // cout << "AFTER IFSTREAM" << endl; 
        bool done = false;
	while(!done){
    Enemy e(keyfilename); //make new enemy
	done = !(e.read(file));//e.read(file) calls read() method in Enemy.cc
	//read rutruns true when there is still more enemies to read in a file and the done will be false
	//until read returns false, then done is true.
	if(!done) {
                e.gallery = this;
		enemies.push_back(e);//add enemy to back of enemies vector
	}
    }
}

void Gallery::add(const Enemy& enemy){
    enemies.push_back(enemy);
}

void Gallery::clear(){
    filenames.clear();
    enemies.clear();
    keyfile = "";
}

size_t Gallery::size() const{
    return enemies.size();
}

bool Gallery::empty() const{
    return enemies.empty();
}

const Enemy* Gallery::get(size_t i) const {
//Enemy* Gallery::get( const size_t i){
    int size_call = size();
    int size_i = i;
    if (size_i < size_call) { //if it's within range
        return &enemies.at(i); //returns enemy at index i in enemies vector
    } else {
        throw range_error("given: " + to_string(i) + " maximum: " + to_string(size() - 1));
    }

}

Enemy* Gallery::get(size_t i){
//	return helper_get(i);
//Enemy* Gallery::get( const size_t i){
    if (i < size()) { //if it's within range
        return &enemies.at(i); //returns enemy at index i in enemies vector
    } else {
        throw range_error("given: " + to_string(i) + " maximum: " + to_string(size() - 1));
    }
}

ostream& operator<<(ostream& lhs, const Gallery& rhs){
    size_t index = 0;
    for (Enemy enemy : rhs.enemies) { //goes through all enemies in enemies vector
        lhs << enemy; //writes each enemy to ostream lhs
	if(index < rhs.enemies.size()-1){
	    lhs << "\n";}
	index++;
    }
    return lhs;
}

//return a reference to the corresponding Enemy in this Gallery.
const Enemy& Gallery::operator[](const size_t &x) const{
    const Enemy& e = enemies[x];
    return e;
}
//Succeed if and only if the Gallery is not empty.
Gallery::operator bool() const{
    return !empty();
}

///////////////////////////////////////NEW
//returns an object that corresponds to the first Enemy in the Gallery
Gallery::iterator Gallery::begin() const{
    return enemies.begin();//using the begin function of vector to return the first Enemy in enemies(the gallery)
}

//returns an object that corresponds to one past the last Enemy in the Gallery
Gallery::iterator Gallery::end() const{
    return enemies.end();//use the end function of vector, returns an iterator referring to one past the last Enemy in enemies(the gallery) 
}










///////////////////////////////////////
