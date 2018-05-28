/*
Copyright © INRIA 2010-2011. 
Authors: Matthijs Douze & Herve Jegou 
Contact: matthijs.douze@inria.fr  herve.jegou@inria.fr

This software is a computer program whose purpose is to provide 
efficient tools for basic yet computationally demanding tasks, 
such as find k-nearest neighbors using exhaustive search 
and kmeans clustering. 

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/


#ifndef __eigs_h
#define __eigs_h


/*---------------------------------------------------------------------------*/
/*! @addtogroup linearalgebra
 *  @{  */

/*! Compute the eigenvalues and eigvectors of a symmetric matrix m
  @param d           dimension of the square matrix m
  @param m(d,d)      the matrix (first elements are the first row)
  @param eigval(d)   on output the eigenvalues (unsorted)
  @param eigvec(d,d) on output, eigenvector j is eigvec(:,j)

  @return            =0 for success, else an error code (see info in lapack's dsygv documentation)
  
  the vectors eigval and eigvec must be allocated externally
*/
int eigs_sym (int d, const float * m, float * eigval, float * eigvec);


/*! Solve a generalized eigenvector problem */
int geigs_sym (int d, const float * a, const float * b, float * eigval, 
	       float * eigvec);


/*! Re-ordering of the eigenvalues and eigenvectors for a given criterion
   @param criterion equal to 0 for ascending order, descending otherwise
*/
void eigs_reorder (int d, float * eigval, float * eigvec, int criterion);


/*! same as eigs_sym, but returns only part of the vectors 
 * 
 * @param nev           nb of eigenvectors/values to return
 * @param eigval(nev)   the n eigenvalues
 * @param eigvec(d,nev) eigenvector j is eigvec(:,j)
 *
 * @return =0 for success, else an error code (see info in ssaupd or ierr in sseupd from arpack's documentation)
 */
int eigs_sym_part (int d, const float * m, int nev, float * eigval, float * eigvec);


/*! thin wrapper around the partial eigenvalue Arpack function */
typedef struct arpack_eigs_t arpack_eigs_t; 


/*! begin partial eigenvalue computation -- user should provide a matrix multiplication function
 *
 * @param n           dimension of the square matrix
 * @param nev         nb of eigenvectors/values to return
 */
arpack_eigs_t *arpack_eigs_begin(int n,int nev); 

/*! one iteration
 * @param x    *x_out is the array that should be multiplied (size n)
 * @param y    *y_out is result of the multiplication (size n)
 * @return     >0 compute y := A * x
               0: stop iteration
               <0, error (call arpack_eigs_end for cleanup)
 */
int arpack_eigs_step(arpack_eigs_t *,
                     float **x_out, float **y_out); 

/*! result and cleanup 
 * @param sout   eigenvalues
 * @param vout   eigenvectors
 * @return       nb of filled-in eigenvals and eigenvecs (may be below nev if some did not converge)
 */
int arpack_eigs_end(arpack_eigs_t *,
                    float * sout, float * vout); 







/*! @} */
#endif
