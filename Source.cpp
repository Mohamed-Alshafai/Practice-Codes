#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
using namespace std;

// Output functions:
int PartsMenu();
void save_tofile(char** pnumber, char* pclass, int* inventory, double* cost, int num_parts);
void display(char** pnumber, char* pclass, int* inventory, double* cost, int num_parts);
void display_item(char** pnumber, char* pclass, int* inventory, double* cost, int index);

// Input functions:
void readfile(ifstream& ifile, char** pnumber, char* pclass, int* inventory, double* cost, int num_parts);
void getNewPartInput(char* partNum, int& quantity, double& cost, char& classification);

// Manipulation functions:
int findparts(char** pnumber, char* searchtarget, int num_parts);
int* resizeIntArr(int newelement, int* originalArray, int sizeoriginal, int& sizenew);
double* resizeDoubleArr(double newelement, double* originalArray, int sizeoriginal, int& sizenew);
char* resizeCharArr(char newelement, char* originalArray, int sizeoriginal, int& sizenew);
char** resizePartIdArr(char* newelement, char** originalArray, int sizeoriginal, int& sizenew);

int main()
{
	// variable declarations
	int num_parts;
	// open file for reading and check for failure to open
	ifstream ifile("parts.txt");
	if (ifile.fail())
	{
		cout << "Failed to open 'parts.txt'... Exiting!\n";
		exit(1);
	}
	// get number of parts
	ifile >> num_parts;
	// Array declarations
	char** pnumber, * pclass;
	int* inventory;
	double* cost;
	// allocate memory for dyamic arrays
	pnumber = new char* [num_parts];
	for (int i = 0; i < num_parts; ++i)
	{
		pnumber[i] = new char[10];
	}
	pclass = new char[num_parts];
	inventory = new int[num_parts];
	cost = new double[num_parts];
	// call function for reading
	readfile(ifile, pnumber, pclass, inventory, cost, num_parts);
	// close input file
	ifile.close();
	// Start continous loop of options
	int option;
	do
	{
		// get option
		option = PartsMenu();
		// do action
		switch (option)
		{
		case 1: // add a new part
		{
			// memorize current size
			const int ogSize = num_parts;
			// create variables for new part
			char* newPartNum = new char[10];
			int newQuantity;
			double newCost;
			char classification;
			// get new part
			getNewPartInput(newPartNum, newQuantity, newCost, classification);
			// resize arrays
			inventory = resizeIntArr(newQuantity, inventory, ogSize, num_parts);
			cost = resizeDoubleArr(newCost, cost, ogSize, num_parts);
			pclass = resizeCharArr(classification, pclass, ogSize, num_parts);
			pnumber = resizePartIdArr(newPartNum, pnumber, ogSize, num_parts);
			// delete array
			delete[]newPartNum;
			// end of function
			break;
		}

		case 2: // view all parts
		{
			// call display function
			display(pnumber, pclass, inventory, cost, num_parts);
			break;
		}

		case 3: // search for a part
		{
			// create variable for part number
			char* searchPartNum = new char[10];
			// enter part number
			cout << "Enter part number: ";
			cin >> searchPartNum;
			// get the index
			int index = findparts(pnumber, searchPartNum, num_parts);
			// if it exists
			if (index > -1)
			{
				// display part
				display_item(pnumber, pclass, inventory, cost, index);
			}
			// otherwise output error message
			else cout << "Part not found!\n";
			break;
		}

		case 4: // quit
		{
			// save parts to file
			save_tofile(pnumber, pclass, inventory, cost, num_parts);
			break;
		}

		default: // invalid options
			cout << "Invalud option.... Try again!\n";
			break;
		}
	} while (option != 4); // exist condition
	// free dynamic memory;
	for (int i = 0; i < num_parts; ++i)
	{
		delete[]pnumber[i];
	}
	delete[]pnumber;
	pnumber = nullptr;
	delete[]pclass;
	pclass = nullptr;
	delete[]inventory;
	inventory = nullptr;
	delete[]cost;
	cost = nullptr;
	// end of program
	return 0;
}

