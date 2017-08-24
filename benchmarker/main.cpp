#pragma once
#include <iostream>
#include <cstdint>
#include <chrono>
#include <iomanip>
#include <thread>
//using namespace chrono;
using namespace std;
long double *v = new long double [2];
#ifdef _WIN32
// WINDOWS
#include <intrin.h>
uint64_t rdtscp() {
	unsigned int aux;
	return __rdtscp(&aux);
}
#else
//LINUX/macOS
unit64_t rdtscp() {
	unsigned int lo, hi;
	asm volatile ("rdtscp" : "=a" (lo), "=d" (hi));
	return ((unit64_t)hi << 32) | lo;
}

#endif
void f1(int pos) {
	auto pi = 0.0;
	long long s = 1.0;
	for (long long i = 2; i <500000000; i += 2)
	{
		pi = pi + s * (4.0 / (i * (i + 1.0) * (i + 2.0)));
		s = -s;
	}
	//return pi;
	v[pos] = pi;
}
void f2(int pos) {
	auto pi = 0.0;
	long long s = 1.0;
	for (long long i = 500000000; i < 1000000000; i += 2)
	{
		pi = pi + s * (4.0 / (i * (i + 1.0) * (i + 2.0)));
		s = -s;
		
	}
	//return pi;
	v[pos] = pi;
}


int main() {
	auto r0 = rdtscp();
	auto t0 = chrono::steady_clock::now();
	// calculo
	thread t1(f1,0);
	thread t2(f2,1);
	t1.join();
	t2.join();
	auto dc = rdtscp() - r0;
	auto d = chrono::steady_clock::now() - t0;
	auto pi = 3.0 + v[0]+ v[1];
	cout << endl;
	//cout << v[0] << " " << v[1] << endl;
	cout << setprecision(20) << pi << endl;
	//d = d / 10000;
	cout << d.count() << "s" << endl;
	cout << dc << " clocks/s" << endl;

}