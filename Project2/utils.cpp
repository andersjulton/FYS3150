#include <string>
#include <cmath>
#include <fstream>


//  Allocating space for a vector and fill elements with a constant value
double* createVector(double value, int n) {
	double *vector;
	vector = new double[n];
	for (int i = 0; i < n; i++) {
		vector[i] = value;
	}
	return vector;
}

//  Allocating space for a m x n matrix and fill elements with 0
double **createMatrix(int m, int n) {
	double **mat;
	mat = new double*[m];
	for(int i = 0; i < m; i++) {
		mat[i] = new double[n];
		for(int j = 0; j < n; j++) {
			mat[i][j] = 0.0;
		}
	}
	return mat;
}

// Allocating space for a n x n diagonal matrix and fill the diagonal with d
double **createDiaMatrix(double d, int n) {
	double **mat;
	mat = createMatrix(n, n);
	// two seperate loops to avoid if-tests
	for(int i = 0; i < n; i++) {
		mat[i][i] = d;
	}
	return mat;
}

// Create a tridiagonal matrix
double **createTriDiaMatrix(double off_value, double d_value, int n) {
	double **mat;
	mat = createMatrix(n, n);
	mat[0][0] = d_value;
	mat[0][1] = off_value;
	// two seperate loops to avoid if-tests
	for (int i = 1; i < (n - 1); i++) {
		mat[i][i-1] = off_value;
		mat[i][i] = d_value;
		mat[i][i+1] = off_value;
	}
	mat[n-1][n-2] = off_value;
	mat[n-1][n-1] = d_value;
	return mat;
}

// delete 2D-array
void deleteMatrix(double **mat, int n) {
	for (int i = 0; i < n; i++) {
		delete[] mat[i];
	}
	delete[] mat;
}

// test if two arrays are equal
int testVector(double *u, double *v, int n) {
	double error;
	for (int i = 0; i < n; i++) {
		error = u[i] - v[i];
		if (fabs(error) > 1e-10) {
			printf("(%d), error = %.30f\n", i, error);
			return 1;
		}
	}
	return 0;
}

// returns the largest relative error comparing two arrays
double maxError(double *expected, double *computed, int n) {
	double max, max_i;
	max = fabs((expected[0] - computed[0])/expected[0]);
	for (int i = 1; i < n; i++) {
		max_i = fabs((expected[i] - computed[i])/expected[i]);
		if (max < max_i) {
			max = max_i;
		}
	}
	return max;
}

// returns log10 of the max relative error
double maxEpsilon(double *expected, double *computed, int n) {
	double max = maxError(expected, computed, n);
	return log10(max);
}

// write out difference between u-array and v-array
void printError(double *u, double *v, int n) {
	double error;
	printf("\n");
	for (int i = 0; i < n; i++) {
		error = u[i] - v[i];
		printf("v = %.20f, u = %.20f, error = %.30f\n", v[i], u[i], error);
	}
	printf("\n");
}

// write single array (double) to a txt-file
void doubleArrayToFile(double *v , int n, std::string filename, bool zeroPadding = false) {
	std::ofstream myfile(filename + ".txt");
	if (myfile.is_open()) {
		if (zeroPadding) {
			myfile << n+2 << "\n";
			myfile << 0.0 << "\n";
		} else {
			myfile << n << "\n";
		}
		for (int i = 0; i < n; i++) {
			myfile << v[i] << "\n";
		}
		if (zeroPadding) {
			myfile << 0.0 << "\n";
		}
	}
}

// write single array (int) to a txt-file
void intArrayToFile(int *v , int n, std::string filename, bool zeroPadding = false) {
	std::ofstream myfile(filename + ".txt");
	if (myfile.is_open()) {
		if (zeroPadding) {
			myfile << n+2 << "\n";
			myfile << 0.0 << "\n";
		} else {
			myfile << n << "\n";
		}
		for (int i = 0; i < n; i++) {
			myfile << v[i] << "\n";
		}
		if (zeroPadding) {
			myfile << 0.0 << "\n";
		}
	}
}