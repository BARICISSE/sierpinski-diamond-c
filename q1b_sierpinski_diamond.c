#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum //quick definition of the datatype bool
{
    true = 1, false = 0
} bool;


bool* makeFractal( int height,int width );
void printFractal( bool* fr,int height,int width );
void increaseFractality( bool* fr,int topRow,int topCol,int heightOfTriangle,int width );

int main( int argc,char* argv[] )
{
    int height = atoi( argv[ 1 ] );
    int level = atoi( argv[ 2 ] );
    int halfHeight = ( height + 1 ) / 2;
    
    if( argc != 3 )
    {
        printf( "ERROR: Wrong number of arguments. Two required.\n" );
        return 0;
    }
    
    if( !( ( height + 1 ) && !( ( height + 1 ) & ( height ) ) ) ) //if height + 1 is not a power of 2
    {
        printf( "ERROR: Height does not allow evenly dividing requested number of levels.\n" );
        return 0;
    }
    
    if( ( int )log2( height + 1 ) < level || level < 1 ) //if level is not in the correct range
    {
        printf( "ERROR: Height does not allow evenly dividing requested number of levels.\n" );
        return 0;
    }
    
    
    bool* fractal = makeFractal( halfHeight,height ); //we are printing H * H characters, so height = width
    
    int fractalHeight = halfHeight; //current heigth of the triangle being fractalized
    
    for( int i = 1; i < level; i++ )
    {
        increaseFractality( fractal,0,halfHeight - 1,fractalHeight,height ); //start in top row in the middle
        fractalHeight /= 2; //call on smaller and smaller triangle heights depending on the level
    }
    
    printFractal( fractal,halfHeight,height );
    
    
    free( fractal );
    fractal = NULL;
    return 0;
}

bool* makeFractal( int height,int width ) //this fills the 2D ( same as q1a or level 1 fractal )
{
    //I represent the fractal as 2D array of bools that is half the size of the full fractal ( it is mirrored )
    bool ( *fractal )[ width ] = malloc( height * sizeof( *fractal ) ); //allocated on heap to practice pointers
    
    int stars = 1; //there is only one star in the very first row
    
    for( int i = 0; i < height; i++ )
    {
        int offset = 0;
        for( int j = 0; j < height - 1 - i; j++,offset++ ) fractal[ i ][ offset ] = false;  //left spaces
        for( int j = 0; j < stars; j++,offset++ )          fractal[ i ][ offset ] = true;   //stars
        for( int j = 0; j < height - 1 - i; j++,offset++ ) fractal[ i ][ offset ] = false;  //right spaces
        
        stars += 2; //each subsequent row has two more stars in it
    }
    return *fractal; //returning the pointer to the array, just so all the functions can modify it
}

void printFractal( bool* fr,int height,int width ) //the fractal is a mirror image
{
    for( int i = 0; i < height; i++ ) //top half
    {
        for( int j = 0; j < width; j++ )
        {
            //print star if the value at index [ i ][ j ] is true, space otherwise
            printf( *( ( fr + i * width ) + j ) == true ? "*" : " " );
            //printf( fr[ i ][ j ] == true ? "*" : " " );
        }
        printf( "\n" );
    }
    
    for( int i = height - 2; i >= 0; i-- ) //bottom half
    {
        for( int j = 0; j < width; j++ )
        {
            printf( *( ( fr + i * width ) + j ) == true ? "*" : " " );
        }
        printf( "\n" );
    }
}

void increaseFractality( bool* fr,int topRow,int topCol,int heightOfTriangle,int width )
{
    int spaces = 1; //all empty triangles start with one space in the bottom most row
    int leftIndex = topCol;
    
    for( int i = 0; i < heightOfTriangle / 2; i++ ) //loops from top to bottom
    {
        for( int j = 0; j < spaces; j++ ) //and changes stars to spaces from left to right
        {
            *( ( fr + ( topRow + ( heightOfTriangle - 1 ) - i ) * width ) + ( leftIndex + j ) ) = false;
        }
        leftIndex--; //move to left by one
        spaces += 2; //each row higher has two more spaces
    }
    
    if( topRow + heightOfTriangle - 1 != ( ( width + 1 ) / 2 ) - 1 ) //if the next iteration exists
    {
        //call itself on bottom left and bottom right triangle
        increaseFractality( fr,topRow + heightOfTriangle,topCol - heightOfTriangle,heightOfTriangle,width );
        increaseFractality( fr,topRow + heightOfTriangle,topCol + heightOfTriangle,heightOfTriangle,width );
    }
}

