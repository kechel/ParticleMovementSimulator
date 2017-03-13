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


#include <gmp.h>
#include <stdio.h>
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
  pool.current_particle_index = 0;
  pool.current_field_index = 0;

  load_particle_pool_from_csv(config.ParticlePoolFilename, &pool);
  printf("--------------------------------------------------------------------------------\n");
  print_particle_pool_values(config.OutputPrecision, &pool);
  printf("--------------------------------------------------------------------------------\n");

  printf("Starting Simulation..\n");

  unsigned long step_number = 0;

  mpf_t current_time;
  mpf_init_set(current_time, config.StartTime);

  do
  {
    step_number += 1;
    if( step_number % config.SaveOnlyStepNumberMultiplesOf == 0)
    {
      printf("--------------------------------------------------------------------------------\n");
      printf("Steps calculated: %lu\n", step_number);
      print_particle_pool_values(config.OutputPrecision, &pool);
    }

    calculate_forces_on_each_particle(&pool); 
    move_particles_to_next_position(&pool, config.StepSize);

    mpf_add(current_time, current_time, config.StepSize);
  } while( mpf_cmp(current_time, config.EndTime) <= 0);


  return 0;
}


