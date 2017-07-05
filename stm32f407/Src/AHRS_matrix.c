/* ------------------------------------------------------------------------------
  File: AHRS_matrix.c 
  Description: Functions for operating on arbitrarily sized matrices
------------------------------------------------------------------------------ */

#include <math.h>
#include "AHRS_matrix.h"

/*******************************************************************************
* Function Name  : mat_add( fMatrix* src1, fMatrix* src2, fMatrix* dest )
* Input          : fMatrix* src1, fMatrix* src2, fMatrix* dest
* Output         : fMatrix* dest
* Return         : 0 if failed, 1 if success
* Description    : 

Performs the operation
  dest = src1 + src2

src1 and src1 must be of the same dimension.  All three input matrices must
have memory allocated before calling this function.
*******************************************************************************/
int mat_add( fMatrix* src1, fMatrix* src2, fMatrix* dest )
{
	 int i, j;
	 fMatrix result;
	 
	 // Make sure that rows and columns on all relevant matrices match
	 if( (src1->rows != src2->rows) || (src1->columns != src2->columns) )
	 {
		  return 0;
	 }
	 
	 result.rows = src1->rows;
	 result.columns = src1->columns;
	 
	 // Compute sum
	 for( i=0; i < src1->rows; i++ )
	 {
		  for( j = 0; j < src1->columns; j++ )
		  {
				result.data[i][j] = src1->data[i][j] + src2->data[i][j];
		  }
	 }
	 
	 // Copy to destination
	 for( i=0; i < result.rows; i++ )
	 {
		  for( j = 0; j < result.columns; j++ )
		  {
				dest->data[i][j] = result.data[i][j];
		  }
	 }
	 dest->rows = result.rows;
	 dest->columns = result.columns;
	 
	 return 1;
}

/*******************************************************************************
* Function Name  : mat_mult( fMatrix* src1, fMatrix* src2, fMatrix* dest )
* Input          : fMatrix* src1, fMatrix* src2, fMatrix* dest
* Output         : fMatrix* dest
* Return         : 0 if failed, 1 if success
* Description    : 

Performs the operation
  dest = src1*src2

src1 and src1 must have compatible dimensions for matrix multiplication.  
*******************************************************************************/
int mat_mult( fMatrix* src1, fMatrix* src2, fMatrix* dest )
{
	 int i,j,k;
	 fMatrix result;

	 // Make sure dimensions are consistent with matrix multiplation rules
	 if( src1->columns != src2->rows )
	 {
		  return 0;
	 }
	 
	 result.rows = src1->rows;
	 result.columns = src2->columns;
	 
	 // Compute matrix product
	 for( i = 0; i < src1->rows; i++ )
	 {
		  for( j = 0; j < src2->columns; j++ )
		  {
				float dot_product = 0;
				for( k = 0; k < src1->columns; k++ )
				{
					 dot_product += src1->data[i][k]*src2->data[k][j];
				}
								
				result.data[i][j] = dot_product;
		  }
	 }
	 
	 // Copy to destination
	 for( i=0; i < result.rows; i++ )
	 {
		  for( j = 0; j < result.columns; j++ )
		  {
				dest->data[i][j] = result.data[i][j];
		  }
	 }
	 dest->rows = result.rows;
	 dest->columns = result.columns;
	 
	 return 1;
}

/*******************************************************************************
* Function Name  : mat_scalar_mult( float scalar, fMatrix* src, fMatrix* dest )
* Input          : float scalar, fMatrix* src, fMatrix* dest
* Output         : fMatrix* dest
* Return         : 0 if failed, 1 if success
* Description    : 

Performs the operation
  dest = scalar*src

src and dest matrices must have memory allocated before calling this function.
*******************************************************************************/
int mat_scalar_mult( float scalar, fMatrix* src, fMatrix* dest )
{
	 int i, j;

	 dest->rows = src->rows;
	 dest->columns = src->columns;
	 
	 for( i = 0; i < src->rows; i++ )
	 {
		  for( j = 0; j < src->columns; j++ )
		  {
				dest->data[i][j] = scalar*src->data[i][j];
		  }
	 }
	 
	 return 1;
}

