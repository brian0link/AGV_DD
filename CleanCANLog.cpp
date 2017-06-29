// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

string CleanLine(string);

const int EMPTYSPACES = 11;

void main() {
	remove("C:\\Users\\bwalsh\\Downloads\\cleanedoutputs.txt");

	string line;
	ifstream myfile("C:\\Users\\bwalsh\\Downloads\\displayoutputs.txt");
	ofstream outfile("C:\\Users\\bwalsh\\Downloads\\cleanedoutputs.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			outfile << CleanLine(line);
		}
		myfile.close();
	}

	else cout << "Unable to open file" << endl;
}

string CleanLine(string input) {
	string output = input;

	if ((output[EMPTYSPACES] != ' ')) {
		output =  "";
	} else {
		output.erase(output.begin(), output.begin() + EMPTYSPACES + 1);
		output = output + "\n";
	}

	return output;
}