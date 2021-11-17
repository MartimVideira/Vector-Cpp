#pragma once
#include <ostream>
#include <string>
#include <vector>


template<typename Vector>
class VectorIterator {
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;

private:
	PointerType m_Ptr;

public:
	VectorIterator(PointerType ptr) : m_Ptr(ptr) {};

	//Prefix ++
	VectorIterator& operator++() {
		m_Ptr++;
		return *this;
	}
	//Postfix ++
	VectorIterator operator++(int) {
		VectorIterator copyIterator = *this;
		++(*this);
		return copyIterator;

	}
	//Prefix --
	VectorIterator& operator--() {
		m_Ptr--;
		return *this;
	}
	//Postfix --
	Vector operator--(int) {
		VectorIterator copyIterator = *this;
		--(*this);
		return copyIterator;

	}
	ReferenceType operator[](size_t index) {
		return *m_Ptr[index];
	}

	PointerType operator->() {

		return m_Ptr;
	}

	ReferenceType operator*() {
		return  *m_Ptr;
	}

	bool operator==(const VectorIterator& other)const  {
		return m_Ptr == other.m_Ptr;
	}

	bool operator!=(const VectorIterator& other)const {
		return !(*this == other);
	}
};





/*

O m_Size quando estamos a criar um novo vetor só se atualiza para o size final depois de
realocar memoria pois vai tentar copiar o que tinha se tinha antes para o novo buffer logo 
o m_Size so pode ser alterado depois dde reAlloc()

As i´m allocating memory with ::new(destination, size) which does not builds 
an default object nor initializes the memory i neeed to use the in place new 
to build the object because, using the  = will throw an error as the  memory 
is unintialized.
*/

template<typename T>
class Vector{

public :
	using ValueType = typename T;
	using Iterator = VectorIterator<Vector<T>>;
//Atributs
private:

	T* m_Buffer = nullptr;
	size_t m_Size;
	size_t m_Capacity;


private:
	//Dynamically resizing the vector
	void reAlloc(size_t newCapacity) {

		T* newBuffer = ( T*)::operator new(newCapacity * sizeof(T));
		size_t oldSize = m_Size;
		size_t newSize = (newCapacity < oldSize) ? newCapacity : oldSize;

		//Copying the old items into the new buffer
		for (size_t i = 0; i < newSize; i++)
			// With the move , a move will be done when possible
			//Doing it like this  will call the move assign operator but the memory called is not initialized
			// meaning that there is no object constructed on memory so we need to construct it directly in memory
			// as dooing this would cause an exception , trying to read from un initialized memory
			//newBuffer[i] = std::move(m_Buffer[i]); 
			new(&newBuffer[i]) T(std::move(m_Buffer[i]));

		//Deleting the old items (because they can old some memory)
		for (size_t i = 0; i < oldSize;i++)
			m_Buffer[i].~T();
		
		//Now the buffer can be safely freed
		::operator delete(m_Buffer,m_Capacity * sizeof(T));
		
		//Updating
		m_Buffer = newBuffer;
		m_Size = newSize;
		m_Capacity = newCapacity;

	}

//Methods
public:

	//Default Constructor	
	Vector() :m_Size(0), m_Capacity(0) { reAlloc(2); }

	//Reserving Size Constructor 
	Vector(size_t reservedSize) : m_Size(0), m_Capacity(reservedSize) {
		reAlloc(reservedSize);
	}

	//Reserving Size and filling Constructor
	Vector(size_t reservedSize, const T& element):m_Capacity(reservedSize){
		reAlloc(reservedSize);
		m_Size = reservedSize;

		for (size_t i = 0; i < m_Size;i++)
			//In place building the object 
			new(&m_Buffer[i]) T(element);

	}
	//Copy COnstructor 
	Vector(const Vector<T> &other):m_Size(0), m_Capacity(other.m_Capacity) {
		reAlloc(m_Capacity);

		m_Size = other.m_Size;
		for (size_t i = 0; i < m_Size; i++)
			new(&m_Buffer[i]) T(other.m_Buffer[i]);

	}


