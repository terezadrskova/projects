#ifndef LOADPPM_H
#define LOADPPM_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

unsigned char *
loadPNM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents );

void
WritePNM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents,
   unsigned char* imageData );


float *
loadPFM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents );

void
WritePFM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents,
   float* imageData );


void CreateAndSavePFM(const char *image_out);
void LoadAndSavePPM(const char *image_in, const char *image_out);
void LoadAndSavePFM(const char *image_in, const char *image_out);
void LoadPPMAndSavePFM(const char *image_in, const char *image_out);
void LoadPFMAndSavePPM(const char *image_in, const char *image_out);


#endif // LOADPPM_H
