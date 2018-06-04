#include <iostream>
using namespace std;
#include <sys/time.h>
#include<stdlib.h>

const size_t G_BigSize    = 1000000;
const size_t G_SmallSize  = 1000;
const int    G_CycleTimes = 1000;

/*
 * 冒泡排序(bubble sort)，做一个例子使用
 */
void bsort(const int * iarr, size_t dsize, int *oarr) {
	for (size_t i = 0; i != dsize; ++i) {
		oarr[i] = iarr[i];
		for (size_t j = i; j > 0 && oarr[j-1] > oarr[j]; --j) {
			swap(oarr[j-1], oarr[j]);
		}
	}
}
void rand4data(size_t dsize, int *iarr) {
	int flag = rand()%100;
	if (flag < 10) {
		for (size_t i = 0; i != dsize; ++i) {
			iarr[i] = i;
		}
	} else if (flag < 20) {
		for (size_t i = 0; i != dsize; ++i) {
			iarr[i] = (int)dsize - i;
		}
	} else {
		for (size_t i = 0; i != dsize; ++i) {
			iarr[i] = rand();
		}
	}
}
long getCurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}
bool right4sorted(const int *oarr, size_t dsize) {
	for (size_t i = 1; i < dsize; ++i) {
		if (oarr[i-1] > oarr[i]) {
			return false;
		}
	}
	return true;
}
void test4sort(size_t dsize, void (*mysort)(const int *iarr, size_t dsize, int *oarr)) {
	int *iarr         = new int[dsize];
	int *oarr         = new int[dsize];
	long   total_time = 0;
	
	for (int i = 0; i != G_CycleTimes; ++i) {
		rand4data(dsize, iarr);
		
		long start_time = getCurrentTime();
		mysort(iarr, dsize, oarr);
		long end_time   = getCurrentTime();
		total_time     += end_time - start_time;
		
		if (!right4sorted(oarr, dsize)) {
			cerr << "sort for ints failed" << endl;
			return;
		}
	}
	cout << "You have completed for sort testing!" << endl;
	cout << "And the total time is " 
		<< float(total_time)/1000000.0 << " seconds." << endl;

	delete [] iarr;
	delete [] oarr;
	iarr = NULL;
	oarr = NULL;
}
int main(int argc, char *argv[]) {
	if (argc < 3) {
		cerr << "\t ./" << argv[0] << " type4data(big|small) method4sort(bsort|todo)" << endl;
		return -1;
	} else {
		size_t dsize = 0;
		if (string(argv[1]) == "small") {
			dsize = G_SmallSize;
		} else if (string(argv[1]) == "big") {
			dsize = G_BigSize;
		} else {
			cerr << "\t ./" << argv[0] << " type4data(big|small) method4sort(bsort|todo)" << endl;
			return -2;
		}
		if (string(argv[2]) == "bsort") {
			test4sort(dsize, bsort);
		} else {
			cerr << "unknown method for sorting : " << argv[2] << endl;
			return -3;
		}
	}
}
