#ifndef numerical_optimization_H
#define numerical_optimization_H


//#include <cmath>
#include <Eigen/Core>
#include <Eigen/LU> // Inverse, determinant




class NumericalOptimization {
private:  
	double epsilon; // bump size to calculate derivation
	unsigned long max_iter; // maximum number of iterations
	double tolerance; // mininum number of tolerance
	double lambda_lm; // Levenberg Marquardt
	double coef_lambda_lm; // Levenberg Marquardt


public:
	NumericalOptimization(
		double epsilon_ = 1e-10,
		unsigned long max_iter_ = 1000,
		double tolerance_ = 1e-15,
		double lambda_lm_ = 1,
		double coef_lambda_lm_ = 10
	)
	{
		epsilon = epsilon_;
		max_iter = max_iter_;
		tolerance = tolerance_;
		lambda_lm = lambda_lm_;
		coef_lambda_lm = coef_lambda_lm_;
	}
	
	Eigen::VectorXd Gradient(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x);
	Eigen::MatrixXd Hesse(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x);
	Eigen::MatrixXd Jacobi(std::function<Eigen::VectorXd(Eigen::VectorXd)> f, Eigen::VectorXd x);

	Eigen::VectorXd gradient_descent(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x0);
	Eigen::VectorXd newton_raphson(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x0);
	Eigen::VectorXd gauss_newton(std::function<Eigen::VectorXd(Eigen::VectorXd)> f, Eigen::VectorXd x0);
	Eigen::VectorXd levenberg_marquardt(std::function<Eigen::VectorXd(Eigen::VectorXd)> f, Eigen::VectorXd x0);


	
};




#endif // !numerical_optimization_H