/*******************************************************************************
* Function Name  : mat_determinant( fMatrix* src, float* det )
* Input          : fMatrix* src, fMatrix* dest
* Output         : float* det
* Return         : 0 if failed, 1 if success
* Description    : 

Computes the determinant of the specified matrix and returns it to the float
pointed to be float* det

src must have memory allocated before calling this function
*******************************************************************************/
int mat_determinant( fMatrix* src, float* det )
{
	 // TODO: Implement this
	 return 0;
}

/*******************************************************************************
* Function Name  : mat_transpose( fMatrix* src, fMatrix* dest )
* Input          : fMatrix* src, fMatrix* dest
* Output         : fMatrix* dest
* Return         : 0 if failed, 1 if success
* Description    : 

Computes the transpose of src and stores it in dest

The dimensions of src and dest must be compatible with the matrix transpose operation
src and dest must have memory allocated before calling this function.


*******************************************************************************/
int mat_transpose( fMatrix* src, fMatrix* dest )
{
	 int i, j;
	 fMatrix result;
	 
	 result.rows = src->columns;
	 result.columns = src->rows;
	 
	 // Compute transpose
	 for( i = 0; i < src->rows; i++ )
	 {
		  for( j = 0; j < src->columns; j++ )
		  {
				result.data[j][i] = src->data[i][j];
		  }
	 }
	 
	 // Copy to destination
	 for( i=0; i < result.rows; i++ )
	 {
		  for( j = 0; j < result.columns; j++ )
		  {
				dest->data[i][j] = result.data[i][j];
		  }
	 }
	 dest->rows = result.rows;
	 dest->columns = result.columns;
		  
	 return 1;
}


/*******************************************************************************
* Function Name  : mat_create_identity( fMatrix* dest )
* Input          : fMatrix* dest
* Output         : fMatrix* dest
* Return         : 0 if failed, 1 if success
* Description    : 

Creates an identity matrix and stores it in 'dest'

dest must have memory allocated before calling this function.


*******************************************************************************/
int mat_create_identity( fMatrix* dest, int rows, int columns )
{
	 int i, j;
	 
	 dest->rows = rows;
	 dest->columns = columns;
	 
	 for( i = 0; i < dest->rows; i++ )
	 {
		  for( j = 0; j < dest->columns; j++ )
		  {
				dest->data[i][j] = 0;
				if( i == j)
				{
					 dest->data[i][j] = 1;
				}
		  }
	 }
	 
	 return 0;
}

/*******************************************************************************
* Function Name  : mat_zero( fMatrix* dest )
* Input          : fMatrix* dest
* Output         : fMatrix* dest
* Return         : 0 if failed, 1 if success
* Description    : 

Fills the matrix 'dest' with zeroes

dest must have memory allocated before calling this function.


*******************************************************************************/
int mat_zero( fMatrix* dest, int rows, int columns )
{
	 int i, j;
	 
	 dest->rows = rows;
	 dest->columns = columns;
	 
	 for( i = 0; i < dest->rows; i++ )
	 {
		  for( j = 0; j < dest->columns; j++ )
		  {
				dest->data[i][j] = 0;
		  }
	 }
	 
	 return 1;
}

/*******************************************************************************
* Function Name  : mat_copy( fMatrix* src, fMatrix* dest )
* Input          : fMatrix* src, fMatrix* dest
* Output         : fMatrix* dest
* Return         : 0 if failed, 1 if success
* Description    : 

Copies the matrix in 'src' to 'dest'.  'src' and 'dest' must be of the same
dimension, and both must have memory allocated before calling this function.


*******************************************************************************/
int mat_copy( fMatrix* src, fMatrix* dest )
{
	 int i, j;
	 
	 dest->rows = src->rows;
	 dest->columns = src->columns;
	 
	 // Perform copy operation
	 for( i = 0; i < dest->rows; i++ )
	 {
		  for( j = 0; j < dest->columns; j++ )
		  {
				dest->data[i][j] = src->data[i][j];
		  }
	 }
	 return 1;
}
