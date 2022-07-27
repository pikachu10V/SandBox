#include "interpolation.hpp"


Interp1D::Interp1D(std::vector<double> x_, std::vector<double> y_, std::string extrapolation_)
{
        this->X = x_;
        this->Y = y_;
        this->N = X.size() - 1; //sizeof(X) / sizeof(X[0]);
        int Ny = Y.size() - 1;// sizeof(Y) / sizeof(Y[0]);
        this->extrapolation = extrapolation_;

		if (N < 2 || Ny < 2) {
			std::cout << "Error! The size of X or Y must be greater than 2." << std::endl;
			return;
		}

		if (N != Ny) {
			std::cout << "Error! The size of X and Y are different." << std::endl;
			return;
		}

		for (int i=0; i<N; i++) {
			if ( X[i] > X[i+1]) {
				std::cout << "Error! X must be monotonically increasing." << std::endl;
				return;
			}
		}
		Valid = true;
}


double Interp1D::Linear(double x, std::string option) {
    if (!Valid) {
        return 0;
    }

    if (x < X[0]) {
        if (extrapolation == "Flat") {
            return Y[0];
        } else if (extrapolation == "Zero") {
            return 0;
        } else if (extrapolation == "Linear") {
            if (option == "ZC") {
                return Y[0] + (Y[1] - Y[0])/(X[1] - X[0]) * (x - X[0]);
            } else if (option == "Fwd") {
                double dF_dT = 2*(Y[1] - Y[0])/(X[1] - X[0]);
                double Fwd_0 = Y[0] + (Y[1] - Y[0])/(X[1] - X[0]) * X[0]; 
                return Fwd_0 + dF_dT * (x - X[0]);
            } else {
                std::cout << "Error! Invalid option." << std::endl;
                return 0;
            }
        } else {
            std::cout << "Error! Invalid extrapolation option." << std::endl;
            return 0;
        }
    }

    if (x > X[N]) {
        if (extrapolation == "Flat") {
            return Y[N];
        } else if (extrapolation == "Zero") {
            return 0;
        } else if (extrapolation == "Linear") {
            if (option == "ZC") {
                return Y[N] + (Y[N] - Y[N-1])/(X[N] - X[N-1]) * (x - X[N]);
            } else if (option == "Fwd") {
                double dF_dT = 2*(Y[N] - Y[N-1])/(X[N] - X[N-1]);
                double Fwd_N = Y[N] + (Y[N] - Y[N-1])/(X[N] - X[N-1]) * X[N]; 
                return Fwd_N + dF_dT * (x - X[N]);
            } else {
                std::cout << "Error! Invalid option." << std::endl;
                return 0;
            }
        } else {
            std::cout << "Error! Invalid extrapolation option." << std::endl;
            return 0;
        }
    }

    int cnt = 0, i = 0;
    bool flag = false;
    while (cnt < N) {
        if (x >= X[cnt] && x <= X[cnt+1]) {
            i = cnt;
            flag = true;
            break;
        }
        cnt++;
    }
    double out;
    
    if (option=="ZC") {
        out = Y[i] + (Y[i+1] - Y[i])/(X[i+1] - X[i]) * (x - X[i]);
    } else if (option=="Fwd") {
        out = Y[i] + (Y[i+1] - Y[i])/(X[i+1] - X[i]) * (2*x - X[i]);
    } else {
        std::cout << "Error! Invalid option." << std::endl;
        out = 0;
    }
    return out;
}




int debug_interpolation()
{    
    double arrx[] = {0,1,2,3,4};
    double arry[] = {1,3,6,4,10};
    //double *x = arrx;
    //double *y = arry;	
    std::vector<double> X, Y;
    X.assign(arrx, arrx + sizeof(arrx) / sizeof(arrx[0]));
    Y.assign(arry, arry + sizeof(arry) / sizeof(arry[0]));

    Interp1D interp(X, Y, "Linear");

    // output to file
    double Tvals, ZCvals, Fwdvals;
    int num_grid = 100;
    std::ofstream ofs("csv\\interp.csv");

    // header
    ofs << "T" << ", "<< "ZC" << ", " << "Fwd" << ", " << std::endl;

    // calculation
    for (int i=0; i<=num_grid; i++) {
        Tvals = i*5.0/num_grid;
        ZCvals = interp.Linear(Tvals, "ZC");
        Fwdvals = interp.Linear(Tvals, "Fwd");
        ofs << Tvals << ", "<< ZCvals << ", " << Fwdvals << ", " << std::endl;
    }


    std::cout << "Done!" << std::endl;
    return 0;
}
