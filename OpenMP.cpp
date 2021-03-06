// Demo parallel.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
// cpp_compiler_options_openmp.cpp
#include <iostream>
#include <cstdlib>
#include <cmath>

#include <omp.h>

// function to compute the 2-norm of a vector v of length n
/*double norm(double *v, int n) {
	double norm = 0.;

	for (int i = 0; i < n; i++)
		norm += v[i] * v[i];

	return sqrt(norm);
}*/

// initialise v to values between -10 and 10
void initialize(double *v, int n) {
	for (int i = 0; i < n; i++)
		v[i] = cos(double(i)) * 10.;
}


void normalize_vector(double *v, int n) {
	double norm = 0.;

	// compute the norm of v
	for (int i = 0; i < n; i++)
		norm += v[i] * v[i];
	norm = sqrt(norm);

	// normalize v
	for (int i = 0; i < n; i++)
		v[i] /= norm;
}

void normalize_vector_omp(double *v, int n)
{
	double norm = 0.;

	// compute the norm of v
#pragma omp parallel reduction(+:norm) 
	{
#pragma omp for 
		for (int i = 0; i < n; i++) {
			norm += v[i] * v[i];
		}
	}
	norm = sqrt(norm);

	// normalize v
#pragma omp parallel 
	{
#pragma omp for
		for (int i = 0; i < n; i++)
			v[i] /= norm;
	}
}

int main(void) {
	omp_set_num_threads(64);
	const int N = 100000000;
	double *v = (double*)malloc(N * sizeof(double));

	initialize(v, N);
	double time_serial = -omp_get_wtime();
	normalize_vector(v, N);
	time_serial += omp_get_wtime();

	int max_threads = omp_get_max_threads();
	initialize(v, N);
	double time_parallel = -omp_get_wtime();
	normalize_vector_omp(v, N);
	time_parallel += omp_get_wtime();
	std::cout << max_threads << " threads" << std::endl;
	std::cout << "serial     : " << time_serial << " seconds\t"
		<< "parallel   : " << time_parallel << " seconds" << std::endl;
	std::cout << "speedup    : " << time_serial / time_parallel << std::endl;
	std::cout << "efficiency : " << (time_serial / time_parallel) / double(max_threads) << std::endl;

	free(v);
	return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
