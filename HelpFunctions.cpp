#include "HelpFunctions.h"

bool isSorted(const Vector<int>& vec) {
	for (size_t i = 0; i < vec.size() - 1; i++) {
		if (vec[i] > vec[i + 1]) {
			std::cout << vec[i] << " " << vec[i + 1] << std::endl;
			return false;
		}
	}
	return true;
}

Vector<int>& randomIntVector(size_t length, int lowerBound, int upperBound) {
	//initialize random seed:

	Vector<int>* vecPtr = new Vector<int>(length);
	for (size_t i = 0; i < length; i++) {
		vecPtr->pushBack((rand() % (upperBound - lowerBound + 1)) + lowerBound);
	}

	return *vecPtr;
}

struct Afonso {

	int age;
	bool sex;
	std::string name;
	Afonso(int age, std::string name, bool sex) : age(age), name(name), sex(sex) {}
};

std::ostream& operator<<(std::ostream& stream, const Afonso& afonso) {
	stream << "Afonso<" << afonso.age << ", " << afonso.name << ", " << afonso.sex << ">";
	return stream;

}

void vectorTester() {
	//Using ints
	{
		std::cout << "Using data type: INT\n";
		Vector<int> defaultIntVec;
		Vector<int> IntAllocNoFill(10);
		Vector<int> IntAllocFill(10, 87);

		std::cout << "Default :\n";
		std::cout << defaultIntVec << std::endl;
		std::cout << "Allocation No Fill:\n";
		std::cout << IntAllocNoFill << std::endl;
		std::cout << "Allocation And Fill\n";
		std::cout << IntAllocFill << std::endl;

		std::cout << "----------------------------------\n";
	}
	//Using Floats	
	{
		std::cout << "Using data type: FLOATS\n";
		Vector<float> defaultFloatVec;
		Vector<float> FloatAllocNoFill(10);
		Vector<float> FloatAllocFill(10, 87.87);

		std::cout << "Default :\n";
		std::cout << defaultFloatVec << std::endl;
		std::cout << "Allocation No Fill:\n";
		std::cout << FloatAllocNoFill << std::endl;
		std::cout << "Allocation And Fill\n";
		std::cout << FloatAllocFill << std::endl;

		std::cout << "----------------------------------\n";
	}
	//Using  std::string
	{
		std::cout << "Using data type: STD::STRING\n";
		Vector<std::string> defaultStdStringVec;
		Vector<std::string> stdStringAllocNoFill(10);
		Vector<std::string> stdStringAllocFill(10, "String");

		std::cout << "Default :\n";
		std::cout << defaultStdStringVec << std::endl;
		std::cout << "Allocation No Fill:\n";
		std::cout << stdStringAllocNoFill << std::endl;
		std::cout << "Allocation And Fill\n";
		std::cout << stdStringAllocFill << std::endl;


		std::cout << "----------------------------------\n";
	}
	//Usint Dummy
	{
		std::cout << "Using data type: Dummy\n";
		Vector<Dummy> defaultDummyVec;
		Vector<Dummy> DummyAllocNoFill(10);
		Vector<Dummy> DummyAllocFill(10, Dummy("Martim"));

		std::cout << "Default :\n";
		std::cout << defaultDummyVec << std::endl;
		std::cout << "Allocation No Fill:\n";
		std::cout << DummyAllocNoFill << std::endl;
		std::cout << "Allocation And Fill\n";
		std::cout << DummyAllocFill << std::endl;

		std::cout << "----------------------------------\n";
	}
	//Using  std::vector
	{
		std::cout << "Using data type: STD::VECTOR\n";

		Vector<std::vector<std::string>> defaultstdvecVec;
		Vector<std::vector<std::string>> stdvecAllocNoFill(10);
		std::vector<std::string> vectorstring;
		vectorstring.push_back("Test");
		Vector<std::vector<std::string>> stdvecAllocFill(10, vectorstring);

		std::cout << "Default :\n";
		std::cout << defaultstdvecVec << std::endl;
		std::cout << "Allocation No Fill:\n";
		std::cout << stdvecAllocNoFill << std::endl;
		std::cout << "Allocation And Fill\n";
		std::cout << stdvecAllocFill << std::endl;

		std::cout << "----------------------------------\n";
	}
	//Using  Vector 
	{
		std::cout << "Using data type: VECTOR\n";


		std::vector<std::string> vectorofstring;
		vectorofstring.push_back("Mega Test");
		Vector <std::vector<std::string>> Vecofvecofstr(3, vectorofstring);
		Vector<Vector<std::vector<std::string>>> VecOfVecOfvecOfString(3, Vecofvecofstr);
		std::cout << VecOfVecOfvecOfString;


		std::cout << "----------------------------------\n";
	}

}
