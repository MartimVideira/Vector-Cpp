#pragma once
#include <iostream>
#include <string>
#include <vector>

static const char* DUMMY_DEFAULT_NAME = "Nobody";
static const size_t DUMMY_NAME_SIZE = 7;

class Dummy {

//Atributes
private:
	static int counter;
	int number;
	char* name;

//Methods
public:
	//Default Constructor
	Dummy();
	// String Constructor
	Dummy(std::string newname);
	
	//copy constructor
	Dummy(const Dummy& other);

	//Move constructor 
	Dummy(Dummy&& other);

	//COpy assign lvalue
	Dummy& operator=(const Dummy& other);

	//Move assign rvalue
	Dummy& operator=(Dummy&& other);

	
	
	
	// Destructor
	~Dummy();




	friend std::ostream& operator<<(std::ostream& out, const Dummy& dummy);

};

