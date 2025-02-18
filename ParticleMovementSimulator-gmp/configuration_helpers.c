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

#include "configuration_helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  const char* tmp_buffer;

  mpf_init(pc.StartTime);
  config_lookup_string(&cfg, "simulation.StartTime", &tmp_buffer);
  mpf_set_str( pc.StartTime, tmp_buffer, 10);

  mpf_init(pc.EndTime);
  config_lookup_string(&cfg, "simulation.EndTime", &tmp_buffer);
  mpf_set_str( pc.EndTime, tmp_buffer, 10);

  mpf_init(pc.StepSize);
  config_lookup_string(&cfg, "simulation.StepSize", &tmp_buffer);
  mpf_set_str( pc.StepSize, tmp_buffer, 10);

  config_lookup_int(&cfg, "simulation.mpf_set_default_prec", &pc.mpf_set_default_prec);
  config_lookup_int(&cfg, "simulation.logging.SaveOnlyStartEndPoints", &pc.SaveOnlyStartEndPoints);
  config_lookup_int(&cfg, "simulation.logging.SaveOnlyStepNumberMultiplesOf", &pc.SaveOnlyStepNumberMultiplesOf);
  config_lookup_int(&cfg, "simulation.logging.OutputPrecision", &pc.OutputPrecision);

  config_lookup_string(&cfg, "simulation.logging.OutputDirectory", &pc.OutputDirectory);

  pc.simulation_run_started = time(NULL);
  pc.simulation_run_started_str = malloc(26);
  struct tm* tm_info;
  tm_info = localtime(&pc.simulation_run_started);
  strftime(pc.simulation_run_started_str, 26, "%Y-%m-%d_%H:%M:%S", tm_info);
  pc.SimulationDirectory = malloc(strlen(pc.OutputDirectory) + strlen(pc.simulation_run_started_str) + 2);
  sprintf(pc.SimulationDirectory, "%s/%s/", pc.OutputDirectory, pc.simulation_run_started_str);

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
  printf("OutputDirectory:                %s\n", config.OutputDirectory);
  printf("Simulation run started at:      %s\n", config.simulation_run_started_str);
  printf("SimulationDirectory:            %s\n", config.SimulationDirectory);
};