/*
* This function displays the available option for the program
* It takes no paramters as input and returns an integer for the option the user picks
*/
int PartsMenu()
{
	// create variable to hold option
	int option;
	// menu list
	cout << "1) Add new part\n"
		<< "2) View all parts\n"
		<< "3) Search for a part\n"
		<< "4) Quit\n"
		<< "Enter option: ";
	// get option
	cin >> option;
	return option;
}

/*
* The function is used to extract the part numbers and their info from the "part.txt" file and store them in the proper array
* It takes an ifstream variable for input, a character matrice for part ID, 
* a character pointer for part type, an int pointer for the number of the specific part, 
* a double pointer for the cost, and an int for the number of unique parts
* and returns nothing as they're passed by reference and not value
*/
void readfile(ifstream& ifile, char** pnumber, char* pclass, int* inventory, double* cost, int num_parts)
{
	// fill arrays
	for (int p = 0; p < num_parts; ++p)
	{
		//		 part #,      part class,   # of parts,     cost of part
		ifile >> pnumber[p] >> pclass[p] >> inventory[p] >> cost[p];
	}
}

/*
* The function is used to enter a new part to the existing list of parts. It asks the user for input by printing 
* a message to the screen prompting them to type the specific input needed.
* It takes a char pointer for the part ID, and a reference to an int for the quantity of that part, 
* a double reference for the cost/part, and a char reference for the type of that part as parameters
* and returns nothing.
*/
void getNewPartInput(char* partNum, int& quantity, double& cost, char& classification)
{
	// get new part's info
	cout << "Enter part number: ";
	cin >> partNum;
	cout << "Enter classification of part [" << partNum << "]: ";
	cin >> classification;
	while (classification < 'A' || classification > 'C')
	{
		cout << "Invalid classification... Try again!\n";
		cout << "Enter classification of part [" << partNum << "]: ";
		cin >> classification;
	}
	cout << "Enter quantity of part [" << partNum << "]: ";
	cin >> quantity;
	cout << "Enter cost of part[" << partNum << "]: ";
	cin >> cost;
}

/*
* This function is used to output the current parts in inventory and their information when quiting the program.
* It takes in a character matrice for part ID, a character pointer for part type,
* an int pointer for the number of the specific part, a double pointer for the cost,
* and an int for the number of unique parts. And it returns nothing.
*/
void save_tofile(char** pnumber, char* pclass, int* inventory, double* cost, int num_parts)
{
	// open file for input
	ofstream ofile("parts.txt");
	// output number of parts
	ofile << num_parts << endl;
	// output parts info
	for (int p = 0; p < num_parts; ++p)
	{
		ofile << pnumber[p] << ' '
			<< pclass[p] << ' '
			<< inventory[p] << ' '
			<< cost[p] << endl;
	}
	// close file
	ofile.close();
}

/*
* This function is used to find a specific part's index by its part number.
* It takes in a a char matrice for the part numbers, a char pointer for the target of the search, and the number
* of parts in inventory and return the index if the part was found or -1 if it wasn't found.
*/
int findparts(char** pnumber, char* searchtarget, int num_parts)
{
	// loop through parts in array
	for (int p = 0; p < num_parts; ++p)
	{
		// compare parts
		if (strcmp(pnumber[p], searchtarget) == 0)
			return p; // return part number if it matches
	}
	// return -1 if no match is found
	return -1;
}

/*
* This function is used to resize the int array for the quantity of a part.
* It takes in an int of the quantity of the new element to add, the int array with the original values,
* an int with the current number of parts, and an int with the required new number.
* It returns the new, resized int array in the end.
*/
int* resizeIntArr(int newelement, int* originalArray, int sizeoriginal, int& sizenew)
{
	// create 1D dynamic array
	int* resizedArray = new int[sizeoriginal + 1];
	// move old values to new array
	for (int i = 0; i < sizeoriginal; ++i)
	{
		resizedArray[i] = originalArray[i];
	}
	// add new values
	resizedArray[sizeoriginal] = newelement;
	// delete old array
	delete[]originalArray;
	// store new size
	sizenew = sizeoriginal + 1;
	// return new array
	return resizedArray;
}

