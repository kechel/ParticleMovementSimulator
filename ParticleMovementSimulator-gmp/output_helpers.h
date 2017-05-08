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

#ifndef OUTPUT_HELPERS_H
#define OUTPUT_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "pms_datatypes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "configuration_helpers.h"

void write_history_to_log(ParticlePoolHistory* pp_history, ParticlePool* pool, pms_config config);
void print_history(ParticlePoolHistory* pp_history, ParticlePool* pool);
void store_status_for_gnuplot(ParticlePoolHistory* pp_history, ParticlePool* pool);
void init_output_directory(pms_config config);
const char* get_line_for_gnuplot(const Particle* p);
void init_pp_history(ParticlePoolHistory* pp_history, ParticlePool* pool);

#endif
