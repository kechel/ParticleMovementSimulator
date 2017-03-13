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

#include "calculation_helpers.h"

void calculate_and_add_force(Particle* p, Particle* other_p, mpf_t* c, mpf_t* a1, mpf_t* a2, mpf_t* a3, mpf_t* a4, mpf_t* t1, mpf_t* t2, mpf_t* t3)
{

  // a2 = v^2

  // a3 = v*r

  // a4 = r_abs_pow_3

  mpf_add(*((*p).force_x), *((*p).force_x), *a1);
  mpf_add(*((*p).force_y), *((*p).force_y), *a1);
  mpf_add(*((*p).force_z), *((*p).force_z), *a1);
}

void calculate_forces_on_each_particle(ParticlePool *pool) 
{

  // pre-initialize several mpf_t variables for inner calculation-loops

  mpf_t* c; // speed of light 
  c = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*c);
  mpf_set_str(*c, SPEED_OF_LIGHT, 10);
 
  mpf_t* a1; // a1 = c^2 / 10^7
  a1 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*a1);
  mpf_mul(*a1, *c, *c);
  mpf_div_ui(*a1, *a1, 10^7);

  // a2, a3, a4, t1, t2, t3 freely available for use within loops
  mpf_t* a2;
  a2 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*a2);

  mpf_t* a3;
  a3 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*a3);

  mpf_t* a4;
  a4 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*a4);

  mpf_t* t1;
  t1 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*t1);

  mpf_t* t2;
  t2 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*t2);

  mpf_t* t3;
  t3 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*t3);

  for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
  {
    Particle* p = (*pool).particles[particle_index];
    mpf_set_ui(*((*p).force_x), 0);
    mpf_set_ui(*((*p).force_y), 0);
    mpf_set_ui(*((*p).force_z), 0);

    for( int other_particle_index = 0; other_particle_index <= (*pool).particles_initialized; other_particle_index++)
    {
      if( particle_index == other_particle_index)
      {
        continue;
      }
      Particle* other_p = (*pool).particles[other_particle_index];
      calculate_and_add_force(p, other_p, c, a1, a2, a3, a4, t1, t2, t3);
    }
  }
}

void move_particles_to_next_position(ParticlePool *pool, mpf_t StepSize)
{
}

