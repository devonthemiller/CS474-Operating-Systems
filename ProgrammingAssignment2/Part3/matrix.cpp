#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <cstdio>
#include <fstream>
using namespace std;

//initialize matrices
int A[100][100];
int B[100][100];
int C[100][100];

//thread function prototype
void *runner(void *param);

//structure for passing parameters to thread runner
struct index
{
	int i;
	int j;
};

int main(int argc, char **argv)
{
	ifstream readFile;
	string fileA, fileB;
	string line;
	int rowA, colA;
	int rowB, colB;
	int count;
	string buffer = "";

	//check if matrix files are given
	//use default matrices if not
	if(argc == 3)
	{
		fileA = argv[1];
		fileB = argv[2];
	}
	else if(argc == 2)
	{
		fileA = argv[1];
		fileB = "";
	}
	else
	{
		fileA = "";
		fileB = "";
	}
	
	//initialize all matrices to zero
	for(int i = 0; i < 100; i++)
	{
		for(int j = 0; j < 100; j++)
		{
			A[i][j] = 0;
			B[i][j] = 0;
			C[i][j] = 0;
		}
	}

	if(fileA != "")
	{
		//read first matrix file
		readFile.open(fileA.c_str(), ifstream::in);
		getline(readFile, line);
		rowA = line[0] - '0'; //get number of rows
		getline(readFile, line);
		colA = line[0] - '0'; //get number of columns
	
		//read each row of matrix A
		for(int i = 0; i < rowA; i++)
		{
			getline(readFile, line);
			count = 0;
			//read each column of matrix A
			for(int j = 0; j < colA; j++)
			{			
				//read each value in row i of matrix A
				while(line[count] != ' ' && line[count] != '\n')
				{
					buffer.push_back(line[count]);
					count++;
				}
				A[i][j] = strtol(buffer.c_str(), nullptr, 10); //convert string to int
				buffer = "";
				count++;
			}
		}
		readFile.close();
	}
	//use default matrix A
	else
	{
		A[0][0] = 1; A[0][1] = 4;
		A[1][0] = 2; A[1][1] = 5;
		A[2][0] = 3; A[2][1] = 6;
		rowA = 3;
		colA = 2;
	}
	
	if(fileB != "")
	{	
		//open second matrix file
		readFile.open(fileB.c_str(), ifstream::in);
		getline(readFile, line);
		rowB = line[0] - '0';
		getline(readFile, line);
		colB = line[0] - '0';
	
		//read each row in matrix B
		for(int i = 0; i < rowB; i++)
		{
			getline(readFile, line);
			count = 0;
			//read each column in matrix B
			for(int j = 0; j < colB; j++)
			{			
				//read each value in row i of matrix B
				while(line[count] != ' ' && line[count] != '\n')
				{
					buffer.push_back(line[count]);
					count++;
				}
				B[i][j] = strtol(buffer.c_str(), nullptr, 10); //convert string to int
				buffer = "";
				count++;
			}
		}
		readFile.close();
	}
	//use default matrix B
	else
	{
		B[0][0] = 8; B[0][1] = 7; B[0][2] = 6;
		B[1][0] = 5; B[1][1] = 4; B[1][2] = 3;
		rowB = 2;
		colB = 3;
	}		

	//error handling for case where matrices are not compatible
	if(colA != rowB)
	{
		cout << "Error: Matrices are not multiplicable." << endl;
		return 0;
	}
	
	//initialize threads and max number of threads
	int thread_max = rowA * colB; //M * N
	pthread_t threads[thread_max];
	int thread_num = 0;

	for(int i = 0; i < rowA; i++)
	{
		for(int j = 0; j < colB; j++)
		{
			//create unique row/column structure for each thread
			struct index *data = (struct index*)malloc(sizeof(struct index));
			data->i = i; //row number
			data->j = j; //column number
			pthread_create(&threads[thread_num], NULL, runner, (void*)data); //create thread
			thread_num++; //index counter for threads
		}
	}

	//join all threads
	for(int i = 0; i < thread_max; i++)
	{
		pthread_join(threads[i], NULL);
	}

	//print matrix A
	cout << "Matrix A: " << endl;
	for(int i = 0; i < rowA; i++)
	{
		for(int j = 0; j < colA; j++)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;
	}

	//print matrix B
	cout << "Matrix B: " << endl;
	for(int i = 0; i < rowB; i++)
	{
		for(int j = 0; j < colB; j++)
		{
			cout << B[i][j] << " ";
		}
		cout << endl;
	}

	//print matrix C
	cout << "Product of given matrices: " << endl;
	for(int i = 0; i < rowA; i++)
	{
		for(int j = 0; j < colB; j++)
		{
			cout << C[i][j] << " ";
		}
		cout << endl;
	}
}

//thread runner
void *runner(void *param)
{
	struct index *data = (struct index*)param; //uncast index structure
	int i = (*data).i;
	int j = (*data).j;
	int sum = 0;

	//calculate C[i][j]
	for(int k = 0; k < 100; k++)
	{
		C[i][j] = C[i][j] + (A[i][k] * B[k][j]);
	}

	pthread_exit(0); //exit thread
}
