#ifndef interpolation_hpp
#define interpolation_hpp

#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <Windows.h>


class Interp1D
{
private:
	bool Valid;
	//double* X;
    //double* Y;
	std::vector<double> X;
	std::vector<double> Y;
	int N;
	std::string extrapolation;

public:
	//Interp1D(double* x_, double* y_, std::string extrapolation_="Flat");
	Interp1D(std::vector<double> x_, std::vector<double> y_, std::string extrapolation_ = "Flat");
	double Linear(double x, std::string option="ZC");
};





int debug_interpolation();


#endif