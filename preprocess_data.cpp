#include "preprocess_data.h"

using namespace std;

int** preprocess_data(string filename, long *len ,float REDUCE_DATA) {
	ifstream dataset_file;
	string line;
	long number_of_line = 0;
	bool skip = true;

	dataset_file.open(filename);
	if(dataset_file.is_open()) {
		while(getline(dataset_file,line)) {
			number_of_line++;					//Count number of rows of data
		}
	}
	number_of_line--;							//Subtract 1 because of extra field headings line.
	dataset_file.close();

//2D data array
	int** data = new int*[number_of_line];
	for(int i = 0; i < number_of_line; i++) {
		data[i] = new int[4];
	}

	dataset_file.open(filename);
	if(dataset_file.is_open()) {
		unsigned int reduce_data = 0;
		long col = 0, row = 0;
		while(getline(dataset_file,line) && reduce_data < REDUCE_DATA*number_of_line) {
			if(!skip) {							//Skip field heading
				reduce_data++;
				int i = 0, startpoint, endpoint;
				char array[6];
				while(line[i] != '\0') {
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
						data[row][col] = stoi(array);
						col++;
					}
					i++;
				}
				col = 0;
				row++;
			}
			skip = false;
		}
	}
	*len = (long)number_of_line*REDUCE_DATA;
	dataset_file.close();
	return data;
}

void free_mem(int** data) {
	delete[] data;
}
