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

#ifndef CALCULATION_HELPERS_H
#define CALCULATION_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "pms_datatypes.h"

#define SPEED_OF_LIGHT "299792458"
#define CHARGE_OF_ELECTRON "-1.6021766208E-19"
#define CHARGE_OF_POSITRON "1.6021766208E-19"
#define MASS_OF_ELECTRON "9.10938356E-31"

void calculate_forces_on_each_particle(ParticlePool *pool, CalculationMemory* cm);
void move_particles_to_next_position(ParticlePool *pool, mpf_t StepSize, CalculationMemory* cm);
CalculationMemory* create_calculation_memory();
void free_calculation_memory(CalculationMemory* cm);

#endif
