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
  char* grosse_zahl = "1.23456789E+50";
  char* kleine_zahl = "1.23456789E-50";
  mpf_set_str(*cm->tmp1, grosse_zahl, 10);
  mpf_set_str(*cm->tmp2, kleine_zahl, 10);
  mpf_add(*cm->tmp3, *cm->tmp1, *cm->tmp2);
  printf("Precision-Check:\n%s + %s = X\n", grosse_zahl, kleine_zahl);
  printf("X - %s = ", grosse_zahl);
  mpf_sub(*cm->tmp1, *cm->tmp3, *cm->tmp1);
  gmp_printf("%.*Fe\n", config.OutputPrecision, cm->tmp1);
  printf("--------------------------------------------------------------------------------\n");

  init_output_directory(config);

  ParticlePoolHistory pp_history;
  init_pp_history(&pp_history, &pool);

  printf("Starting Simulation..\n");

  mpf_t current_time;
  mpf_init_set(current_time, config.StartTime);

  unsigned long step_number = 0;

  
  mpf_t total_steps;
  mpf_init(total_steps);
  mpf_sub(total_steps, config.EndTime, config.StartTime);
  mpf_div(total_steps, total_steps, config.StepSize);

  mpf_t percentage_done;
  mpf_init(percentage_done);

  if( strcmp( config.ParticlePoolFilename, "gravity-test.csv") == 0)
  {
    printf("Sonderfall: gravity-test.csv\n");

    Particle* proton_links = pool.particles[0];
    Particle* elektron_links = pool.particles[1];
    Particle* proton_rechts = pool.particles[2];
    Particle* elektron_rechts = pool.particles[3];

    gmp_randstate_t random_state;
    gmp_randinit_default(random_state);
    mp_bitcnt_t bits_mantisse = 100;

    int anzahl_summen = 4;
    mpf_t* summen[anzahl_summen];
    for( int i = 0; i < anzahl_summen; i++)
    {
      summen[i] = (mpf_t*)malloc(sizeof(mpf_t));
      mpf_init(*summen[i]);
    }

    unsigned long v_elektron = 1500000; 
    unsigned long v_proton = v_elektron / 1832; 

    printf("Geschwindigkeit v_elektron: %ldm/s\n", v_elektron);
    printf("bei 1kg Wasserstoff erwartete Kraft Fx bei Entfernung 1m: 6.67430e-11N\n");

    // verschiedene Entfernungen durchspielen
    mpf_t* letzte_gesamtkraft = (mpf_t*)malloc(sizeof(mpf_t));
    mpf_init(*letzte_gesamtkraft);

    mpf_t* null_komma_fuenf = (mpf_t*)malloc(sizeof(mpf_t));
    mpf_init(*null_komma_fuenf);
    mpf_set_d(*null_komma_fuenf, 0.5);

    for( unsigned int entfernung_x = 1; entfernung_x <= 32; entfernung_x*=2)
    {
      printf("\nEntfernung %dm: \n", entfernung_x);
      mpf_set_ui(*elektron_rechts->position_x, entfernung_x);
      mpf_set_ui(*proton_rechts->position_x, entfernung_x);

      // Summierung der Kräfte für jede Entfernung neu berechnen
      for( int i = 0; i < anzahl_summen; i++)
      {
        mpf_set_ui(*summen[i], 0);
      }

      step_number = 0;
      while( step_number < 1000000000)
      {
        step_number += 1;

        // Elektron links bewegt sich zufaellig
        mpf_urandomb(*elektron_links->velocity_x, random_state, bits_mantisse);
        mpf_urandomb(*elektron_links->velocity_y, random_state, bits_mantisse);
        mpf_urandomb(*elektron_links->velocity_z, random_state, bits_mantisse);
        // damit die Geschwindigkeit mal + mal - ist, jeweils von der Zufallszahl 0..1 0,5 abziehen, dann ists eine Zufallszahl -0,5..+0,5
        mpf_sub(*elektron_links->velocity_x, *elektron_links->velocity_x, *null_komma_fuenf);
        mpf_sub(*elektron_links->velocity_y, *elektron_links->velocity_y, *null_komma_fuenf);
        mpf_sub(*elektron_links->velocity_z, *elektron_links->velocity_z, *null_komma_fuenf);

        normalisiere_geschwindigkeitsvektor(elektron_links, cm);

        mpf_mul_ui(*elektron_links->velocity_x, *elektron_links->velocity_x, v_elektron);
        mpf_mul_ui(*elektron_links->velocity_y, *elektron_links->velocity_y, v_elektron);
        mpf_mul_ui(*elektron_links->velocity_z, *elektron_links->velocity_z, v_elektron);


        // Elektron rechts bewegt sich zufaellig
        mpf_urandomb(*elektron_rechts->velocity_x, random_state, bits_mantisse);
        mpf_urandomb(*elektron_rechts->velocity_y, random_state, bits_mantisse);
        mpf_urandomb(*elektron_rechts->velocity_z, random_state, bits_mantisse);
        mpf_sub(*elektron_rechts->velocity_x, *elektron_rechts->velocity_x, *null_komma_fuenf);
        mpf_sub(*elektron_rechts->velocity_y, *elektron_rechts->velocity_y, *null_komma_fuenf);
        mpf_sub(*elektron_rechts->velocity_z, *elektron_rechts->velocity_z, *null_komma_fuenf);

        normalisiere_geschwindigkeitsvektor(elektron_rechts, cm);

        mpf_mul_ui(*elektron_rechts->velocity_x, *elektron_rechts->velocity_x, v_elektron);
        mpf_mul_ui(*elektron_rechts->velocity_y, *elektron_rechts->velocity_y, v_elektron);
        mpf_mul_ui(*elektron_rechts->velocity_z, *elektron_rechts->velocity_z, v_elektron);

        // Proton links bewegt sich zufaellig
        mpf_urandomb(*proton_links->velocity_x, random_state, bits_mantisse);
        mpf_urandomb(*proton_links->velocity_y, random_state, bits_mantisse);
        mpf_urandomb(*proton_links->velocity_z, random_state, bits_mantisse);
        mpf_sub(*proton_links->velocity_x, *proton_links->velocity_x, *null_komma_fuenf);
        mpf_sub(*proton_links->velocity_y, *proton_links->velocity_y, *null_komma_fuenf);
        mpf_sub(*proton_links->velocity_z, *proton_links->velocity_z, *null_komma_fuenf);

        normalisiere_geschwindigkeitsvektor(proton_links, cm);

        mpf_mul_ui(*proton_links->velocity_x, *proton_links->velocity_x, v_proton);
        mpf_mul_ui(*proton_links->velocity_y, *proton_links->velocity_y, v_proton);
        mpf_mul_ui(*proton_links->velocity_z, *proton_links->velocity_z, v_proton);

        // Proton rechts bewegt sich zufaellig
        mpf_urandomb(*proton_rechts->velocity_x, random_state, bits_mantisse);
        mpf_urandomb(*proton_rechts->velocity_y, random_state, bits_mantisse);
        mpf_urandomb(*proton_rechts->velocity_z, random_state, bits_mantisse);
        mpf_sub(*proton_rechts->velocity_x, *proton_rechts->velocity_x, *null_komma_fuenf);
        mpf_sub(*proton_rechts->velocity_y, *proton_rechts->velocity_y, *null_komma_fuenf);
        mpf_sub(*proton_rechts->velocity_z, *proton_rechts->velocity_z, *null_komma_fuenf);

        normalisiere_geschwindigkeitsvektor(proton_rechts, cm);

        mpf_mul_ui(*proton_rechts->velocity_x, *proton_rechts->velocity_x, v_proton);
        mpf_mul_ui(*proton_rechts->velocity_y, *proton_rechts->velocity_y, v_proton);
        mpf_mul_ui(*proton_rechts->velocity_z, *proton_rechts->velocity_z, v_proton);

        // Kraefte durch entferntes Elektron
        
        calculate_and_set_force(proton_links, elektron_rechts, cm);
        mpf_add(*summen[0], *summen[0], *proton_links->force_x);

        calculate_and_set_force(elektron_links, elektron_rechts, cm);
        mpf_add(*summen[1], *summen[1], *elektron_links->force_x);

        // Kraefte durch entferntes Proton

        calculate_and_set_force(elektron_links, proton_rechts, cm);
        mpf_add(*summen[2], *summen[2], *elektron_links->force_x);

        calculate_and_set_force(proton_links, proton_rechts, cm);
        mpf_add(*summen[3], *summen[3], *proton_links->force_x);

        if( step_number % config.SaveOnlyStepNumberMultiplesOf == 0)
        {
          printf("mittl. Fx nach %7ld Iter.: ", step_number);
          // gmp_printf("P durch e %.*Fe ", config.OutputPrecision, *summen[0]);
          // gmp_printf("e durch e %.*Fe ", config.OutputPrecision, *summen[1]);
          // gmp_printf("e durch P %.*Fe ", config.OutputPrecision, *summen[2]);
          // gmp_printf("P durch P %.*Fe ", config.OutputPrecision, *summen[3]);

          // Gesamtkraft Fx
          mpf_set_ui(*cm->tmp1, 0);
          mpf_add(*cm->tmp1, *cm->tmp1, *summen[0]);
          mpf_add(*cm->tmp1, *cm->tmp1, *summen[1]);
          mpf_add(*cm->tmp1, *cm->tmp1, *summen[2]);
          mpf_add(*cm->tmp1, *cm->tmp1, *summen[3]);

          // Durchschnittliche Kraft Fx pro Step
          mpf_div_ui(*cm->tmp1, *cm->tmp1, step_number + 1);

          gmp_printf("%.*FeN ", config.OutputPrecision, cm->tmp1);

          // rechne die Gewichtskraft von 1kg Masse links auf ein kg Masse rechts hoch
          mpf_add(*cm->tmp2, *cm->mp, *cm->me); // gesamtmasse links
          mpf_ui_div(*cm->tmp3, 1, *cm->tmp2); // anzahl teilchen in 1kg
          mpf_mul(*cm->tmp2, *cm->tmp1, *cm->tmp3); // gesamtkraft hochgerechnet bei 1kg Teilchen

          gmp_printf("Kraft auf 1kg: %.*FeN", config.OutputPrecision, cm->tmp2);
          // gmp_printf(" (%.*Fe N * ", config.OutputPrecision, cm->tmp1);
          // gmp_printf("%.*Fe Teilchen)", config.OutputPrecision, cm->tmp3);

          printf("\n");

          // print_particle_pool_values(config.OutputPrecision, &pool);
        }
      }

      if( entfernung_x > 1)
      {
        mpf_div(*cm->tmp3, *letzte_gesamtkraft, *cm->tmp1); // letztes Gesamtergebnis durch das neue Gesamtergebnis
        gmp_printf("Faktor der Gesamtkraft zur vorherigen Entfernung: %.*Fe\n", config.OutputPrecision, cm->tmp3);
      }
      mpf_set(*letzte_gesamtkraft, *cm->tmp1);
    }
  }
  else
  {
    while( mpf_cmp(current_time, config.EndTime) < 0)
    {
      calculate_forces_on_each_particle(&pool, cm); 
      move_particles_to_next_position(&pool, config.StepSize, cm);

      mpf_add(current_time, current_time, config.StepSize);

      step_number += 1;

      if( step_number % config.SaveOnlyStepNumberMultiplesOf == 0)
      {
        mpf_ui_div( percentage_done, step_number, total_steps);
        printf("--------------------------------------------------------------------------------\n");
        gmp_printf("%lu/%.*Fe steps done (%.*Fe%)\n", step_number, 3, total_steps, 3, percentage_done);
        print_particle_pool_values(config.OutputPrecision, &pool);
        store_status_for_gnuplot(&pp_history, &pool);
      }
    }
  }

  printf("--------------------------------------------------------------------------------\n");
  printf("All Steps DONE: %lu\n", step_number);
  // print_particle_pool_values(config.OutputPrecision, &pool);
  //print_history(&pp_history, &pool);
  write_history_to_log(&pp_history, &pool, config);

  free_calculation_memory(cm);

  chdir(config.SimulationDirectory);
  execl("/usr/bin/gnuplot", "gnuplot", "gnuplot.script", "-", (char*) NULL);

  return 0;
}