	//Move Constructor
	Vector(Vector<T>&& other):m_Size(other.m_Size), m_Capacity(other.m_Capacity),m_Buffer(other.m_Buffer) {
		other.m_Size = 0;
		other.m_Capacity = 0;
		other.m_Buffer = nullptr;
		

	}
	//Destructor 
	~Vector() {
		//Destroying the vector elements
		for (size_t i = 0; i < m_Size; i++)
			m_Buffer[i].~T();
		
		//Freeing vector memory
		::operator delete(m_Buffer, m_Capacity * sizeof(T));
	}

	size_t size() const {
		return m_Size;
	}
	//pushBack copying  
	void pushBack(const T& element) {
		//std::cout << "Copied\n";
		if (m_Size == m_Capacity)
			reAlloc(m_Capacity + m_Capacity / 2);
		new(&m_Buffer[m_Size]) T(element);
		m_Size++;
	}
	//push back moving
	void pushBack(T&& element){
		//std::cout << "Moved\n";
		if (m_Size == m_Capacity)
			reAlloc(m_Capacity + m_Capacity / 2);
		new(&m_Buffer[m_Size]) T(std::move(element));
		m_Size++;
	}
	// Emplace Back 
	template<typename... Args>
	void emplaceBack(Args&&... args) {
		if (m_Size == m_Capacity)
			reAlloc(m_Capacity + m_Capacity / 2);
		new(&m_Buffer[m_Size]) T(std::forward<Args>(args)...);
		m_Size++;
	}
	//Deletes the last element
	void popBack() {
		m_Size--;
		m_Buffer[m_Size].~T();
	}
	//Clears the vector
	void clear() {
		for (size_t i = 0; i < m_Size; i++)
			m_Buffer[i].~T();
		m_Size = 0;

	}
	//Deletes An element at a certain index
	void deleteAt(size_t index) {
		m_Buffer[index].~T();
		//Shifting the other elements
		int  toShift = m_Size - index -1 ;
		for (int i = 0; i < toShift; i++)
			m_Buffer[index + i] = std::move(m_Buffer[index + i + 1]);
		//Reducing the size
		m_Size--;

	}

	//Inserts an element and tries to move it if it is an r-value it
	void insert(size_t index , const T& element){
		if (m_Size == m_Capacity) {
			std::cout << "i was called";
			//We do a special allocation that solves the problem in 1 go
			size_t newCapacity = m_Capacity + m_Capacity / 2;
			T* newBuffer = (T*)::operator new(newCapacity * sizeof(T));
			size_t c = 0;
			for (size_t i = 0; i < m_Size; i++) {
				if (i == index) {
					new(&newBuffer[c]) T(std::move(element));
					c++;
				}
				new(&newBuffer[c]) T(std::move(m_Buffer[i]));
				c++;	
			}
			if (index == m_Size)
				new(&newBuffer[m_Size]) T(std::move(element));

			::operator delete(m_Buffer);
			m_Buffer = newBuffer;
			m_Size++;
		}
		else {
			//Moving the elements 
			for (size_t i = m_Size; i > (index); i--) 
				new(&m_Buffer[i]) T(std::move (m_Buffer[i - 1]));
		
			
			//Inserting the element
			new(&m_Buffer[index]) T(std::move(element));
			m_Size++;

		}
	}
	//Inserts an element Moving  it
	/*
	void insert(size_t index,T&& element) {
		if (m_Size == m_Capacity) {
			std::cout << "i was moved called";
			//We do a special allocation that solves the problem in 1 go
			size_t newCapacity = m_Capacity + m_Capacity / 2;
			T* newBuffer = (T*)::operator new(newCapacity * sizeof(T));
			size_t c = 0;
			for (size_t i = 0; i < m_Size; i++) {
				if (i == index) {
					new(&newBuffer[c]) T(std::move(element));
					c++;
				}
				new(&newBuffer[c]) T(std::move(m_Buffer[i]));
				c++;
			}
			if (index == m_Size)
				new(&newBuffer[m_Size]) T(std::move(element));

			::operator delete(m_Buffer);
			m_Buffer = newBuffer;
			m_Size++;
		}
		else {
			//Moving the elements 
			for (size_t i = m_Size; i > (index); i--)
				new(&m_Buffer[i]) T(std::move(m_Buffer[i - 1]));


			//Inserting the element
			new(&m_Buffer[index]) T(std::move(element));
			m_Size++;

		}
	}
	*/
	
