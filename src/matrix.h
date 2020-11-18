#pragma GCC optimize(3,"Ofast","inline")

#ifndef _MATRIX_H
#define _MATRIX_H

#include "utility.h"
#include "Eigen/Dense"
#include "Eigen/Eigenvalues"
#include <omp.h>

using namespace std;
using namespace Eigen;

class Matrixx {

public:
	Matrixx():n(0) {};
	Matrixx(const char * infilename);
	MatrixXf get_matrix();
	vector<string> Get_Sample_Names();
	void get_Deviation_Matrix();
	float ** Get_PC_Matrix(int k);
	float * get_percentage(int k);
	friend unsigned int Output_Table(vector<string> & samples, Matrixx & PC_m, ostream * outfile, int k);
	friend unsigned int Output_Table(vector<string> & samples, Matrixx & PC_m, const char * outfilename, int k);
	friend ostream & operator << (ostream& os, const Matrixx& M);

private:
	int n;
	MatrixXf matrix;
	MatrixXf pdblVects;
	MatrixXf pdbEigenValues;
	vector<string> samples;	
};

Matrixx::Matrixx(const char * infilename) {
	ifstream infile(infilename, ifstream::in);
	if(!infile) {
		cerr << "Error: Cannot open file : " << infilename << endl;
		exit(0);
	}
	
	string buffer;
	
	getline(infile, buffer);
	stringstream strin(buffer);
	
	string sample_name;
	while(strin >> sample_name) 
		samples.push_back(sample_name);

	this->n = samples.size();
	matrix.resize(this->n, this->n);

	float dist;
	for(int i = 0; i < n; i++) {
		getline(infile, buffer);
		stringstream strin(buffer);
		strin >> sample_name;
		for(int j = 0; j < n; j++) {
			strin >> dist;
			matrix(i, j) = dist;
		}
	}
	get_Deviation_Matrix();
	//debug
	/*
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			cout << matrix(i, j) << "\t";
		}
		cout << endl;
	}
	*/
	//debug
}

MatrixXf Matrixx::get_matrix() {
	return matrix;
}

vector<string> Matrixx::Get_Sample_Names() {
	return samples;
}

void Matrixx::get_Deviation_Matrix() {
	/*
	MatrixXf one = MatrixXf::Constant(n, n, 1);
	MatrixXf mat(n, n);
	mat.setIdentity();
	mat = mat - one / n;
	float para = 1.0;

	#pragma omp parallel for schedule(dynamic, 1)
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= i; j++) {
			matrix(i, j) = matrix(j, i) = matrix(i, j) * matrix(i, j) * para;
		}
	}

	matrix =  -mat * matrix * mat;
	*/

	#pragma omp parallel for schedule(dynamic, 1)	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j <= i; j++) {
			matrix(i, j) = matrix(j, i) = matrix(i, j) * matrix(i, j);
		}
	}
	
	float * row_sums = (float *) malloc(sizeof(float) * n);
	#pragma omp parallel for
	for(int i = 0; i < n; i++) {
		row_sums[i] = 0;
	}
	//#pragma omp parallel for
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			row_sums[i] += matrix(i, j);
		}
	}

	/*
	float * col_sums = (float *) malloc(sizeof(float) * n);
	#pragma omp parallel for 
	for(int i = 0; i < n; i++) {
		col_sums[i] = 0;
	}
	//#pragma omp parallel for
	for(int i = 0; i < n; i++) { 
                for(int j = 0; j < n; j++) { 
                        col_sums[j] += matrix(i, j); 
                } 
        } 
	*/
	float sums = 0;
	#pragma omp parallel for reduction(+:sums)
	for(int i = 0; i < n; i++) {
		sums += row_sums[i];
	}
	#pragma omp parallel for
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			//matrix(i, j) = -0.5 * matrix(i, j) + 1.0 / (2 * n) * row_sums[i] + 1.0 / (2 * n) * col_sums[j] - 1.0 / (2 * n * n) * sums;
			matrix(i, j) = -0.5 * matrix(i, j) + 1.0 / (2 * n) * (row_sums[i] + row_sums[j]) - 1.0 / (2 * n * n) * sums;
		}
	}
	//debug
	/*
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			cout << matrix(i, j) << "\t";
		}
		cout << endl;
	}
	*/
	
	//debug
}

float ** Matrixx::Get_PC_Matrix(int k) {
	if(k >= n) {
		cout << "'k' must be in {1, 2, ..  n - 1}" << endl;
		float ** tmp = NULL;
		return tmp;
	}
	
	SelfAdjointEigenSolver<MatrixXf> es(matrix);
        pdbEigenValues = es.eigenvalues();
        pdblVects = es.eigenvectors();
	//cout << "pdblVects" << endl;
	//cout << pdblVects << endl;
        float ** pc = (float **) malloc(sizeof(float*) * n);
	for(int i = 0; i < n; i++) {
		pc[i] = (float *) malloc(sizeof(float) * k);
	}

        #pragma omp parallel for 
        for(int i = 0; i < n; i++) {
                for(int j = 0; j < k; j++) {
                        pc[i][j] = pdblVects(i, n-j-1) * sqrt(pdbEigenValues(n-j-1));
                }
        }

        return pc;	
}

float * Matrixx::get_percentage(int k) {
	float * percentage = (float *) malloc(sizeof(float) * k);
	float sum_s = 0;
        #pragma omp parallel for reduction(+:sum_s) 
        for(int i = 0; i < n; i++) {
        	sum_s += matrix(i, i);
        }
        #pragma omp parallel for 
        for(int i = 0; i < k; i++) {
                percentage[i] = pdbEigenValues(n-i-1)/sum_s;
        }
	return percentage;
}

unsigned int Output_Table(vector<string> & samples, float ** PC_m, ostream * outfile, int k){

	for(int i = 0; i < k; i++) {
		*outfile << "\tPC" << i+1;
	}
	*outfile << endl;
	//cout << "samples.size = " << samples.size() << endl;
	for(int i = 0; i < samples.size(); i++) {
		*outfile << samples[i];
		for(int j = 0; j < k; j++) {
			*outfile << "\t" << PC_m[i][j];
		}
		*outfile << endl;
	}
	return samples.size();
}

unsigned int Output_Table(vector<string> & samples, float ** PC_m, const char * outfilename, int k){

	ofstream outfile(outfilename, ofstream::out);
	if (!outfile){
		cerr << "Error: Cannot open output file: " << outfilename << endl;
		return 0;
	}

	unsigned int out_count = Output_Table(samples, PC_m, &outfile, k);

	outfile.close();
	outfile.clear();
	return out_count;
}

ostream & operator << (ostream& os, const Matrixx& M) {
	for(int i = 0; i < M.n; i++) {
		for(int j = 0; j < M.n; j++) {
			os << M.matrix(i, j) << "\t";
		}
		os << endl;
	}
	return os;
}


#endif
