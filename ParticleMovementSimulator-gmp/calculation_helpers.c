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
    along with PMS with GMP.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include "calculation_helpers.h"
#include "load_particle_pool_from_csv.h"

// calculates the force on p due to dynamic movement, distance and charge of other_p
// TODO: missing gravity due to other_p
void calculate_and_add_force(Particle* p, Particle* other_p, CalculationMemory *cm)
{

  mpf_t* c_square = (*cm).c_square;
  mpf_t* c_square_over_10_pow_7 = (*cm).c_square_over_10_pow_7;
  mpf_t* tmp1 = (*cm).tmp1;
  mpf_t* tmp2 = (*cm).tmp2;
  mpf_t* tmp3 = (*cm).tmp3;
  mpf_t* a1 = (*cm).a1;
  mpf_t* a2 = (*cm).a2;
  mpf_t* a3 = (*cm).a3;
  mpf_t* a4 = (*cm).a4;

  // *****************************************************
  // c_square = c^2 
  // -> c_square ist given pre-calculated with correct value

  // *****************************************************
  // c_square_over_10_pow_7 = c^2 / 10^7
  // -> c_square_over_10_pow_7 ist given pre-calculated with correct value
  
  // *****************************************************
  // a1 = v^2 / c^2 = (v1x-v0x)^2  + (v1y-v0y)^2 +(v1z-v0z)^2
  mpf_sub(*tmp1, *((*p).velocity_x), *((*other_p).velocity_x));
  mpf_mul(*tmp1, *tmp1, *tmp1);

  mpf_sub(*tmp2, *((*p).velocity_y), *((*other_p).velocity_y));
  mpf_mul(*tmp2, *tmp2, *tmp2);

  mpf_sub(*tmp3, *((*p).velocity_z), *((*other_p).velocity_z));
  mpf_mul(*tmp3, *tmp3, *tmp3);

  mpf_add(*a1, *tmp1, *tmp2);
  mpf_add(*a1, *a1, *tmp3);

  mpf_div(*a1, *a1, *c_square);

  // *****************************************************
  // a2 = v*r / c^2 = (v1x-v0z)*(r1x-r0z) + (v1y-v0y)*(r1y-r0y) + (v1z-v0z)*(r1z-r0z)
  mpf_sub(*tmp1, *((*p).velocity_x), *((*other_p).velocity_x));
  mpf_sub(*tmp2, *((*p).position_x), *((*other_p).position_x));
  mpf_mul(*a2, *tmp1, *tmp2);

  mpf_sub(*tmp1, *((*p).velocity_y), *((*other_p).velocity_y));
  mpf_sub(*tmp2, *((*p).position_y), *((*other_p).position_y));
  mpf_mul(*tmp1, *tmp1, *tmp2);
  mpf_add(*a2, *a2, *tmp1);

  mpf_sub(*tmp1, *((*p).velocity_z), *((*other_p).velocity_z));
  mpf_sub(*tmp2, *((*p).position_z), *((*other_p).position_z));
  mpf_mul(*tmp1, *tmp1, *tmp2);
  mpf_add(*a2, *a2, *tmp1);

  mpf_div(*a2, *a2, *c_square);

  // *****************************************************
  // a3 = r_abs_pow_3 = ((r1x-r0x)^2 + (r1y-r0y)^2 + (r1z-r0z)^2)^(3/2)
  mpf_sub(*tmp1, *((*p).position_x), *((*other_p).position_x));
  mpf_mul(*tmp1, *tmp1, *tmp1);

  mpf_sub(*tmp2, *((*p).position_y), *((*other_p).position_y));
  mpf_mul(*tmp2, *tmp2, *tmp2);

  mpf_sub(*tmp3, *((*p).position_z), *((*other_p).position_z));
  mpf_mul(*tmp3, *tmp3, *tmp3);

  mpf_add(*a3, *tmp1, *tmp2);
  mpf_add(*a3, *a3, *tmp3);

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
  // tmp1 = r1x-r0x
  mpf_sub(*tmp1, *((*p).position_x), *((*other_p).position_x)); // hatten wir schonmal!

  // tmp2 = (r1x-r0x)*a1
  mpf_mul(*tmp2, *tmp1, *a1);
  
  // tmp3 = (v1x-v0x)*a2
  mpf_sub(*tmp3, *((*p).velocity_x), *((*other_p).velocity_x)); // hatten wir schonmal!
  mpf_mul(*tmp3, *tmp3, *a2);

  mpf_add(*tmp1, *tmp1, *tmp2);
  mpf_sub(*tmp1, *tmp1, *tmp3);

  mpf_mul(*((*p).force_x), *a4, *tmp1);


  // *****************************************************
  // Fy = a4 * ((r1y-r0y) + (r1yr0y)*a1 - (v1y-v0y)*a2)
  // tmp1 = r1y-r0y
  mpf_sub(*tmp1, *((*p).position_y), *((*other_p).position_y)); // hatten wir schonmal!

  // tmp2 = (r1y-r0y)*a1
  mpf_mul(*tmp2, *tmp1, *a1);
  
  // tmp3 = (v1y-v0y)*a2
  mpf_sub(*tmp3, *((*p).velocity_y), *((*other_p).velocity_y)); // hatten wir schonmal!
  mpf_mul(*tmp3, *tmp3, *a2);

  mpf_add(*tmp1, *tmp1, *tmp2);
  mpf_sub(*tmp1, *tmp1, *tmp3);

  mpf_mul(*((*p).force_y), *a4, *tmp1);


  // *****************************************************
  // Fz = a4 * ((r1z-r0z) + (r1zr0z)*a1 - (v1z-v0z)*a2)
  // tmp1 = r1z-r0z
  mpf_sub(*tmp1, *((*p).position_z), *((*other_p).position_z)); // hatten wir schonmal!

  // tmp2 = (r1z-r0z)*a1
  mpf_mul(*tmp2, *tmp1, *a1);
  
  // tmp3 = (v1z-v0z)*a2
  mpf_sub(*tmp3, *((*p).velocity_z), *((*other_p).velocity_z)); // hatten wir schonmal!
  mpf_mul(*tmp3, *tmp3, *a2);

  mpf_add(*tmp1, *tmp1, *tmp2);
  mpf_sub(*tmp1, *tmp1, *tmp3);

  mpf_mul(*((*p).force_z), *a4, *tmp1);

}

