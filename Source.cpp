#include<iostream>
#include<limits>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

enum MENU { PRINT = 1, QUIT };

const int MAX_SIZE = 65; // Max string length
const int WIDTH = 66; // Column Width

class CharStack {
private:
	char str[MAX_SIZE];
	int front;

public:
	CharStack() { front = -1; }
	void push(char c);
	void pop();
	char peek() { return str[front]; }

	bool isFull() { return (front == MAX_SIZE - 1); }
	bool isEmpty() { return (front == -1); }
	void clear() { front = -1; }
};

int menuSelection(string menu_options);
void purgeInputErrors(string err_mess);
string processData(string line);
void parseData(string file_name, stringstream& header);
bool isValidLength(string str);
void print(string line, const string& reversed, ofstream& outfile);

int main() {
	string file_name = "Data.txt";
	string menu_options = "1. Print Reverse\n2. Quit Program\nSelection: ";
	int selection{ 0 };
	stringstream header;
	header << left << setw(WIDTH) << "Input" << setw(WIDTH) << "Reversed" << endl;

	cout << "Welcome User\n";
	
	do {
		selection = menuSelection(menu_options);
		switch (selection) {
		case PRINT:
			parseData(file_name, header);
			break;
		case QUIT:
			cout << "\nTerminating Program\n";
			break;
		default:
			purgeInputErrors("\nError: Invalid Selection Option\n");
			break;
		}
	} while (selection != QUIT);

	return 0;
}

void CharStack::push(char c) {
	if (isFull()) {
		cout << "ERROR: Stack Overflow!\nPlease pop existing values before pushing new values in\n\n";
	}
	else {
		str[++front] = c;
	}
}

void CharStack::pop() {
	if (isEmpty()) {
		cout << "ERROR: Empty Stack!\nThere are no values in the stack to pop\n\n";
	}
	else {
		--front;
	}
}

int menuSelection(string menu_options) {
	int selection;
	cout << "\nPlease select one of the following:\n" << menu_options;
	cin >> selection;
	return selection;
}

void purgeInputErrors(string err_mess) {
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << err_mess;
}

string processData(string line) {
	CharStack stack;
	string reversed{ "" };
	bool apostrophe_encountered = false;

	if (isValidLength(line)) { // Check string length
		for (int i{ 0 }; i < line.size(); i++) {
			// When an apostrophe is encountered, set the flag equal to true until a white space is encountered
			if (line[i] == '\'') {
				apostrophe_encountered = true;
			}
			if (apostrophe_encountered == true && line[i] == ' ') { // if we've already encountered an apostrophe and the current character is a space, reset the flag
				apostrophe_encountered = false;
			}
			if (apostrophe_encountered == false && isalpha(line[i])) {
				stack.push(tolower(line[i]));
			}
		}
		while (!stack.isEmpty()) {
			reversed += stack.peek();
			stack.pop();
		}
	}
	return reversed;
}

void parseData(string file_name, stringstream& header) {
	int valid_count{ 0 };
	int total_count{ 0 };
	stringstream restrictions;
	restrictions << "\n-Lines exceeding " << MAX_SIZE << " characters will not be processed"
				 << "\n-Lines containing only non-alphabetical characters will not be processed"
				 << "\n-Characters following an apostrophe in a given word on a given line "
					"will be ignored until either the next word or next line is encountered\n\n"
				 << "--- Please ensure that the " << file_name << " adheres to these restrictions before attempting to print the data ---\n";

	ifstream infile(file_name);
	if (!infile) {
		cout << "Error: Unable to open " << file_name << " file.\nTerminating Program\n";
		exit(EXIT_FAILURE);
	}
	else if (infile.peek() == EOF) {
		cout << "Error: " << file_name << " file is empty. No data to process.\nTerminating Program\n";
		infile.close();
		exit(EXIT_FAILURE);
	}

	ofstream outfile("Results.txt");
	if (!outfile) {
		cout << "Unable to create output file.\nTerminating Program\n";
		infile.close();
		exit(EXIT_FAILURE);
	}

	string line{ "" };
	CharStack stack;
	
	// Push valid characters onto the stack
	while (getline(infile, line)) {
		string reversed = processData(line);
		total_count++;
		if (!reversed.empty()) {
			if (valid_count == 0) {
				cout << "\nData Successfully Printed to " << file_name << "\n\n" << header.str();
				outfile << header.str();
			}
			print(line, reversed, outfile);
			valid_count++;
		}
	}

	if (valid_count == 0) {
		cout << "\n--- The data file " << file_name << " does not contain any valid data ---\n" << restrictions.str();
		outfile << "\n--- The data file " << file_name << " does not contain any valid data ---\n" << restrictions.str();
	}
	else {
		cout << "\nTotal Lines In Data File: " << total_count
			 << "\nTotal Lines Processed: " << valid_count << "\n";

		outfile << "\nTotal Lines In Data File: "  << total_count
				<< "\nTotal Lines Processed: " << valid_count << "\n";
	}

	infile.close();
	outfile.close();
}

