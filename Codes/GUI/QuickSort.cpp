#include "QuickSort.h"
#include <iostream>
#include <algorithm>	// std::swap()
#include <vector>
#include "time.h"
#include "qdebug.h"
using namespace std;

CQuickSort::CQuickSort(void)
{
}


CQuickSort::~CQuickSort(void)
{

}

vector<double> QSort(const vector<double> &V1)
{

	if (V1.size() <= 1) return V1;
	vector<double> V = V1;
	int end = V.size();
	if (V[end - 1]<V[0]) V = reverse_order(V);
	vector<double> less, greater;
	greater.push_back(V[end - 1]);
	for (int i = 0; i<end - 1; i++)
		if (V[i]<V[end - 1]) less.push_back(V[i]);
		else greater.push_back(V[i]);


		if ((V == greater) && (less.size() == 0))
			return greater;
		vector<double> res = QSort(less);
		vector<double> x2 = QSort(greater);

		res.insert(res.end(), x2.begin(), x2.end());
		less.clear();
		greater.clear();
		x2.clear();
		return res;

}

vector<int> QSort(const vector<int> &V1)
{
	if (V1.size() <= 1) return V1;
	vector<int> V = V1;
	int end = V.size();
	if (V[end - 1]<V[0]) V = reverse_order(V);
	vector<int> less, greater;
	greater.push_back(V[end - 1]);
	for (int i = 0; i<end - 1; i++)
		if (V[i]<V[end - 1]) less.push_back(V[i]);
		else greater.push_back(V[i]);


		vector<int> res = QSort(less);
		if ((V == greater) && (less.size() == 0)) return greater;
		vector<int> x2 = QSort(greater);

		res.insert(res.end(), x2.begin(), x2.end());
		less.clear();
		greater.clear();
		x2.clear();
		return res;

}

vector<double> reverse_order(const vector<double> &V)
{
	vector<double> A;
	for (int i = V.size() - 1; i >= 0; i--)
		A.push_back(V[i]);

	return A;
}

vector<int> reverse_order(const vector<int> &V)
{
	vector<int> A;
	for (int i = V.size() - 1; i >= 0; i--)
		A.push_back(V[i]);

	return A;
}

vector<double> bubbleSort(const vector<double> &V)
{
	clock_t t0 = clock();
	if (V.size() <= 1) return V;
	vector<double> A;
	if (V[V.size() - 1] < V[0])
		A = reverse_order(V);
	else
		A = V;

	int n = A.size();
	double temp;
	bool swapped = false;
	do
	{
		swapped = false;
		for (int i = 1; i < n - 1; i++)
		{
			if (A[i - 1] > A[i])
			{
				//temp = A[i - 1];
				//A[i - 1] = A[i];
				//A[i] = temp;
				swap(A[i], A[i - 1]);
				swapped = true;
			}
		}
	} while (swapped);
	clock_t t1 = clock() - t0;
	float run_time = ((float)t1) / CLOCKS_PER_SEC;
	qDebug() << "sorting finished in" << run_time << " sec";
	return A;
}
vector<int> bubbleSort(const vector<int> &V)
{
	if (V.size() <= 1) return V;
	vector<int> A;
	if (V[V.size() - 1] < V[0])
		A = reverse_order(V);
	else
		A = V;
	int n = A.size();
	int temp;
	bool swapped = false;
	do
	{
		swapped = false;
		for (int i = 1; i < n - 1; i++)
		{
			if (A[i - 1] > A[i])
			{
				//temp = A[i - 1];
				//A[i - 1] = A[i];
				//A[i] = temp;
				swap(A[i], A[i - 1]);
				swapped = true;
			}
		}
	} while (!swapped);
	return A;
}




