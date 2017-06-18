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

#ifndef LOAD_PARTICLE_POOL_FROM_CSV_H
#define LOAD_PARTICLE_POOL_FROM_CSV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <csv.h>
#include "pms_datatypes.h"

int load_particle_pool_from_csv(const char* filename, ParticlePool* pool, CalculationMemory* cm);
void print_particle_values(int precision, const Particle* p);
void print_particle_pool_values(int precision, const ParticlePool* pool);

#endif
