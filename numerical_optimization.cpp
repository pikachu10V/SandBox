#include "numerical_optimization.hpp"
#include <string>
#include <vector>



// ---------- Gradient vector and Hesse matrix ---------- 
Eigen::VectorXd NumericalOptimization::Gradient(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x) {
	Eigen::VectorXd grad(x.size());
	for (int i = 0; i < x.size(); i++) {
		Eigen::VectorXd x_plus = x;
		Eigen::VectorXd x_minus = x;
		x_plus(i) += epsilon;
		x_minus(i) -= epsilon;
		grad(i) = (f(x_plus) - f(x_minus)) / (2 * epsilon);
	}
	return grad;
}


Eigen::MatrixXd NumericalOptimization::Hesse(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x) {
	Eigen::MatrixXd hess(x.size(), x.size());
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < x.size(); j++) {
			Eigen::VectorXd x_plus = x;
			Eigen::VectorXd x_minus = x;
			x_plus(i) += epsilon;
			x_minus(i) -= epsilon;
			x_plus(j) += epsilon;
			x_minus(j) -= epsilon;
			hess(i, j) = (f(x_plus) - 2 * f(x) + f(x_minus)) / (epsilon * epsilon);
		}
	}
	return hess;
}


Eigen::MatrixXd NumericalOptimization::Jacobi(std::function<Eigen::VectorXd(Eigen::VectorXd)> f, Eigen::VectorXd x)
{
	Eigen::VectorXd fx = f(x);
	Eigen::MatrixXd dx = Eigen::MatrixXd::Zero(x.size(), x.size());
	for (int i = 0; i < x.size(); i++) {
		Eigen::VectorXd x_plus_eps = x;
		Eigen::VectorXd x_minus_eps = x;
		x_plus_eps(i) += epsilon;
		x_minus_eps(i) -= epsilon;
		Eigen::VectorXd fx_plus_eps = f(x_plus_eps);
		Eigen::VectorXd fx_minus_eps = f(x_minus_eps);
		Eigen::VectorXd dx_i = (fx_plus_eps - fx_minus_eps) / epsilon;
		
		for (int j = 0; j < dx_i.size(); j++) {
			dx(i, j) = dx_i(j);
		}
	}
	return dx;
}


// ---------- Optimization Algorithm ---------- 
Eigen::VectorXd NumericalOptimization::gradient_descent(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x0) {
	Eigen::VectorXd x = x0;
	Eigen::VectorXd x_prev = x0;
	Eigen::VectorXd grad = Gradient(f, x0);
	int iter = 0;
	while (iter < max_iter && (x - x_prev).norm() > tolerance) {
		x_prev = x;
		x = x - epsilon * grad;
		grad = Gradient(f, x);
		iter++;
	}
	return x;
}


Eigen::VectorXd NumericalOptimization::newton_raphson(std::function<double(Eigen::VectorXd)> f, Eigen::VectorXd x0) {
	Eigen::VectorXd x = x0;
	Eigen::VectorXd x_prev = x0;
	Eigen::VectorXd grad = Gradient(f, x0);
	Eigen::MatrixXd hess = Hesse(f, x0);
	
	int iter = 0;
	while (iter < max_iter && (x - x_prev).norm() > tolerance) {
		x_prev = x;
		x = x - hess.inverse() * grad;
		grad = Gradient(f, x);
		hess = Hesse(f, x);
		iter++;
	}

	return x;
}


Eigen::VectorXd NumericalOptimization::gauss_newton(std::function<Eigen::VectorXd(Eigen::VectorXd)> f, Eigen::VectorXd x0) {
	Eigen::VectorXd x = x0;
	Eigen::VectorXd fx = f(x);
	Eigen::VectorXd dx = -fx;
	
	int iter = 0;
	while (dx.norm() > tolerance && iter < max_iter) {
		Eigen::MatrixXd J = Jacobi(f, x);
		Eigen::VectorXd dx = -J.inverse() * fx;
		x = x + dx;
		fx = f(x);
		iter++;
	}
	
	return x;
}


Eigen::VectorXd NumericalOptimization::levenberg_marquardt(std::function<Eigen::VectorXd(Eigen::VectorXd)> f, Eigen::VectorXd x0) {
	Eigen::VectorXd x = x0;
	Eigen::VectorXd fx = f(x);
	Eigen::VectorXd dx = -fx;
	double s_old = fx.squaredNorm();

	double lambda = this->coef_lambda_lm;
	int iter = 0;
	while (dx.norm() > tolerance && iter < max_iter) {
		Eigen::MatrixXd J = Jacobi(f, x);
		Eigen::MatrixXd JJandLambdaI = J.transpose() * J + lambda * Eigen::MatrixXd::Identity(x.size(), x.size());
		Eigen::VectorXd dx = -JJandLambdaI.inverse() * (J.transpose()) * fx;
		x = x + dx;
		fx = f(x);

		double s_new = fx.squaredNorm();
		lambda *= (s_old > s_new) ? (1 / this->coef_lambda_lm) : this->coef_lambda_lm;
		s_old = s_new;
		iter++;
	}

	return x;
}
