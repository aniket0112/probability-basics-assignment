/*
 * Program to run and compute several terminologies of probability
 * Run the code and follow the instructions provided in console.
 * */
#define PLOT 1		//change to 0 to not use gnuplot
#define WIN 0		//change to 1 for Windows

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "preprocess_data.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

long dataset_len;
int** dataset;

struct randomVariable {
	double probability = 0;
	long occurence = 0;
	long outcome = 0;
};

struct expectedValue {
	double exp = 0;		//First moment
	double exp_2 = 0;	//Second momment
};

//Function to check valid data row
bool check_validity(enum event event1,int data1) {
	bool flag = true;
	if(event1 == marital) {
		if(data1 == 9) {
			flag = false;
		}
	} else if(event1 == agewed) {
		if(data1 == 98 || data1 == 99 || data1 == 0) {
			flag = false;
		}
	} else if(event1 == ever_div || event1 == ever_wid) {
		if(data1 == 0 || data1 == 8 || data1 == 9) {
			flag = false;
		}
	}
	return flag;
}

//Funtion to find probability/conditional probability of a random variable
struct randomVariable probability(enum event event_a, int value_a,enum event event_b, int value_b) {
	struct randomVariable RV;
	bool do_not_use_b = false;
	if(event_b < 0 || value_b < 0) {
		do_not_use_b = true;
	}
	for(long i = 0; i < dataset_len; i+=1) {
		if(check_validity(event_a,dataset[i][event_a]) && check_validity(event_b,dataset[i][event_b])) {
			if(dataset[i][event_a] == value_a && (do_not_use_b || dataset[i][event_b] == value_b)) {
				RV.occurence++;
			}
			RV.outcome++;
		}
	}
	RV.probability = (double)RV.occurence/RV.outcome;
	return RV;
}


//Returns expected value of random variable
struct expectedValue expected_value(enum event eventa,int minv, int maxv,
					enum event eventb, int valueb)
{
	struct randomVariable r;
	struct expectedValue e;
	for(int i = minv; i <= maxv; i++) {
		 r = probability(eventa,i,eventb,valueb);
		 e.exp += r.probability*i;
		 e.exp_2 += r.probability*i*i;
	}
	double p = probability(eventb,valueb,eventa,-1).probability;
	e.exp /= p;
	e.exp_2 /= p;
	return e;
}


//Pipeline to gnuplot for plotting graphs
void gnuplot(double xmin, double xmax, double ymin, double ymax,char* filename) {
#if PLOT==1
	FILE *gnuplotPipe = popen("gnuplot", "w");  // Open a pipe to gnuplot
	if (gnuplotPipe) {   // If gnuplot is found
		fprintf(gnuplotPipe, "reset\n"); //gnuplot commands
		fprintf(gnuplotPipe,"set xrange [%f:%f]\nset yrange [%f:%f]\n",xmin,xmax,ymin,ymax);
		fprintf(gnuplotPipe,"set boxwidth 0.5\n");
		fprintf(gnuplotPipe,"set style fill solid\n");
		fprintf(gnuplotPipe,"set term png\n set output '%s'\n",filename);
		fprintf(gnuplotPipe, "plot 'plotdata.dat' using 1:3:xtic(2) with boxes title 'PDF'\n");
		fprintf(gnuplotPipe,"set term x11\n");
		fprintf(gnuplotPipe, "plot 'plotdata.dat' using 1:3:xtic(2) with boxes title 'PDF'\n");
		fflush(gnuplotPipe); //flush pipe
		cout << "Press any key to exit GNUplot";
		getchar();
		getchar();
		fprintf(gnuplotPipe,"exit \n");   // exit gnuplot
		pclose(gnuplotPipe);    //close pipe
	}
#endif
}

