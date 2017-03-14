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

// calculates the force on p due to dynamic movement, distance and charge of other_p
// TODO: missing gravity due to other_p
void calculate_and_add_force(Particle* p, Particle* other_p, mpf_t* c_square, mpf_t* c_square_over_10_pow_7, mpf_t* a1, mpf_t* a2, mpf_t* a3, mpf_t* a4, mpf_t* t1, mpf_t* t2, mpf_t* t3)
{
  // *****************************************************
  // c_square = c^2 
  // -> c_square ist given pre-calculated with correct value

  // *****************************************************
  // c_square_over_10_pow_7 = c^2 / 10^7
  // -> c_square_over_10_pow_7 ist given pre-calculated with correct value
  
  // *****************************************************
  // a1 = v^2 / c^2 = (v1x-v0x)^2  + (v1y-v0y)^2 +(v1z-v0z)^2
  mpf_sub(*t1, *((*p).velocity_x), *((*other_p).velocity_x));
  mpf_mul(*t1, *t1, *t1);

  mpf_sub(*t2, *((*p).velocity_y), *((*other_p).velocity_y));
  mpf_mul(*t2, *t2, *t2);

  mpf_sub(*t3, *((*p).velocity_z), *((*other_p).velocity_z));
  mpf_mul(*t3, *t3, *t3);

  mpf_add(*a1, *t1, *t2);
  mpf_add(*a1, *a1, *t3);

  mpf_div(*a1, *a1, *c_square);

  // *****************************************************
  // a2 = v*r / c^2 = (v1x-v0z)*(r1x-r0z) + (v1y-v0y)*(r1y-r0y) + (v1z-v0z)*(r1z-r0z)
  mpf_sub(*t1, *((*p).velocity_x), *((*other_p).velocity_x));
  mpf_sub(*t2, *((*p).position_x), *((*other_p).position_x));
  mpf_mul(*a2, *t1, *t2);

  mpf_sub(*t1, *((*p).velocity_y), *((*other_p).velocity_y));
  mpf_sub(*t2, *((*p).position_y), *((*other_p).position_y));
  mpf_mul(*t1, *t1, *t2);
  mpf_add(*a2, *a2, *t1);

  mpf_sub(*t1, *((*p).velocity_z), *((*other_p).velocity_z));
  mpf_sub(*t2, *((*p).position_z), *((*other_p).position_z));
  mpf_mul(*t1, *t1, *t2);
  mpf_add(*a2, *a2, *t1);

  mpf_div(*a2, *a2, *c_square);

  // *****************************************************
  // a3 = r_abs_pow_3 = ((r1x-r0x)^2 + (r1y-r0y)^2 + (r1z-r0z)^2)^(3/2)
  mpf_sub(*t1, *((*p).position_x), *((*other_p).position_x));
  mpf_mul(*t1, *t1, *t1);

  mpf_sub(*t2, *((*p).position_y), *((*other_p).position_y));
  mpf_mul(*t2, *t2, *t2);

  mpf_sub(*t3, *((*p).position_z), *((*other_p).position_z));
  mpf_mul(*t3, *t3, *t3);

  mpf_add(*a3, *t1, *t2);
  mpf_add(*a3, *a3, *t3);

  mpf_pow_ui(*a3, *a3, 3);
  mpf_sqrt(*a3, *a3);

  // *****************************************************
  // a4 = c^2/10^7 * q0*q1 / |r|^3
  // a4 = c_square_over_10_pow_7 * q0*q1 / a3
  mpf_mul(*a4, *c_square_over_10_pow_7, *((*p).charge));
  mpf_mul(*a4, *a4, *((*other_p).charge));
  mpf_div(*a4, *a4, *a3);

  // *****************************************************
  // Fx = a4 * ((r1x-r0x) + (r1xr0x)*a1 - (v1x-v0x)*a2)
  // t1 = r1x-r0x
  mpf_sub(*t1, *((*p).position_x), *((*other_p).position_x)); // hatten wir schonmal!

  // t2 = (r1x-r0x)*a1
  mpf_mul(*t2, *t1, *a1);
  
  // t3 = (v1x-v0x)*a2
  mpf_sub(*t3, *((*p).velocity_x), *((*other_p).velocity_x)); // hatten wir schonmal!
  mpf_mul(*t3, *t3, *a2);

  mpf_add(*t1, *t1, *t2);
  mpf_sub(*t1, *t1, *t3);

  mpf_mul(*((*p).force_x), *a4, *t1);


  // *****************************************************
  // Fy = a4 * ((r1y-r0y) + (r1yr0y)*a1 - (v1y-v0y)*a2)
  // t1 = r1y-r0y
  mpf_sub(*t1, *((*p).position_y), *((*other_p).position_y)); // hatten wir schonmal!

  // t2 = (r1y-r0y)*a1
  mpf_mul(*t2, *t1, *a1);
  
  // t3 = (v1y-v0y)*a2
  mpf_sub(*t3, *((*p).velocity_y), *((*other_p).velocity_y)); // hatten wir schonmal!
  mpf_mul(*t3, *t3, *a2);

  mpf_add(*t1, *t1, *t2);
  mpf_sub(*t1, *t1, *t3);

  mpf_mul(*((*p).force_y), *a4, *t1);


  // *****************************************************
  // Fz = a4 * ((r1z-r0z) + (r1zr0z)*a1 - (v1z-v0z)*a2)
  // t1 = r1z-r0z
  mpf_sub(*t1, *((*p).position_z), *((*other_p).position_z)); // hatten wir schonmal!

  // t2 = (r1z-r0z)*a1
  mpf_mul(*t2, *t1, *a1);
  
  // t3 = (v1z-v0z)*a2
  mpf_sub(*t3, *((*p).velocity_z), *((*other_p).velocity_z)); // hatten wir schonmal!
  mpf_mul(*t3, *t3, *a2);

  mpf_add(*t1, *t1, *t2);
  mpf_sub(*t1, *t1, *t3);

  mpf_mul(*((*p).force_z), *a4, *t1);

}

void calculate_forces_on_each_particle(ParticlePool *pool) 
{

  // pre-initialize several mpf_t variables for inner calculation-loops

  mpf_t* c; // speed of light 
  c = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*c);
  mpf_set_str(*c, SPEED_OF_LIGHT, 10);

  mpf_t* c_square; // speed of light ^ 2
  c_square = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*c_square);
  mpf_mul(*c_square, *c, *c);
 
  mpf_t* c_square_over_10_pow_7; // c_square_over_10_pow_7 = c^2 / 10^7
  c_square_over_10_pow_7 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*c_square_over_10_pow_7);
  mpf_mul(*c_square_over_10_pow_7, *c, *c);
  mpf_div_ui(*c_square_over_10_pow_7, *c_square_over_10_pow_7, 10^7);

  // a1, a2, a3, a4, t1, t2, t3 freely available for use within loops
  mpf_t* a1;
  a1 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*a1);

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
      calculate_and_add_force(p, other_p, c_square, c_square_over_10_pow_7, a1, a2, a3, a4, t1, t2, t3);
    }
  }
}

void move_particles_to_next_position(ParticlePool *pool, mpf_t StepSize)
{
}

