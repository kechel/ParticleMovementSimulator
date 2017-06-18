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

#ifndef CONFIGURATION_HELPERS_H
#define CONFIGURATION_HELPERS_H

#include <libconfig.h>
#include <gmp.h>
#include <time.h>

typedef struct pms_config
{
  const char* ConfigurationFilename;
  const char* ParticlePoolFilename;
  mpf_t StartTime;
  mpf_t EndTime;
  mpf_t StepSize;
  int mpf_set_default_prec;
  int SaveOnlyStartEndPoints;
  int SaveOnlyStepNumberMultiplesOf;
  int OutputPrecision;
  const char* OutputDirectory;
  time_t simulation_run_started;
  char* simulation_run_started_str;
  char* SimulationDirectory;
} pms_config;

pms_config load_configuration(int argc, char *argv[]);
void print_config_values(int precision, pms_config config);
#endif