//driver function
int main(void) {
	dataset = preprocess_data("sub-data.txt",&dataset_len,1.0);
	struct randomVariable RV_m = probability(marital,married,agewed,-1);
	struct randomVariable RV_w = probability(marital,widowed,agewed,-1);
	struct randomVariable RV_d = probability(marital,divorced,agewed,-1);
	struct randomVariable RV_s = probability(marital,separated,agewed,-1);
	struct randomVariable RV_o = probability(marital,never_married,agewed,-1);

	//gui code here
	here :
#if WIN == 0
	system("clear");
#elif WIN == 1
	system("cls");
#endif
	unsigned int choice;
	cout << "For a given age of a person, the sample space about the current marital status is \n{Married, Widowed, Divorced, Separated, Never Married}" << endl;
	cout << "Let them be denoted by :\nMarried = M\nWidowed = W\nDivorced = D\nSeparated = S\nNever Married = O\n"<<endl;
	cout << endl;
	cout << "Type in the option number which you want to proceed and press Enter:"<<endl;
	cout << "1. Disjoint, Exhaustive and Complementary events" << endl;
	cout << "2. Probability of a person's current marital status" << endl;
	cout << "3. Conditional Probability of a person's marital status for a given age:"<<endl;
	cout << "4. Random variable:"<<endl;
	cout << "5. PDF of marital status:"<<endl;
	cout << "6. CDF of marital status:"<<endl;
	cout << "7. Expected value of age for each marital status:"<<endl;
	cout << "8. Standard deviation of age for each marital status:"<<endl;
	cout << "9. Gaussian/Normal distribution of age for married, marital status"<<endl;
	cout << "0. Exit" <<endl;
	cin >> choice;

	//Choice decision ladder
	if(choice == 0) {		//Exit invoked
		free_mem(dataset);
		return 0;
	} else {
		if(choice == 1) {
			//disjoint event
			cout << "All the events in the sample space are disjoint as a person cannot have two marital statuses simultaneously. A(intersection)B = (null)"<<endl;
			//exhaustive event
			cout << "All the events in the sample space are exhaustive as union of all events(Married, Widowed, Divorced ...) gives sample set. union{M,W,D,S,O} = S. It wouldn't have been exhaustive in the original sample set because there are entries of N.A. which have been removed from sample space."<<endl;
			//Complementary event
			cout << "Complementary events are union{Married, Widowed, Divorced, Separated} and {Never Married}.\nP(O) = 1-P(union{M,W,D,S})" << endl;
		} else if(choice == 2) {
			cout <<"P(M) = "<< RV_m.probability << endl;
			cout <<"P(W) = "<< RV_w.probability << endl;
			cout <<"P(D) = "<< RV_d.probability << endl;
			cout <<"P(S) = "<< RV_s.probability << endl;
			cout <<"P(O) = "<< RV_o.probability << endl;
			cout <<"P(M)+P(W)+P(D)+P(S)+P(O) = "<< RV_m.probability + RV_w.probability + RV_d.probability + RV_s.probability + RV_o.probability << endl;
			cout <<"NOTE : All the probabilities are calculated for outcomes with valid data availablity in corresponding Age of Wedding field.";
		} else if (choice == 3) {
			int age;
			cout << "Enter an age to find conditional probability of a person's marital status for a given age : ";
			cin >> age;
			struct randomVariable PA = probability(agewed,age,agewed,-1);
			cout << "P(M|A) = " << probability(marital,married,agewed,age).probability/PA.probability << endl;
			cout << "P(W|A) = " << probability(marital,widowed,agewed,age).probability/PA.probability << endl;
			cout << "P(D|A) = " << probability(marital,divorced,agewed,age).probability/PA.probability << endl;
			cout << "P(S|A) = " << probability(marital,separated,agewed,age).probability/PA.probability << endl;
			cout << "P(O|A) = " << probability(marital,never_married,agewed,age).probability/PA.probability << endl;
		} else if(choice == 4) {
			cout << "Choose random variable to find probability and value :"<<endl;
			cout << "1. X = Number of married people"<<endl;
			cout << "2. X = Number of widowed people"<<endl;
			cout << "3. X = Number of divorced people"<<endl;
			cout << "4. X = Number of separated people"<<endl;
			cout << "5. X = Number of never married people"<<endl;
			int X;
			cin >> X;
			struct randomVariable r = probability(marital,(int)X,agewed,-1);
			cout << "P(X) = " << r.probability << endl;
			cout << "X = " << r.occurence << "of total outcome : "<<r.outcome <<endl;
		} else if(choice == 5) {
			ofstream plotdata;		// file to store data points to plot
			plotdata.open("plotdata.dat");
			plotdata <<"1 married "<<RV_m.probability<<endl;
			plotdata <<"2 widowed "<<RV_w.probability<<endl;
			plotdata <<"3 divorced "<<RV_d.probability<<endl;
			plotdata <<"4 separated "<<RV_s.probability<<endl;
			plotdata <<"5 never_married "<<RV_o.probability<<endl;
			plotdata.close();
			gnuplot(0,6,0,1.5,"pdf");			//Plot graph function using gnuplot and read from file plotdata.dat
		} else if(choice == 6) {
			ofstream plotdata;		// file to store data points to plot
			plotdata.open("plotdata.dat");
			plotdata <<"1 mar "<<RV_m.probability<<endl;
			plotdata <<"2 wid/mar "<<RV_w.probability+RV_m.probability<<endl;
			plotdata <<"3 div/wid/mar "<<RV_d.probability+RV_w.probability+RV_m.probability<<endl;
			plotdata <<"4 sep/div/wid/mar "<<RV_s.probability+RV_d.probability+RV_w.probability+RV_m.probability<<endl;
			plotdata <<"5 samplespace "<<RV_o.probability+RV_s.probability+RV_d.probability+RV_w.probability+RV_m.probability<<endl;
			plotdata.close();
			gnuplot(0,6,0,1.5,"cdf");			//Plot graph function using gnuplot and read from file plotdata.dat
		} else if(choice == 7) {
			cout << "E(Married_AgeWed) = " << expected_value(agewed,1,98,marital,married).exp <<endl;
			cout << "E(Widowed_AgeWed) = " << expected_value(agewed,1,98,marital,widowed).exp <<endl;
			cout << "E(Divorced_AgeWed) = " << expected_value(agewed,1,98,marital,divorced).exp <<endl;
			cout << "E(Separated_AgeWed) = " << expected_value(agewed,1,98,marital,separated).exp <<endl;
			cout << "E(Never_Married_AgeWed) = " << expected_value(agewed,1,98,marital,never_married).exp <<endl;
		} else if(choice == 8) {
			struct expectedValue e;
			e = expected_value(agewed,1,98,marital,married);
			cout << "SD(Married_AgeWed) = " <<sqrt(e.exp_2 - e.exp*e.exp) <<endl;
			e = expected_value(agewed,1,98,marital,widowed);
			cout << "SD(Widowed_AgeWed) = " <<sqrt(e.exp_2 - e.exp*e.exp) <<endl;
			e = expected_value(agewed,1,98,marital,divorced);
			cout << "SD(Divorced_AgeWed) = " <<sqrt(e.exp_2 - e.exp*e.exp) <<endl;
			e = expected_value(agewed,1,98,marital,separated);
			cout << "SD(Separated_AgeWed) = " <<sqrt(e.exp_2 - e.exp*e.exp) <<endl;
			e = expected_value(agewed,1,98,marital,never_married);
			cout << "SD(Never_Married_AgeWed) = " <<sqrt(e.exp_2 - e.exp*e.exp) <<endl;
		} else if(choice == 9) {
			ofstream plotdata;		// file to store data points to plot
			plotdata.open("plotdata.dat");
			struct randomVariable RV;
			long max = 0;
			for(int i = 1; i < 97; i++) {
				RV = probability(marital,married,agewed,i);
				plotdata <<i<<" "<<i<<" "<<RV.occurence<<endl;
				if(max < RV.occurence) {
					max = RV.occurence;
				}
			}
			plotdata.close();
			gnuplot(1,98,0,max+1,"gaussian");			//Plot graph function using gnuplot and read from file plotdata.dat
		}
	}	
	cout << endl;
	cout << "Press any key to continue....";
	getchar();
	getchar();
	cout << "=========================================================" <<endl;
	cout << endl;
	goto here;	//return to gui beginning
}

