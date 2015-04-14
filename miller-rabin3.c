/* Note from Jared: we used this file from the source of GMP-6.0.0a so that we
 * could modify it to work on a distributed system with MPI. Little was modified
 * other than to split the witness loop among several workers. The code is not
 * our own, it is an implementation written by John Amanatides. */

/* mpz_millerrabin(n,reps) -- An implementation of the probabilistic primality
   test found in Knuth's Seminumerical Algorithms book.  If the function
   mpz_millerrabin() returns 0 then n is not prime.  If it returns 1, then n is
   'probably' prime.  The probability of a false positive is (1/4)**reps, where
   reps is the number of internal passes of the probabilistic algorithm.  Knuth
   indicates that 25 passes are reasonable.

   THE FUNCTIONS IN THIS FILE ARE FOR INTERNAL USE ONLY.  THEY'RE ALMOST
   CERTAIN TO BE SUBJECT TO INCOMPATIBLE CHANGES OR DISAPPEAR COMPLETELY IN
   FUTURE GNU MP RELEASES.

Copyright 1991, 1993, 1994, 1996-2002, 2005 Free Software Foundation, Inc.

Contributed by John Amanatides.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the GNU MP Library.  If not,
see https://www.gnu.org/licenses/.  */

#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "primes.h"

int mpi_mpz_millerrabin (mpz_srcptr n, int reps);
static int mpi_millerrabin (mpz_srcptr, mpz_srcptr,
			mpz_ptr, mpz_ptr,
			mpz_srcptr, unsigned long int);

bool isPrime(mpz_t n) {
	return (mpi_mpz_millerrabin(n, 25) != 0);
}

int
mpi_mpz_millerrabin (mpz_srcptr n, int reps)
{
  int r;
  mpz_t nm1, nm3, x, y, q;
  unsigned long int k;
  gmp_randstate_t rstate;
  int is_prime;
  
  mpz_init2(nm1, n->_mp_size);
  mpz_sub_ui(nm1, n, 1L);

  mpz_init2(x, n->_mp_size + 1);
  mpz_init2(y, 2 * n->_mp_size);

  /* Perform a Fermat test.  */
  mpz_set_ui (x, 210L);
  mpz_powm (y, x, nm1, n);
  if (mpz_cmp_ui (y, 1L) != 0)
    {
      // TMP_FREE;
      return 0;
    }

  mpz_init2(q, n->_mp_size);

  /* Find q and k, where q is odd and n = 1 + 2**k * q.  */
  k = mpz_scan1 (nm1, 0L);
  mpz_tdiv_q_2exp (q, nm1, k);

  /* n-3 */
  mpz_init2(nm3, n->_mp_size + 1);
  mpz_sub_ui (nm3, n, 3L);

  gmp_randinit_default (rstate);

  is_prime = 1;
  for (r = 0; r < reps && is_prime; r++)
    {
      /* 2 to n-2 inclusive, don't want 1, 0 or -1 */
      mpz_urandomm (x, rstate, nm3);
      mpz_add_ui (x, x, 2L);

      is_prime = mpi_millerrabin (n, nm1, x, y, q, k);
    }

  gmp_randclear (rstate);

  return is_prime;
}

static int
mpi_millerrabin (mpz_srcptr n, mpz_srcptr nm1, mpz_ptr x, mpz_ptr y,
	     mpz_srcptr q, unsigned long int k)
{
  unsigned long int i;

  mpz_powm (y, x, q, n);

  if (mpz_cmp_ui (y, 1L) == 0 || mpz_cmp (y, nm1) == 0)
    return 1;

  for (i = 1; i < k; i++)
    {
      mpz_powm_ui (y, y, 2L, n);
      if (mpz_cmp (y, nm1) == 0)
	return 1;
      if (mpz_cmp_ui (y, 1L) == 0)
	return 0;
    }
  return 0;
}
