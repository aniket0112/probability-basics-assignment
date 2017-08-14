Basic Statisctics Calculator
============================
# Introduction
This repository consists of codes developed to compute random variable functions on dataset used from SDA GSS which can be found from :
sda.berkeley.edu/GSS
The dataset used is Personal and Family Information -> Marital Status
The application can calculate probability, conditional probability, random variable frequency, expected value, second moment, standard deviation and plot PDF, CDF of discrete data.
# Setup
* Clone the repository to your local computer
* The compiler is C++11. Make changes to your compiler as required. For Eclipse, follow this stack exchange answer : https://stackoverflow.com/questions/17457069/enabling-c11-in-eclipse-juno-kepler-luna-cdt
* You will also need to install GNUplot on your respective system. For Ubuntu, follow the following links to install and configure GNUplot : https://www.howtoinstall.co/en/ubuntu/xenial/gnuplot
* Once done with the above steps, you can run the code and follow console instructions to use the application.

# Note
* If you do not want to setup GNUplot, the plots are saved in the repository with apt names. To ignore plot commands change macro `PLOT` to `0` in `main.cpp`. Otherwise, there can be error.
* Also on Windows, change macro `WIN` to `1` in `main.cpp`.

