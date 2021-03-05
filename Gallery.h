#ifndef GALLERY_H_INCLUDED
#define GALLERY_H_INCLUDED


#include <string>	//for string
#include <vector>	//for vector (ArrayList)
#include "Enemy.h"
#include <regex>


class Gallery{
    public:
        Gallery(const std::string s1="",
		const std::string s2="",
		const std::string s3="",
		const std::string s4="",
		const std::string s5="",
		const std::string s6=""
	);
	Gallery(const Gallery &);
	Gallery &operator=(const Gallery &);
	~Gallery();

	const std::string a1, a2, a3, a4, a5, a6;
    std::vector<std::string> filenames;
	std::vector<Enemy> enemies;//vector of Enemies, a gallery can have multiple Enemy vectors. emenies is the gallery
	std::string keyfile;

	void read(const std::string, const std::string);//Read enemies from this file, using this keyfile, adding them to the Gallery.
	void add(const Enemy &);           //Given an Enemy (not a pointer), add it to this Gallery.
	void clear();                      //Make this Gallery empty.
	size_t size() const;               //Return the number of enemies in this object, as a size_t.
	bool empty() const;                //Return true iff this object has no entries.
	const Enemy* get( size_t) const;   //Given a zero-based index, return a pointer to that Enemy. 
	Enemy* get(size_t);
	Enemy* helper_get(const size_t);
    const Enemy& operator[](const std::size_t &x) const;
    operator bool() const;//Succeed if and only if the Gallery is not empty

    
    
///////////////////////////////////////NEW
    using iterator = std::vector<Enemy>::const_iterator;//namespae variable that has functionality of vector class into global namespace
    iterator begin() const;//Returns an object of type Gallery::iterator that corresponds to the first Enemy in the Gallery
    iterator end() const;//Returns an object of type Gallery::iterator that corresponds to ONE PAST the last Enemy in the Gallery

/////////////////////////////////////    Enemy.cc has a pair vector type




private:
        void check_args();
        bool isKeyfile(const std::string) const;
};

std::ostream &operator<<(std::ostream&, const Gallery&);
//Enemy* &operator[](size_t x);

        
#endif