	// Resizes The vector
	void resize(){}

	//Searching Functions
	int linearSearch(const T& target){
		for (int i = 0; i < m_Size; i++) {
			if (m_Buffer[i] == target)
				return i;
		}
		return -1;
	}
	int binarySearch(const T& target){
		size_t lowerBound = 0;
		size_t upperBound = m_Size;
		size_t midPoint;
		while (lowerBound < upperBound) {
			midPoint = (upperBound + lowerBound)/ 2;
			T elementAtMid(m_Buffer[midPoint]);
			if (elementAtMid == target)
				return midPoint;
			else if (elementAtMid > target)
				upperBound = midPoint;
			else
				lowerBound = midPoint;
		}
		return -1;
	}

	//Sorting Functions 
	void selectionSort() {
		for (size_t currentPosition = 0; currentPosition < m_Size - 1; currentPosition++) {
		
			T currentElement = m_Buffer[currentPosition];
			for (size_t c = currentPosition + 1; c < m_Size; c++) {
				T comparedElement = m_Buffer[c];
				if (currentElement > comparedElement) {
					m_Buffer[currentPosition] = comparedElement;
					m_Buffer[c] = currentElement;
					currentElement =comparedElement;

				}
			}
		}
	}
	void selectionSort1() {
		for (size_t currentPosition = 0; currentPosition < m_Size - 1; currentPosition++) {

			T currentElement = m_Buffer[currentPosition];
			size_t comparedPosition = currentPosition + 1;
			T comparedElement = m_Buffer[comparedPosition];
			
			for (size_t c = currentPosition + 2; c < m_Size; c++) {
				T possibleChange = m_Buffer[c];
				if (comparedElement > possibleChange) {
					comparedElement = possibleChange;
					comparedPosition = c;

				}
			}
			if (currentElement > comparedElement) {
				m_Buffer[currentPosition] = comparedElement;
				m_Buffer[comparedPosition] = currentElement;
			}

		}
	}

	

	void bubleSort(){
		bool troca;
		do {
			troca = false;
			for (size_t i = 0; i < m_Size - 1; i++) {
				if (m_Buffer[i] > m_Buffer[i + 1]) {
					troca = true;
					T temporiario(std::move(m_Buffer[i]));
					m_Buffer[i] = std::move(m_Buffer[i + 1]);
					m_Buffer[i + 1] = std::move(temporiario);	
				}


			}
		} while (troca);
	}


