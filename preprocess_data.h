#ifndef PREPROP_H
#define PREPROP_H

#include <iostream>
#include <fstream>
#include <string>

#define MARITAL_COMMA 5
#define AGEWED_COMMA 7
#define DIVORCE_COMMA 10
#define WIDOWED_COMMA 12

enum event {
	no_event = -1, marital = 0, agewed = 1, ever_div = 2, ever_wid = 3
};

enum attribute {
	married = 1,
	widowed = 2,
	divorced = 3,
	separated = 4,
	never_married = 5
};

int** preprocess_data(std::string filename, long *len ,float reductionRatio);
void free_mem(int** data);
#endif
