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

#include "configuration_helpers.h"

#include <stdio.h>
#include <stdlib.h>

pms_config load_configuration(int argc, char *argv[])
{
  if( argc != 2)
  {
    printf("Please provide configuration filename as parameter.\n");
    exit(-1);
  }

  pms_config pc;

  config_t cfg;
  config_init(&cfg);

  pc.ConfigurationFilename = argv[1];

  printf("Loading configuration file %s ..\n", pc.ConfigurationFilename);
  if(! config_read_file(&cfg, pc.ConfigurationFilename))
  { 
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    exit(-2);
  }

  config_lookup_string(&cfg, "simulation.ParticlePoolFilename", &pc.ParticlePoolFilename);

  double tmp_double;

  mpf_init(pc.StartTime);
  config_lookup_float(&cfg, "simulation.StartTime", &tmp_double);
  mpf_set_d( pc.StartTime, tmp_double);

  mpf_init(pc.EndTime);
  config_lookup_float(&cfg, "simulation.EndTime", &tmp_double);
  mpf_set_d( pc.EndTime, tmp_double);

  mpf_init(pc.StepSize);
  config_lookup_float(&cfg, "simulation.StepSize", &tmp_double);
  mpf_set_d( pc.StepSize, tmp_double);

  config_lookup_int(&cfg, "simulation.mpf_set_default_prec", &pc.mpf_set_default_prec);
  config_lookup_int(&cfg, "simulation.logging.SaveOnlyStartEndPoints", &pc.SaveOnlyStartEndPoints);
  config_lookup_int(&cfg, "simulation.logging.SaveOnlyStepNumberMultiplesOf", &pc.SaveOnlyStepNumberMultiplesOf);
  config_lookup_int(&cfg, "simulation.logging.OutputPrecision", &pc.OutputPrecision);

  return pc;
};


void print_config_values(int precision, pms_config config)
{
  printf("Dumping Configuration:\n");
  printf("ParticlePoolFilename:           %s\n", config.ParticlePoolFilename);

  gmp_printf("StartTime                       %.*Fe\n", precision, config.StartTime);
  gmp_printf("EndTime                         %.*Fe\n", precision, config.EndTime);
  gmp_printf("StepSize                        %.*Fe\n", precision, config.StepSize);

  printf("mpf_set_default_prec:           %d\n", config.mpf_set_default_prec);
  printf("SaveOnlyStartEndPoints:         %d\n", config.SaveOnlyStartEndPoints);
  printf("SaveOnlyStepNumberMultiplesOf:  %d\n", config.SaveOnlyStepNumberMultiplesOf);
  printf("OutputPrecision:                %d\n", config.OutputPrecision);
};


