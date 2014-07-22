#include "loadPPM.h"

unsigned int width;
unsigned int height;
unsigned int numComponents;


unsigned char *
loadPNM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents )
{
  FILE          *imgFile;
  char          buf[1024];
  char          type;
  int            i, j, k, l;
  double        distance, x, y;
  unsigned char* imageData;
  unsigned char* tmpData;

  imgFile= fopen( filename, "rb" );
//  if( errno!= 0 )
//    return NULL;
  // read PNM magic number (P1 to P6)
  if( fscanf( imgFile, "P%c \n", &type )!= 1 || type< '1' || type> '8')
  {
    fclose( imgFile );
    return NULL;
  }
  // skip comments
  while( fscanf( imgFile, "#%[^\n]\n", buf ) )
    ;
  // read width
  fscanf( imgFile, "%d", &width );
  /* skip comments */
  while( fscanf( imgFile, "#%[^\n]\n", buf ) )
    ;
  /* read height */
  fscanf( imgFile, "%d", &height );
  /* skip comments */
  while( fscanf( imgFile, "#%[^\n]\n", buf ) )
    ;
  /* skip max. component and exactly one whitespace */
  fscanf( imgFile, "%*d%*c" );

  switch( type )
  {
  case '1': // ASCII bitmap
  case '4': // binary bitmap
    cerr << "Bitmaps not implemented\n";
    fclose( imgFile );
    return NULL;
  case '2': // ASCII greymap
    imageData= new unsigned char[width*height*3];
    for( i= 0 ; i< height ; i++ )
      for( j= 0 ; j< width ; j++ )
      {
    fscanf( imgFile, "%d", &l );
    imageData[i*width+j]= l;
      }
    numComponents= 1;
    break;
  case '3': // ASCII RGB
    imageData= new unsigned char[width*height*3];
    for( i= 0 ; i< height ; i++ )
      for( j= 0 ; j< width ; j++ )
    for( k= 0 ; k< 3 ; k++ )
    {
      fscanf( imgFile, "%d", &l );
      imageData[(i*width+j)*3+k]= l;
    }
    numComponents= 3;
    break;
  case '5': // binary greymap
    imageData= new unsigned char[width*height];
    fread( imageData, 1, width*height, imgFile );
    numComponents= 1;
    break;
  case '6': // binary RGB
    imageData= new unsigned char[width*height*3];
    fread( imageData, 1, width*height*3, imgFile );
    numComponents= 3;
    break;
  }

  fclose( imgFile );

  return imageData;
}

void
WritePNM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents,
   unsigned char* imageData )
{
   FILE          *fp;
  char          buf[1024];
  char          type;
  int           i, j, k, l;
  double        distance, x, y;


// Write PGM image file with filename "file"

   // The PGM file format for a GREYLEVEL image is:
   // P5 (2 ASCII characters) <CR>
   // two ASCII numbers for nx ny (number of rows and columns <CR>
   // 255 (ASCII number) <CR>
   // binary pixel data with one byte per pixel

   // The PGM file format for a COLOR image is:
   // P6 (2 ASCII characters) <CR>
   // two ASCII numbers for nx ny (number of rows and columns <CR>
   // 255 (ASCII number) <CR>
   // binary pixel data with three bytes per pixel (one byte for each R,G,B)

    fp=fopen(filename,"wb");

    // write the first ASCII line with the file type
    if(numComponents==1)
      fprintf(fp,"P5\n"); //greylevel image
    else if(numComponents==3)
      fprintf(fp,"P6\n");  // color image

    // write image dimensions
    fprintf(fp,"%d %d\n",width,height);

    // write '255' to the next line
    fprintf(fp,"255\n");


    fwrite(imageData,sizeof(unsigned char),width*height*numComponents, fp);

    fclose(fp);

}







float *
loadPFM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents )
{
  FILE          *imgFile;
  char          buf[1024];
  char          type;
  int           i, j, k, l;
  double        distance, x, y;
  float* imageData;
  float* tmpData;

  imgFile= fopen( filename, "rb" );


  fscanf( imgFile, "PF \n", &type );
  // skip comments
  while( fscanf( imgFile, "#%[^\n]\n", buf ) )
    ;

  // read width
  fscanf( imgFile, "%d", &width );
  /* skip comments */
  while( fscanf( imgFile, "#%[^\n]\n", buf ) )
    ;

  /* read height */
  fscanf( imgFile, "%d", &height );
  /* skip comments */
  while( fscanf( imgFile, "#%[^\n]\n", buf ) )
    ;



  /* skip max. component and exactly one whitespace */
  fscanf( imgFile, "%*f%*c" );



  imageData= new float[width*height*3];

  fread( imageData, sizeof(float), width*height*3, imgFile );
  numComponents= 3;

  fclose( imgFile );

  tmpData= new float[width*3];

  //invert image for reading!!!
  for(int i=0;i<height/2;i++)
  {
    for(int j=0;j<width;j++)
    {
      for(int k=0;k<3;k++)
      {
        int indexS = i*width*3 + j*3 + k;
        int indexD = (height-1 - i)*width*3 + j*3 + k;

        tmpData[j*3 + k] = imageData[indexS];
        imageData[indexS] = imageData[indexD];
        imageData[indexD] = tmpData[j*3 + k];
      }
    }
  }

  delete [] tmpData;

  return imageData;
}

