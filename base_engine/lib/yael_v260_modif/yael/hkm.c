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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "vector.h"
#include "nn.h"
#include "kmeans.h"
#include "sorting.h"
#include "machinedeps.h"
#include "hkm.h"

/*--------------------------------------------------------------
 * hierarchical clustering
 --------------------------------------------------------------*/

hkm_t * hkm_new (int d, int nlevel, int bf)
{
  int l, k = 1;
  hkm_t *hkm = (hkm_t *) malloc (sizeof (*hkm));
  hkm->nlevel = nlevel;
  hkm->bf = bf;
  hkm->d = d;
  hkm->centroids = (float **) malloc (nlevel * sizeof (*hkm->centroids));

  for (l = 0 ; l < nlevel ; l++) {
    hkm->centroids[l] = fvec_new (k * bf * d);
    k *= bf;
  }
  hkm->k = k;
  return hkm;
}


hkm_t *hkm_learn (int n, int d, int nlevel, int bf,
		  const float *points, int nb_iter_max, int nt, int verbose, 
		  int **clust_assign_out)
{
  int i, l, parent, k = 1;
  hkm_t *hkm = hkm_new (d, nlevel, bf);

  /* the absolute assignement of all points and the sizes of clusters */
  int *node_assign = calloc (sizeof (int), n);

  /* the buffer that receives the vectors gathered by parent node */
  float *v = fvec_new (n * d);

  /* Initialization */
  for (l = 0; l < nlevel; l++) {

    /* sort the vectors depending on which cluster they have been assigned to,
       and compute the number of vectors assigned to each cluster 
       *** NOTE: to replace with the k_max function of ivfgeo
       -> put this function in a separate library             */
    int *node_assign_idx = malloc (sizeof (*node_assign_idx) * n);
    ivec_sort_index (node_assign, n, node_assign_idx);

    /* Re-order the vectors depending on the previous order */
    for (i = 0; i < n ; i++)
      memmove (v + d * i, points + d * node_assign_idx[i], 
	       sizeof (*points) * d);

    /* k is the number of nodes/leaves at this level */
    int pos = 0;
    for (parent = 0; parent < k ; parent++) {
      /* Count the number of vectors assigned to this internal node */
      int nassign = 0;
      while (pos + nassign < n)
        if (node_assign[node_assign_idx[pos + nassign]] == parent)
          nassign++;
        else break;

      if (verbose) 
	fprintf (stderr, "[Level %d | Parent %d] nassign=%d | pos=%d", l, parent, nassign, pos); 

      if (nassign == 0) {
        fprintf (stderr, "# Problem2: no enough vectors in a node\n");
        exit (1);
      }

      /* Perform the clustering on this subset of points */
      int *clust_assign = ivec_new (nassign);
      float * centroids = fvec_new (bf * d);
      int nt = count_cpu();
      int flags = nt | KMEANS_INIT_RANDOM | KMEANS_QUIET;
      float err = kmeans (d, nassign, bf, nb_iter_max, v + d * pos, flags,
			  0, 1, centroids, NULL, clust_assign, NULL);
      if (verbose)
	fprintf (stderr, "-> err = %.3f\n", err);
      memcpy (hkm->centroids[l] + d * parent * bf, centroids,
              d * bf * sizeof (*centroids));

      /* Update the indexes for those points */
      for (i = 0; i < nassign; i++) {
        int truepos = node_assign_idx[pos + i];
        node_assign[truepos] = node_assign[truepos] * bf + clust_assign[i];
      }

      free (centroids);
      free (clust_assign);
      pos += nassign;
    }

    k *= bf;
    free (node_assign_idx);
  }

  if(clust_assign_out) {
    *clust_assign_out = (int *) malloc (n * sizeof (int));
    memcpy (*clust_assign_out, node_assign, n * sizeof (int));
  } 
  free (node_assign);
  free (v);
  return hkm;
}


void hkm_delete (hkm_t * hkm)
{
  int l;
  for (l = 0; l < hkm->nlevel; l++) 
    free (hkm->centroids[l]);
  free (hkm->centroids);
  free (hkm);
}



/* Quantization usign the hierarchical clustering */
void hkm_quantize (const hkm_t * hkm, int npt, const float * v, int * idx)
{
  int i, l, vw, vwtmp;

  int nlevel = hkm->nlevel;
  int bf = hkm->bf;
  int d = hkm->d;
  
  /* WARNING: not optimized *at all* (no BLAS 3 for level 1) */
  for (i = 0 ; i < npt ; i++) {
    vw = 0;
    for (l = 0 ; l < nlevel ; l++) {
      /* at this point, vw contains the parent node */
      nn (1, bf, d, hkm->centroids[l] + vw * d * bf,
	  v + d * i, &vwtmp, NULL, NULL);
      vw = vw * bf + vwtmp;
    }
    idx[i] = vw;
  }
}


/* retrieve the centroids from a particular level */
float * hkm_get_centroids (const hkm_t * hkm, int l, int no)
{
  return hkm->centroids[l] + hkm->d * hkm->bf * no;
}


/***********************************************************************/
/* I/O function for hkm                                                */

/* Macros to handle the i/O of the ivfgeo structure */
#define HKM_READ_ERROR(ret, expected_ret)               \
  {                                 \
    if (ret != (expected_ret)) {                    \
      fprintf (stderr, "# Unable to read the hkm file %s\n",  filename);\
      return NULL;                          \
    }                                   \
  }

#define HKM_WRITE_ERROR(ret, expected_ret)              \
  {                                 \
    if (ret != (expected_ret)) {                    \
      fprintf (stderr, "# Unable to write the hkm file %s\n", filename);\
      return;                               \
    }                                   \
  }


void hkm_write (const char *filename, const hkm_t * hkm)
{
  int ret = 0, l, k = hkm->bf;
  FILE *f = fopen (filename, "w");
  assert (f);

  ret = fwrite (&hkm->nlevel, sizeof (hkm->nlevel), 1, f);
  HKM_WRITE_ERROR (ret, 1);
  ret = fwrite (&hkm->bf, sizeof (hkm->bf), 1, f);
  HKM_WRITE_ERROR (ret, 1);
  ret = fwrite (&hkm->d, sizeof (hkm->d), 1, f);
  HKM_WRITE_ERROR (ret, 1);

  for (l = 0; l < hkm->nlevel; l++) {
    ret = fvec_fwrite (f, hkm->centroids[l], k * hkm->d);
    k *= hkm->bf;
  }
  fclose (f);
}


hkm_t *hkm_read (const char *filename)
{
  int ret = 0, l;
  FILE *f = fopen (filename, "r");

  int nlevel, bf, d;

  ret = fread (&nlevel, sizeof (nlevel), 1, f);
  HKM_READ_ERROR (ret, 1);
  ret = fread (&bf, sizeof (bf), 1, f);
  HKM_READ_ERROR (ret, 1);
  ret = fread (&d, sizeof (d), 1, f);
  HKM_READ_ERROR (ret, 1);


  hkm_t *hkm = hkm_new (d, nlevel, bf);
  assert (hkm);


  int k = hkm->bf;
  for (l = 0; l < hkm->nlevel; l++) {
    /* need to allocate the memory */
    ret = fvec_fread (f, hkm->centroids[l], k * hkm->d);
    HKM_READ_ERROR (ret, k * hkm->d);

    k *= hkm->bf;
  }

  hkm->k = k / hkm->bf;
  return hkm;
}