CalculationMemory* create_calculation_memory()
{
  CalculationMemory* cm = (CalculationMemory*)malloc(sizeof(CalculationMemory));

  (*cm).c = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).c);
  mpf_set_str(*(*cm).c, SPEED_OF_LIGHT, 10);

  (*cm).qe = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).qe);
  mpf_set_str(*(*cm).qe, CHARGE_OF_ELECTRON, 10);

  (*cm).qp = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).qp);
  mpf_set_str(*(*cm).qp, CHARGE_OF_POSITRON, 10);

  (*cm).me = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).me);
  mpf_set_str(*(*cm).me, MASS_OF_ELECTRON, 10);

  (*cm).c_square = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).c_square);
  mpf_mul(*(*cm).c_square, *(*cm).c, *(*cm).c);
 
  (*cm).c_square_over_10_pow_7 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).c_square_over_10_pow_7);
  mpf_mul(*(*cm).c_square_over_10_pow_7, *(*cm).c, *(*cm).c);
  mpf_div_ui(*(*cm).c_square_over_10_pow_7, *(*cm).c_square_over_10_pow_7, 10^7);

  // a1, a2, a3, a4, tmp1, tmp2, tmp3 freely available for use within loops
  (*cm).a1 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).a1);

  (*cm).a2 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).a2);

  (*cm).a3 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).a3);

  (*cm).a4 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).a4);

  (*cm).tmp1 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).tmp1);

  (*cm).tmp2 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).tmp2);

  (*cm).tmp3 = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*cm).tmp3);

  return cm;
}

void free_calculation_memory(CalculationMemory* cm)
{
  mpf_clear(*((*cm).c));
  free(cm);
}

void calculate_forces_on_each_particle(ParticlePool* pool, CalculationMemory* cm) 
{
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
      calculate_and_add_force(p, other_p, cm);
    }
  }
}

void move_particles_to_next_position(ParticlePool* pool, mpf_t StepSize, CalculationMemory* cm)
{
    mpf_t* next_vx = (*cm).a1;
    mpf_t* next_vy = (*cm).a2;
    mpf_t* next_vz = (*cm).a3;
    mpf_t* tmp1 = (*cm).tmp1;
    //mpf_t* tmp2 = (*cm).tmp2;
    //mpf_t* tmp3 = (*cm).tmp3;

    for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
    {
        Particle* p = (*pool).particles[particle_index];

        // check for errors due to discretion
        // mpf_cmp: Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, and a negative value if op1 < op2.
        if( mpf_cmp_d(*((*p).force_x), 1e-18) > 0 || mpf_cmp_d(*((*p).force_x), -1e-18) < 0)
        {
          printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
          printf("Warning: very high forces");
          print_particle_pool_values(5, pool);
          printf("YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n");
        }

        // next Vx = Vx + Fx * StepSize / Mass
        mpf_mul(*tmp1, *((*p).force_x), StepSize);
        mpf_div(*tmp1, *tmp1, *((*p).mass));
        mpf_add(*next_vx, *tmp1, *((*p).velocity_x));

        // Next Px = Px + ((Vx+Vx_next)/2)*StepSize
        mpf_add(*tmp1, *next_vx, *((*p).velocity_x));
        mpf_div_ui(*tmp1, *tmp1, 2);
        mpf_mul(*tmp1, *tmp1, StepSize);

        // set pool values to next step
        mpf_add(*((*p).position_x), *((*p).position_x), *tmp1);
        mpf_set(*((*p).velocity_x), *next_vx);

        // next Vy = Vy + Fy * StepSize / Mass
        mpf_mul(*tmp1, *((*p).force_y), StepSize);
        mpf_div(*tmp1, *tmp1, *((*p).mass));
        mpf_add(*next_vy, *tmp1, *((*p).velocity_y));

        // Next Py = Py + ((Vy+Vy_next)/2)*StepSize
        mpf_add(*tmp1, *next_vy, *((*p).velocity_y));
        mpf_div_ui(*tmp1, *tmp1, 2);
        mpf_mul(*tmp1, *tmp1, StepSize);

        // set pool values to next step
        mpf_add(*((*p).position_y), *((*p).position_y), *tmp1);
        mpf_set(*((*p).velocity_y), *next_vy);

        // next Vz = Vz + Fz * StepSize / Mass
        mpf_mul(*tmp1, *((*p).force_z), StepSize);
        mpf_div(*tmp1, *tmp1, *((*p).mass));
        mpf_add(*next_vz, *tmp1, *((*p).velocity_z));

        // Next Pz = Pz + ((Vz+Vz_next)/2)*StepSize
        mpf_add(*tmp1, *next_vz, *((*p).velocity_z));
        mpf_div_ui(*tmp1, *tmp1, 2);
        mpf_mul(*tmp1, *tmp1, StepSize);

        // set pool values to next step
        mpf_add(*((*p).position_z), *((*p).position_z), *tmp1);
        mpf_set(*((*p).velocity_z), *next_vz);
    }
}

