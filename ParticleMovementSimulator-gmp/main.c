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


#include <stdio.h>
#include <gmp.h>
#include "load_particle_pool_from_csv.h"

#define OUTPUT_PRECISION 5
#define POOL_FILENAME "particle_pool.csv"

void print_particle_values(Particle* p)
{
    gmp_printf(" x %.*Fe ", OUTPUT_PRECISION, *((*p).position_x));
    gmp_printf(" y %.*Fe ", OUTPUT_PRECISION, *((*p).position_y));
    gmp_printf(" z %.*Fe\n", OUTPUT_PRECISION, *((*p).position_z));

    gmp_printf("vx %.*Fe ", OUTPUT_PRECISION, *((*p).velocity_x));
    gmp_printf("vy %.*Fe ", OUTPUT_PRECISION, *((*p).velocity_y));
    gmp_printf("vz %.*Fe\n", OUTPUT_PRECISION, *((*p).velocity_z));

    gmp_printf(" m %.*Fe ", OUTPUT_PRECISION, *((*p).mass));
    gmp_printf(" c %.*Fe\n", OUTPUT_PRECISION, *((*p).charge));
}
 
int main(int argc, char *argv[])
{
  mpf_set_default_prec(1000); // set internal precision of GNU Multiple Precision Arithmetic Library 

  ParticlePool pool;
  pool.current_particle_index = 0;
  pool.current_field_index = 0;

  printf("Parsing Particle Pool %s", POOL_FILENAME);
  load_particle_pool_from_csv(POOL_FILENAME, &pool);
  printf("\nParsing Particle Pool %s done\n", POOL_FILENAME);

  for( int particle_index = 0; particle_index <= pool.particles_initialized; particle_index++)
  {
    printf("\nParticle %d:\n", particle_index);

    Particle* p = pool.particles[particle_index];

    print_particle_values(p);

  }

  return 0;
}