/*
* This function is used to resize the array of type double for the price of a part.
* It takes in a double for the new element's price to be added, the array of type double with the original values,
* an int with the current number of parts, and an int with the required new number.
* It returns the new, resized array of type double in the end.
*/
double* resizeDoubleArr(double newelement, double* originalArray, int sizeoriginal, int& sizenew)
{
	// create 1D dynamic array
	double* resizedArray = new double[sizeoriginal + 1];
	// move old values to new array
	for (int i = 0; i < sizeoriginal; ++i)
	{
		resizedArray[i] = originalArray[i];
	}
	// add new values
	resizedArray[sizeoriginal] = newelement;
	// delete old array
	delete[]originalArray;
	// store new size
	sizenew = sizeoriginal + 1;
	// return new array
	return resizedArray;
}

/*
* This function is used to resize the char array for the type of a part.
* It takes in a char for the type of the new element to add, the char array with the original values,
* an int with the current number of parts, and an int with the required new number.
* It returns the new, resized char array in the end.
*/
char* resizeCharArr(char newelement, char* originalArray, int sizeoriginal, int& sizenew)
{
	// create 1D dynamic array
	char* resizedArray = new char[sizeoriginal+1];
	// move old values to new array
	for (int i = 0; i < sizeoriginal; ++i)
	{
		resizedArray[i] = originalArray[i];
	}
	// add new values
	resizedArray[sizeoriginal] = newelement;
	// delete old array
	delete[]originalArray;
	// store new size
	sizenew = sizeoriginal + 1;
	// return new array
	return resizedArray;
}

/*
* This function is used to resize the char matrice for the part numbers.
* It takes in a char pointer for the number of the new element to add, the char matrice with the original values,
* an int with the current number of parts, and an int with the required new number.
* It returns the new, resized char matrice in the end.
*/
char** resizePartIdArr(char* newelement, char** originalArray, int sizeoriginal, int& sizenew)
{
	// create 2D dynamic array
	char** resizedArr = new char* [sizeoriginal + 1];
	// allocate memory
	for (int i = 0; i < sizenew; ++i)
	{
		resizedArr[i] = new char[10];
	}
	// move old values to new array
	for (int i = 0; i < sizeoriginal; ++i)
	{
		strcpy(resizedArr[i], originalArray[i]);
	}
	// add new values
	strcpy(resizedArr[sizeoriginal], newelement);
	// delete old array
	for (int i = 0; i < sizeoriginal; ++i)
	{
		delete[]originalArray[i];
	}
	delete[]originalArray;
	// store new size
	sizenew = sizeoriginal + 1;
	// return new array
	return resizedArr;
}

/*
* This function is used to dislpay the current parts and their information, along with the total cost
* of the inventory and the total cost per part, to the console for the user. It takes in a character matrice for 
* part numbers, a character pointer for part types, an int pointer for the quantity of the each part, 
* a double pointer for the cost of each part, and an int for the number of unique parts and returns nothing
*/
void display(char** pnumber, char* pclass, int* inventory, double* cost, int num_parts)
{
	// create variable to hold sum
	double sum = 0;
	// output formatting
	cout << "---------------------------------------------------------------------------------------------------------\n"
		<< "|Part ID\t|\t" << "Class\t|" << "Inventory\t|\t" << "Cost\t\t|\t" << "Total Cost\t\t|\n"
		<< "---------------------------------------------------------------------------------------------------------\n";
	for (int p = 0; p < num_parts; ++p)
	{
		// output parts & formatting
		cout << '|' << pnumber[p] << "   \t|\t"
			<< pclass[p] << "\t|"
			<< inventory[p] << "\t\t|\t"
			<< cost[p] << "\t\t|\t"
			<< (cost[p] * inventory[p]) << "\t\t\t|" << endl;
		sum += (cost[p] * inventory[p]);
	}
	// output sum & formatting
	cout << "---------------------------------------------------------------------------------------------------------\n"
		<< "|Total cost of the inventory is: $" << sum << "\t\t\t\t\t\t\t\t|"
		<< "\n---------------------------------------------------------------------------------------------------------\n" << endl;
}

