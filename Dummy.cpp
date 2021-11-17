#include "Dummy.h"

int Dummy::counter = 0;
Dummy::Dummy(){
	number = counter++;
	name = new char[DUMMY_NAME_SIZE];
	memcpy(name, DUMMY_DEFAULT_NAME, DUMMY_NAME_SIZE);

}

Dummy::Dummy(std::string newname) {
	number = counter++;
	name = new char[newname.size()+1];
	for (size_t i = 0; i < newname.size(); i++)
		name[i] = newname[i];
	name[newname.size()] = 0;
}

Dummy::Dummy(const Dummy& other) {
	//std::cout << "Copy Constructor!\n";
	number = other.number;
	size_t name_size = strlen(other.name);
	name = new char[name_size +1];
	for (size_t i = 0; i < (name_size + 1); i++)
		name[i] = other.name[i];

}

Dummy::Dummy(Dummy&& other) {
	//std::cout << "Move Constructor!\n";
	number = other.number;
	other.number = -1; //invalid State
	name = other.name;
	other.name = nullptr;

}

Dummy& Dummy::operator=(const Dummy& other) {
	//std::cout << "Copy Assignment!\n";
	number = other.number;
	size_t name_size = strlen(other.name);
	name = new char[name_size + 1];
	for (size_t i = 0; i < (name_size + 1); i++)
		name[i] = other.name[i];
	
	return *this;
	
}

Dummy& Dummy::operator=(Dummy&& other) {
	//std::cout << "Move Assignment!\n";
	number = other.number;
	other.number = -1; //invalid State
	name = other.name;
	other.name = nullptr;
	
	return *this;
}

Dummy::~Dummy() {
	//std::cout << "Destructed!\n";

	delete[] name;
}




std::ostream& operator<<(std::ostream& out, const Dummy& dummy) {
	const char* name = (dummy.name != nullptr) ? dummy.name : "INVALID OBJECT";
	out << "Dummy<" << dummy.number << ", " << name << ">";
	return out;
}
