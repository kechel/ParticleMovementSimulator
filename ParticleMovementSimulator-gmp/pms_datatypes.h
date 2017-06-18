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

#ifndef PMS_DATATYPES_H 
#define PMS_DATATYPES_H

#include <gmp.h>

#define MAX_PARTICLES 100

typedef struct CalculationMemory
{
  mpf_t* c; // speed of light 
  mpf_t* qe; // electric charge of electron
  mpf_t* qp; // electric charge of positron
  mpf_t* me; // mass of electron
  mpf_t* c_square; // speed of light ^ 2
  mpf_t* c_square_over_10_pow_7; // c_square_over_10_pow_7 = c^2 / 10^7

  mpf_t* a1;
  mpf_t* a2;
  mpf_t* a3;
  mpf_t* a4;
  mpf_t* tmp1;
  mpf_t* tmp2;
  mpf_t* tmp3;
} CalculationMemory;

typedef struct Particle
{
  mpf_t* position_x;
  mpf_t* position_y;
  mpf_t* position_z;

  mpf_t* velocity_x;
  mpf_t* velocity_y;
  mpf_t* velocity_z;

  mpf_t* force_x;
  mpf_t* force_y;
  mpf_t* force_z;

  mpf_t* mass;

  mpf_t* charge;

} Particle;

typedef struct ParticlePool
{
  unsigned int current_particle_index;
  unsigned int current_field_index;
  unsigned int particles_initialized;
  Particle* particles[MAX_PARTICLES];
  CalculationMemory* cm;
} ParticlePool;

typedef struct ParticleHistory ParticleHistory;

struct ParticleHistory
{
    const char* line_for_gnuplot;
    ParticleHistory* next;
};

typedef struct ParticlePoolHistory
{
    ParticleHistory* particle_histories[MAX_PARTICLES];
    ParticleHistory* last_particle_histories[MAX_PARTICLES];
} ParticlePoolHistory;


#endif