void
WritePFM( const char *filename,
     unsigned int& width, unsigned int &height,
     unsigned int &numComponents,
   float* imageData )
{
   FILE          *fp;
  char          buf[1024];
  char          type;
  int           i, j, k, l;
  double        distance, x, y;
  float max=1.0f;
  char space=' ';

// Write PGM image file with filename "file"

   // The PGM file format for a GREYLEVEL image is:
   // P5 (2 ASCII characters) <CR>
   // two ASCII numbers for nx ny (number of rows and columns <CR>
   // 255 (ASCII number) <CR>
   // binary pixel data with one byte per pixel

   // The PGM file format for a COLOR image is:
   // P6 (2 ASCII characters) <CR>
   // two ASCII numbers for nx ny (number of rows and columns <CR>
   // 255 (ASCII number) <CR>
   // binary pixel data with three bytes per pixel (one byte for each R,G,B)

    fp=fopen(filename,"wb");


    fputc('P', fp);
    fputc('F', fp);
    fputc(0x0a, fp);

    fprintf(fp, "%d %d", width, height);
    fputc(0x0a, fp);

    fprintf(fp, "%f", -1.0f);
    fputc(0x0a, fp);

    // write image data upside down!!!
  // since we flipped the image upside-down when we read it
    // we have to write it upside-down so it's stored the right way
    for(i=height-1;i>=0;i--)
      fwrite(&imageData[i*width*numComponents],sizeof(float),width*numComponents, fp);

    fclose(fp);

}



/* ------------------ added functions ------------------ */



void CreateAndSavePFM(const char *image_out)
{
    width = 511; // set size of image to 511x511 pixels
    height = 511;
    numComponents = 3;

    float *img_out = new float [width*height*numComponents];

    for ( uint i = 0 ; i < height ; ++i ) // height
    {
        for ( uint j = 0 ; j < width ; ++j ) // width
        {
            for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
            {
                uint index = i*width*numComponents + j*numComponents + k; //index within the image

                //set image to white
                img_out[index] = 1.0f; //RGB all set to white

            }

        }
    }


    WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadAndSavePPM(const char *image_in, const char *image_out)
{
    unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
    unsigned char *img_out = new unsigned char [width*height*numComponents];

    for ( uint i = 0 ; i < height ; ++i ) // height
    {
        for ( uint j = 0 ; j < width ; ++j ) // width
        {
            for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
            {
                uint index = i*width*numComponents + j*numComponents + k; //index within the image

                img_out[index] = img_in[index]; //copy all color channels of each pixel

            }

        }
    }


    WritePNM(image_out, width, height, numComponents, img_out);

}

void LoadAndSavePFM(const char *image_in, const char *image_out)
{
    float *img_in = loadPFM(image_in, width, height, numComponents);
    float *img_out = new float [width*height*numComponents];

    for ( uint i = 0 ; i < height ; ++i ) // height
    {
        for ( uint j = 0 ; j < width ; ++j ) // width
        {
            for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
            {
                uint index = i*width*numComponents + j*numComponents + k; //index within the image

                img_out[index] = img_in[index]; //copy all color channels of each pixel

            }

        }
    }


    WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadPPMAndSavePFM(const char *image_in, const char *image_out)
{
    unsigned char *img_in = loadPNM(image_in, width, height, numComponents);
    float *img_out = new float [width*height*numComponents];

    for ( uint i = 0 ; i < height ; ++i ) // height
    {
        for ( uint j = 0 ; j < width ; ++j ) // width
        {
            for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
            {
                uint index = i*width*numComponents + j*numComponents + k; //index within the image

                //typecast 0 - 255 values to the 0.0f -> 1.0f range
                img_out[index] = static_cast<float>(img_in[index])/255.0f; //typecast all color channels of each pixel

            }

        }
    }


    WritePFM(image_out, width, height, numComponents, img_out);

}

void LoadPFMAndSavePPM(const char *image_in, const char *image_out)
{
    float *img_in = loadPFM(image_in, width, height, numComponents);
    unsigned char *img_out = new unsigned char [width*height*numComponents];

    for ( uint i = 0 ; i < height ; ++i ) // height
    {
        for ( uint j = 0 ; j < width ; ++j ) // width
        {
            for ( uint k = 0 ; k < numComponents ; ++k ) // color channels - 3 for RGB images
            {
                uint index = i*width*numComponents + j*numComponents + k; //index within the image
                if(img_in[index]>1.0){
                    img_in[index] = 1.0;
                }
                //typecast 0.0f -> 1.0f values to the 0 - 255 range
                img_out[index] = static_cast<unsigned char>(img_in[index]*255.0f); //typecast all color channels of each pixel

            }

        }
    }


    WritePNM(image_out, width, height, numComponents, img_out);

}