	 Vector<T>& mergeSort(){
		// An emplty list or a list that has only one element is already sorted 
		 if (m_Size <= 1)
			 return  *this;
		
		 // Dividing the lists into sublists
		 size_t leftSize = m_Size / 2;
		 size_t rightSize = m_Size - leftSize;
		 Vector<T> left(leftSize);
		 Vector<T> right(rightSize);
		 for (size_t i = 0; i < m_Size; i++) {
			 if (i < leftSize)
				 left.pushBack(std::move(m_Buffer[i]));
			 else
				 right.pushBack(std::move(m_Buffer[i]));
		 }
		 m_Size = 0;
		 
		 left.mergeSort();
		 right.mergeSort();
		 //As listas aqui já estao organizadas logo  fundir duas listas que ja estao organizadas numa é facil
		 size_t rightIndex = 0;
		 size_t leftIndex = 0;
		 while (leftIndex < (left.size()) ||  rightIndex <(right.size()) ) {

			 // Se a lista esquerda estiver vazia completamos a lista com os elementos da direita
			 if (leftIndex >= (left.size()) && rightIndex < (right.size())) {
				 for (rightIndex; rightIndex < right.size(); rightIndex++) {
					 m_Buffer[m_Size] = std::move(right[rightIndex]);
					 m_Size++;
				 }
			 }
			// Se a lista direita estiver vazia completamos a lista com os elementos da esquerda
			 else if (rightIndex >= (right.size()) && leftIndex< (left.size())) {

				 for (leftIndex; leftIndex < left.size(); leftIndex++) {
					 m_Buffer[m_Size] = std::move(left[leftIndex]);
					 m_Size++;
				 }
			 }
			 
			 else if (right[rightIndex] < left[leftIndex]) {
				 m_Buffer[m_Size] = std::move(right[rightIndex]);
				 m_Size++;
				 rightIndex++;
			 }
			 //Se o  item da esquerda  for o menor
			 else if (left[leftIndex] <= right[rightIndex]) {
				new(&m_Buffer[m_Size]) T(std::move(left[leftIndex]));
				m_Size++;
				leftIndex++;
			}
		 }

		 return *this;



	}
	Vector<T>& quickSort(){
		if (m_Size <= 1)
			return *this;
		size_t pivotIndex = m_Size / 2;
		T  pivot = std::move(m_Buffer[pivotIndex]);
		Vector<T> left, right;
		for (size_t i = 0; i < m_Size; i++) {
			if (i != pivotIndex) {
				T compared = std::move(m_Buffer[i]);
				if (compared < pivot)
					left.pushBack(std::move(compared));
				else
					right.pushBack(std::move(compared));
			}
		}
		std::cout << left << " " << right ;
		left.quickSort();
		right.quickSort();
		
		std::cout << left << " " << right;
		size_t thisIndex = 0;
		for (thisIndex; thisIndex < left.size(); thisIndex++)
			m_Buffer[thisIndex] = std::move(left[thisIndex]);
		m_Buffer[thisIndex] = std::move(pivot);
		thisIndex++;

		for (size_t i = 0; i < right.size(); i++) {
			m_Buffer[thisIndex] = std::move(right[i]);
			thisIndex++;
		}

		return *this;
	}

	
	//Indexing Operators (const / mutable)
	const  T& operator[](size_t index) const  {
		return m_Buffer[index];
	}

	T& operator[](size_t index) {
		return m_Buffer[index];
	}

	Iterator  begin() {
		return Iterator(m_Buffer);
	}

	Iterator end() {
		return Iterator(m_Buffer + m_Size);
	}

	// Cout <<  overload
	template<typename T> friend std::ostream& operator<<(std::ostream& out, const Vector<T>& element);
	
	template<typename T> friend void vectorInfo(const  Vector<T>& vector);
};
template<typename T>
std::ostream& operator<<(std::ostream& out,const  std::vector<T>& vec) {
	out << "[";
	if (vec.size() != 0) {
		for (size_t i = 0; i < (vec.size() - 1); i++)
			out << vec[i] << ", ";
		out << vec[vec.size() - 1];
	}
	out << "]";
	return out;
}


template<typename T>
std::ostream& operator<<(std::ostream& out,const Vector<T>& element) {
	out << "[";
	if (element.m_Size != 0) {
		for (size_t i = 0; i < (element.m_Size - 1); i++)
			out << element[i] << ", ";
		out << element[element.m_Size - 1];
	}
	out << "]";
	return out;
}

template<typename T> 
void vectorInfo(const Vector<T>&  vector) {
	std::cout << "------------------------------\n";
	std::cout << "	Vector Info" << std::endl;;
	std::cout << "------------------------------\n";
	std::cout << "Zise: " << vector.m_Size << std::endl;;
	std::cout << "Capacity: " << vector.m_Capacity  << std::endl << " ->";
	std::cout << vector << std::endl;
	
	std::cout << "------------------------------\n";



}