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


#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include "load_particle_pool_from_csv.h"
#include "output_helpers.h"
#include "configuration_helpers.h"
#include "calculation_helpers.h"

int main(int argc, char *argv[])
{


  pms_config config = load_configuration(argc, argv);
  printf("--------------------------------------------------------------------------------\n");
  print_config_values(config.OutputPrecision, config);
  printf("--------------------------------------------------------------------------------\n");

  mpf_set_default_prec(config.mpf_set_default_prec); 

  ParticlePool pool;
  CalculationMemory* cm = create_calculation_memory();
  pool.cm = cm;
  pool.current_particle_index = 0;
  pool.current_field_index = 0;

  load_particle_pool_from_csv(config.ParticlePoolFilename, &pool, cm);
  printf("--------------------------------------------------------------------------------\n");
  print_particle_pool_values(config.OutputPrecision, &pool);
  printf("--------------------------------------------------------------------------------\n");

  init_output_directory(config);

  ParticlePoolHistory pp_history;
  init_pp_history(&pp_history, &pool);

  printf("Starting Simulation..\n");

  mpf_t current_time;
  mpf_init_set(current_time, config.StartTime);

  unsigned long step_number = 0;

  while( mpf_cmp(current_time, config.EndTime) < 0)
  {
    calculate_forces_on_each_particle(&pool, cm); 
    move_particles_to_next_position(&pool, config.StepSize, cm);

    mpf_add(current_time, current_time, config.StepSize);

    step_number += 1;

    if( step_number % config.SaveOnlyStepNumberMultiplesOf == 0)
    {
      printf("--------------------------------------------------------------------------------\n");
      printf("Status after %lu steps\n", step_number);
      print_particle_pool_values(config.OutputPrecision, &pool);
      printf("done %lu steps\n", step_number);
      store_status_for_gnuplot(&pp_history, &pool);
    }
  }


  printf("--------------------------------------------------------------------------------\n");
  printf("All Steps DONE: %lu\n", step_number);
  print_particle_pool_values(config.OutputPrecision, &pool);
  //print_history(&pp_history, &pool);
  write_history_to_log(&pp_history, &pool, config);

  free_calculation_memory(cm);

  chdir(config.SimulationDirectory);
  execl("/usr/bin/gnuplot", "gnuplot", "gnuplot.script", "-", (char*) NULL);

  return 0;
}