bool isValidLength(string str) {
	return str.size() <= MAX_SIZE;
}

void print(string line, const string& reversed, ofstream& outfile) {
	cout << left << setw(WIDTH) << line;
	outfile << left << setw(WIDTH) << line;

	cout << setw(WIDTH) << reversed << "\n";
	outfile << setw(WIDTH) << reversed << "\n";
}

/*
---------------RUN PROGRAM THEN COPY PASTE OUTPUT HERE---------------

Test Data:

YES!!! you can dance! (Valid and Invalid Characters, Mixed Cases)
Matching cover pAGE, header, and sidebar. (Valid and Invalid Characters, Mixed Cases)
cat (Valid Lowercase Characters)
hello@123 (Valid and Invalid Characters, Lowercase)
apple banana cherry (Strings With Spaces)
AbCdEfGhIjKlMnOpQrStUvWxYz (Alphabetical Characters, Mixed Cases)
a (Single Valid Character)
& (Single Invalid Character)
   123   abc     (Line With Leading and Trailing Spaces)
lets'sagooo (Apostrophe In The Middle Of A Word)
I'm okay M'lady (Apostrophes, Mixed Cases, Spaces)
Shouldn't've 'om' 
&invalid!@characters# (Valid and Invalid Characters, Mixed Cases)
%!@#$%^&*()_-+=[]{}|;:'",.<>?/ (ONLY Invalid Characters)
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmn (Line > Max String Length)
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklm (Line = Max String Length)


*****Test Case #1: Special Characters************************

Input                                                             Reversed
hello@123                                                         olleh
&invalid!@characters#                                             sretcarahcdilavni

Total Lines In Data File: 2
Total Lines Processed: 2


*****Test Case #2: String Length Exceeds MAX_SIZE************

Please select one of the following:
1. Print Reverse
2. Quit Program
Selection: 1

--- The data file Data.txt does not contain any valid data ---

-Lines exceeding 65 characters will not be processed
-Lines containing only non-alphabetical characters will not be processed
-Characters following an apostrophe in a given word on a given line will be ignored until either the next word or next line is encountered

--- Please ensure that the Data.txt adheres to these restrictions before attempting to print the data ---


*****Test Case #3: String Length Equals MAX_SIZE*************

Input                                                             Reversed
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklm mlkjihgfedcbazyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcba

Total Lines In Data File: 1
Total Lines Processed: 1


*****Test Case #4: Valid Characters, Invalid Length**********

Please select one of the following:
1. Print Reverse
2. Quit Program
Selection: 1

--- The data file Data.txt does not contain any valid data ---

-Lines exceeding 65 characters will not be processed
-Lines containing only non-alphabetical characters will not be processed
-Characters following an apostrophe in a given word on a given line will be ignored until either the next word or next line is encountered

--- Please ensure that the Data.txt adheres to these restrictions before attempting to print the data ---


*****Test Case #5: File Contains ONLY Invalid Characters*****

Please select one of the following:
1. Print Reverse
2. Quit Program
Selection: 1

--- The data file Data.txt does not contain any valid data ---


*****Test Case #6: Strings With Empty Spaces*****************

Input                                                             Reversed
YES!!! you can dance!                                             ecnadnacuoysey
Matching cover pAGE, header, and sidebar.                         rabedisdnaredaehegaprevocgnihctam
I'm okay M'lady                                                   myakoi

Total Lines In Data File: 3
Total Lines Processed: 3


*****Test Case #7: Strings With Leading and Trailing Spaces**

Input                                                             Reversed
   123   abc                                                      cba

Total Lines In Data File: 1
Total Lines Processed: 1


*****Test Case #8: Single Valid Character********************

Input                                                             Reversed
a                                                                 a

Total Lines In Data File: 1
Total Lines Processed: 1


*****Test Case #9: Single Invalid Character******************

Please select one of the following:
1. Print Reverse
2. Quit Program
Selection: 1

--- The data file Data.txt does not contain any valid data ---


*****Test Case #10: Single Word With Apostrophe***************

Input                                                             Reversed
lets'sagooo                                                       stel

Total Lines In Data File: 1
Total Lines Processed: 1


*****Test Case #11: Multiple Words With Spaces AND Apostrophes

Input                                                             Reversed
I'm okay M'lady                                                   myakoi
Shouldn't've 'om'                                                 ndluohs

Total Lines In Data File: 2
Total Lines Processed: 2


*****Test Case #12: Empty Input File**************************

Error: Data.txt file is empty. No data to process. 
Terminating Program


*****Test Case #13: Unable To Open File***********************

Error: Unable to open Data.txt file. 
Terminating Program


*****Test Case #14: Invalid Menu Selection********************

Please select one of the following:
1. Print Reverse
2. Quit Program
Selection: asdf

Error: Invalid Selection Option

Please select one of the following:
1. Print Reverse
2. Quit Program
Selection:


*/