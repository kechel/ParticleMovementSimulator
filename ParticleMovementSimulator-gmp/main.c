/*
    Jan Kechel - jan@kechel.de
    Copyright (c) 2017

    This File is part of Particle Movement Simulator with the The GNU Multiple Precision Arithmetic Library (PMS with GMP)

    PMS with GMP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PMS with GMP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>. 
*/


#include <gmp.h>
#include <stdio.h>
#include "load_particle_pool_from_csv.h"
#include "output_helpers.h"

#define POOL_FILENAME "particle_pool.csv"
 
int main(int argc, char *argv[])
{
  mpf_set_default_prec(1000); // set internal precision of GNU Multiple Precision Arithmetic Library 

  ParticlePool pool;
  pool.current_particle_index = 0;
  pool.current_field_index = 0;

  printf("Parsing Particle Pool %s", POOL_FILENAME);
  load_particle_pool_from_csv(POOL_FILENAME, &pool);
  printf("\nParsing Particle Pool %s done\n", POOL_FILENAME);

  print_particle_pool_values(&pool);

  return 0;
}