/*
* This function is used to dislay the specified part's information in a paragraph format. It takes in a 
character matrice for part numbers, a character pointer for part types, an int pointer for the quantity of
* each part, a double pointer for the cost of each part, and an int for the specified part and return nothing
*/
void display_item(char** pnumber, char* pclass, int* inventory, double* cost, int index)
{
	// output specific item & formatting
	cout << "Available quantity of Part Number '" << pnumber[index] << "' is " << inventory[index] << "pcs. "
		<< "The part belongs to class '" << pclass[index] << "'. \n"
		<< "It costs $" << cost[index] << "/part. "
		<< "The value of total parts in the invnetory is $" << (cost[index] * inventory[index]) << '.' << endl;
}


//int menu();
//void newPart(vector<double>& ppricevec, vector<char*>& pnumbervec);
//void dispPart(vector<double>& ppricevec, vector<char*>& pnumbervec);
//void delPart(vector<double>& ppricevec, vector<char*>& pnumbervec);
//int main()
//{
//	vector<double> ppricevec;
//	vector<char*>pnumbervec;
//	int option;
//	do
//	{
//		option = menu();
//
//		switch (option)
//		{
//		case 1:
//			newPart(ppricevec, pnumbervec);
//			break;
//
//		case 2:
//			dispPart(ppricevec, pnumbervec);
//			break;
//
//		case 3:
//			if (ppricevec.size() > 0)
//			{
//				delPart(ppricevec, pnumbervec);
//			}
//			else
//			{
//				cout << "No items to delete!" << endl;
//			}
//			break;
//		case 4:
//			break;
//		default:
//			cout << "Invalid option" << endl;
//		}
//	} while (option != 4);
//
//	for (int i = 0; i < pnumbervec.size(); ++i)
//	{
//		delete[]pnumbervec[i];
//	}
//	pnumbervec.clear();
//
//	return 0;
//}
//
//int menu()
//{
//	int option;
//	cout << "1.Enter new part\n"
//		<< "2.Display parts\n"
//		<< "3.Delete part\n"
//		<< "4.Exit\n"
//		<< "Enter Option: ";
//	cin >> option;
//
//	return option;
//}
//
//void newPart(vector<double>& ppricevec, vector<char*>& pnumbervec)
//{
//	int numelements;
//	double newPrice;
//
//	cout << "How many elements: ";
//	cin >> numelements;
//
//	for (int i = 0; i < numelements; ++i)
//	{
//		char* newID = new char[10];
//		cout << "Enter part [" << i << "] " << "ID: ";
//		cin >> newID;
//
//		cout << "Enter part [" << i << "] " << "price: ";
//		cin >> newPrice;
//
//		pnumbervec.push_back(newID);
//		ppricevec.push_back(newPrice);
//	}
//}
//
//void dispPart(vector<double>& ppricevec, vector<char*>& pnumbervec)
//{
//	cout << "List of parts...." << endl;
//	for (int i = 0; i < pnumbervec.size(); ++i)
//	{
//		cout << "Index [" << i << "]: " << ppricevec[i] << ", " << pnumbervec[i] << endl;
//	}
//}
//
//void delPart(vector<double>& ppricevec, vector<char*>& pnumbervec)
//{
//
//	int delpart;
//	cout << "Select which part from the following:" << endl;
//	dispPart(ppricevec, pnumbervec);
//	cout << "to delete by its index: ";
//	cin >> delpart;
//	ppricevec.erase(ppricevec.begin() + delpart);
//	delete[]pnumbervec[delpart];
//	pnumbervec.erase(pnumbervec.begin() + delpart);
//}

