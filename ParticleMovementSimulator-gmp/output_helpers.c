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

#include "output_helpers.h"
#include <string.h>

void print_history(ParticlePoolHistory* pp_history, ParticlePool* pool)
{
  printf("ParticlePoolHistory:\n");

  for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
  {
    ParticleHistory ph = *((*pp_history).particle_histories[particle_index]);
    printf("\n\nParticle %d History:\n", particle_index);
    printf("%s", ph.line_for_gnuplot);
    while( ph.next != NULL)
    {
        ph = *ph.next;
        printf("%s", ph.line_for_gnuplot);
    }
  }
}

void write_history_to_log(ParticlePoolHistory* pp_history, ParticlePool* pool, pms_config config)
{
  char filename[1000];
  for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
  {
    sprintf( filename, "%s%d", config.SimulationDirectory, particle_index);
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file %s\n", filename);
        continue;
    }

    ParticleHistory ph = *((*pp_history).particle_histories[particle_index]);
    fprintf(f, "%s", ph.line_for_gnuplot);
    while( ph.next != NULL)
    {
        ph = *ph.next;
        fprintf(f, "%s", ph.line_for_gnuplot);
    }

    fclose(f);
  }

  // gnuplot.script:
  //splot "0" with lines, "1" with lines, "2" with lines, "3" with lines
  sprintf( filename, "%sgnuplot.script", config.SimulationDirectory);
  FILE *f = fopen(filename, "w");
  if (f == NULL)
  {
    printf("Error opening file %s\n", filename);
    return;
  }

  fprintf(f, "splot ");
  for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
  {
    fprintf(f, "\"%d\" with lines", particle_index);
    if( particle_index < (*pool).particles_initialized)
    {
        fprintf(f, ", ");
    }
  }
  fclose(f);
}

void store_status_for_gnuplot(ParticlePoolHistory* pp_history, ParticlePool* pool)
{
  for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
  {
    Particle* p = (*pool).particles[particle_index];

    ParticleHistory* previous = (*pp_history).last_particle_histories[particle_index];
    ParticleHistory* next = malloc(sizeof(ParticleHistory));
    (*next).line_for_gnuplot = get_line_for_gnuplot(p);
    (*next).next = NULL;

    (*previous).next = next;
    (*pp_history).last_particle_histories[particle_index] = next;
  }
}

const char* get_line_for_gnuplot(const Particle* p)
{
    char* buffer = malloc(4096);
    int precision = 5;

    gmp_sprintf(buffer, "%.*Fe %.*Fe %.*Fe %.*Fe %.*Fe %.*Fe %.*Fe %.*Fe %.*Fe\n",
        precision, *((*p).position_x),
        precision, *((*p).position_y),
        precision, *((*p).position_z),
        precision, *((*p).velocity_x),
        precision, *((*p).velocity_y),
        precision, *((*p).velocity_z),
        precision, *((*p).force_x),
        precision, *((*p).force_y),
        precision, *((*p).force_z));
    return buffer;
}

void init_pp_history(ParticlePoolHistory* pp_history, ParticlePool* pool)
{
  for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
  {
    Particle* p = (*pool).particles[particle_index];

    ParticleHistory* next = malloc(sizeof(ParticleHistory));
    (*next).line_for_gnuplot = get_line_for_gnuplot(p);
    (*next).next = NULL;

    (*pp_history).particle_histories[particle_index] = next;
    (*pp_history).last_particle_histories[particle_index] = next;
  }
}

void init_output_directory(pms_config config)
{
    printf("Writing to SimulationDirectory: %s\n", config.SimulationDirectory);

    struct stat st = {0};

    if( stat( config.OutputDirectory, &st) == -1) 
    {
        mkdir(config.OutputDirectory, 0755);
    }

    if( stat( config.SimulationDirectory, &st) == -1) 
    {
        mkdir( config.SimulationDirectory, 0755);
    }
}
