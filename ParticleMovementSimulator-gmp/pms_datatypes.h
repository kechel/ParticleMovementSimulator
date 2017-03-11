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

#ifndef PMS_DATATYPES_H 
#define PMS_DATATYPES_H

#include <gmp.h>

#define MAX_PARTICLES 100

typedef struct Particle
{
  mpf_t* position_x;
  mpf_t* position_y;
  mpf_t* position_z;

  mpf_t* velocity_x;
  mpf_t* velocity_y;
  mpf_t* velocity_z;

  mpf_t* mass;

  mpf_t* charge;

} Particle;

typedef struct ParticlePool
{
  unsigned int current_particle_index;
  unsigned int current_field_index;
  unsigned int particles_initialized;
  Particle* particles[MAX_PARTICLES];
} ParticlePool;

#endif
