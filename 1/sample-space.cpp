#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define MARITAL_COMMA 5
#define AGEWED_COMMA 7
#define DIVORCE_COMMA 10
#define WIDOWED_COMMA 12

using namespace std;
enum {
	marital,agewed,divorce,widowed
};

int main(void) {
	ifstream dataset_file;
	string line;
	unsigned int number_of_line = 0;
	bool skip = true;

	dataset_file.open("../sub-data.txt");
	if(dataset_file.is_open()) {
		while(getline(dataset_file,line)) {
			number_of_line++;					//Count number of rows of data
		}
	}
	number_of_line--;							//Subtract 1 because of extra field headings line.

	int data[number_of_line][4];
	if(dataset_file.is_open()) {
		while(getline(dataset_file,line)) {
			int col = 0, row = 0;
			if(!skip) {							//Skip field heading
				skip = false;
				int i = 0, startpoint, endpoint;
				char array[6];
				while(line[i] != '\n') {
					if(line[i] == ',') {
						if (i == MARITAL_COMMA) {
							startpoint = MARITAL_COMMA + 1;
							endpoint = startpoint;
						} else if (i == AGEWED_COMMA) {
							startpoint = AGEWED_COMMA + 1;
							endpoint = startpoint + 1;						
						} else if (i == DIVORCE_COMMA) {
							startpoint = DIVORCE_COMMA + 1;
							endpoint = startpoint;							
						} else if (i == WIDOWED_COMMA) {
							startpoint = WIDOWED_COMMA + 1;
							endpoint = startpoint;							
						}
						size_t len = line.copy(array,endpoint - startpoint + 1,startpoint);
						array[len] = '\0';
						sstream cnvtr(array);
						cnvtr >> data[row][col];
						col++;
					}
					i++;
				}
				col = 0;
				row++;
			}
		}
	}	
	cout << number_of_line << endl;
	dataset_file.close();
	return 0;
}