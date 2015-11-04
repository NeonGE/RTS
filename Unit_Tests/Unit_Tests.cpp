/********************************************************************
	Created:	2014/02/10
	Filename:	Unit_Tests.cpp
	Author:		Samuel Prince

	Purpose:	Unit_Tests: Es una aplicación creada para hacer
				pruebas de velocidad con diferentes algoritmos y
				para comprobar que varias de las funciones de las 
				librerías funcionan como es debido
*********************************************************************/
#include "Unit_TestsPCH.h"
#include <math.h>       /* log */
#include <xmmintrin.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <vld.h>

//Forward declarations
void memtest1();
void memtest2();

void dynmemtest1();
void dynmemtest2();

unsigned char g_HugeMemoryChunk[4096][4096];

float luisLog(const float &X, const int &presicion)
{
	float logNum= 1.0f / (X);
	float result=0;
	float exponential=1;
	for( int i=1; i<presicion; i++)
	{
		exponential *= logNum;
		result += i%2 ? exponential/i : -(exponential/i);
	}

	return result;
}

float miLog(const float &X, const int &presicion)
{
	float result;
	float logNum = 1/X;
	float logNumLessOne = logNum-1;

	result = 0;

	for( int i=1; i<presicion; i++)
	{
		result += pow(-1.f, i+1) * (pow(logNumLessOne,i)/i);
	}

	return -result;
}

float suLog(const float &X)
{
	return log(X);
}

void memtest1()
{
	for(int x=0; x<4096; x++)
		for(int y=0; y<4096; y++)
			g_HugeMemoryChunk[x][y] = 10;
}

void memtest2()
{
	for(int y=0; y<4096; y++)
		for(int x=0; x<4096; x++)
			g_HugeMemoryChunk[x][y] = 20;
}

void dynmemtest1()
{
	unsigned char **hugeDynMemoryChunk;
	hugeDynMemoryChunk = new unsigned char*[4096];
	for(int i=0; i<4096; i++)
	{
		hugeDynMemoryChunk[i] = new unsigned char[4096];
	}

	for(int x=0; x<4096; x++)
		for(int y=0; y<4096; y++)
			hugeDynMemoryChunk[x][y] = 128;

	for(int i=0; i<4096; i++)
	{
		delete [] hugeDynMemoryChunk[i];
	}

	delete [] hugeDynMemoryChunk;
}

void dynmemtest2()
{
	unsigned char **hugeDynMemoryChunk;
	hugeDynMemoryChunk = new unsigned char*[4096];
	for(int i=0; i<4096; i++)
	{
		hugeDynMemoryChunk[i] = new unsigned char[4096];
	}

	for(int x=0; x<4096; x++)
		for(int y=0; y<4096; y++)
			hugeDynMemoryChunk[y][x] = 128;

	for(int i=0; i<4096; i++)
	{
		delete [] hugeDynMemoryChunk[i];
	}

	delete [] hugeDynMemoryChunk;
}

static __forceinline float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration

	return y;
}

static __forceinline float WinInvSqrt( float F )
{
	return 1.0f/sqrt(F);
}

#define NUM_ITERACIONES_SQRT 13107200*4
float matriz_sqrts[NUM_ITERACIONES_SQRT];

int32 _tmain(int32, TCHAR**)
{
	memtest1();
	memtest2();

	float valor = 0.0f;
	int i;

	auto startnorm = std::chrono::high_resolution_clock::now();

	for(i=0; i<NUM_ITERACIONES_SQRT; i++)
	{
		valor = (float)i;
		matriz_sqrts[i] = WinInvSqrt(valor);
	}

	auto endnorm = std::chrono::high_resolution_clock::now();
	auto elapsednorm = std::chrono::duration_cast<std::chrono::milliseconds>(endnorm - startnorm);

	auto startcarmack = std::chrono::high_resolution_clock::now();

	for(i=0; i<NUM_ITERACIONES_SQRT; i++)
	{
		valor = (float)i;
		matriz_sqrts[i] = Q_rsqrt(valor);
	}

	auto endcarmack = std::chrono::high_resolution_clock::now();
	auto elapsedcarmack = std::chrono::duration_cast<std::chrono::milliseconds>(endcarmack - startcarmack);

	auto startinv = std::chrono::high_resolution_clock::now();

	for(i=0; i<NUM_ITERACIONES_SQRT; i++)
	{
		valor = (float)i;
		matriz_sqrts[i] = InvSqrt(valor);
	}

	auto endinv = std::chrono::high_resolution_clock::now();
	auto elapsedinv = std::chrono::duration_cast<std::chrono::milliseconds>(endinv - startinv);

	std::cout << "Normal:    " << elapsednorm.count()		<< "ms. InvSqrt 50000: " << WinInvSqrt(50000.0f) << std::endl;
	std::cout << "SIMD:      " << elapsedinv.count()		<< "ms. InvSqrt 50000: " << InvSqrt(50000.0f) << std::endl;
	std::cout << "Carmack:   " << elapsedcarmack.count()	<< "ms. InvSqrt 50000: " << Q_rsqrt(50000.0f) << std::endl;

	int wait;
	std::cin >>wait;

	return 0;
}