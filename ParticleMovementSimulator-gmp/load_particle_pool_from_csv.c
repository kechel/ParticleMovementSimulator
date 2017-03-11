/*
    Copied from http://pastebin.com/UrYF7rL2 (CC-BY-SA 3.) and then modified:

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_particle_pool_from_csv.h"
#include "pms_datatypes.h"
#include <csv.h>


char* readfile(char *filename, size_t *len) 
{
    // open file for reading
    FILE *fh = fopen(filename, "r");
    if (fh == NULL) {
        perror("fopen()");
        return NULL;
    }

    int rc;

    // get file length
    rc = fseek(fh, 0, SEEK_END);
    if (rc < 0) {
        perror("fseek(END)");
        return NULL;
    }
    long l = ftell(fh);
    if (l < 0) {
        perror("ftell()");
        return NULL;
    }
    *len = l;

    // return file pointer to the beginning of input
    rc = fseek(fh, 0, SEEK_SET);
    if (rc < 0) {
        perror("fseek(SET)");
        return NULL;
    }

    // read in all file contents
    char *contents = malloc(*len);
    if (contents == NULL) {
        perror("malloc");
        return NULL;
    }
    size_t read = 0;
    while (read < *len) {
        size_t r = fread(contents + read, 1, *len - read, fh);
        if (r == 0) {
            if (ferror(fh)) {
                printf("error reading input\n");
                free(contents);
                fclose(fh);
                return NULL;
            } else if (feof(fh)) {
                printf("EOF encountered after %zu bytes (expected %zu)\n", read, *len);
                *len = read;
                break;
            }
        }
        read += r;
    }

    fclose(fh);
    return contents;
}

void process_field(void *field, size_t field_len __attribute__((unused)), void *p_pool)
{
  ParticlePool* pool = ((ParticlePool*)p_pool);
  unsigned int particle_index = (*pool).current_particle_index;
  unsigned int field_index = (*pool).current_field_index;

  if( particle_index >= MAX_PARTICLES)
  {
    return;
  }

  Particle** particles = (*pool).particles;

  if( field_index == 0)
  {
    printf("\nParticle %d: ", particle_index);
    particles[particle_index] = (Particle*)malloc(sizeof(Particle));
  }

  Particle* p = particles[particle_index];

  mpf_t* value;
  value = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*value);
  mpf_set_str( *value, (char*)field, 10);

  switch(field_index)
  {
    case 0:
      (*p).position_x = value;
      break;
    case 1:
      (*p).position_y = value;
      break;
    case 2:
      (*p).position_z = value;
      break;
    case 3:
      (*p).velocity_x = value;
      break;
    case 4:
      (*p).velocity_y = value;
      break;
    case 5:
      (*p).velocity_z = value;
      break;
    case 6:
      (*p).mass = value;
      break;
    case 7:
      (*p).charge = value;
  }

  gmp_printf("%.*Fe ", 2, *value);

  (*pool).current_field_index += 1;
}

void process_row(int delim __attribute__((unused)), void* p_pool) 
{
  ParticlePool* pool = ((ParticlePool*)p_pool);

  if( (*pool).current_particle_index >= MAX_PARTICLES)
  {
    return;
  }

  (*pool).current_field_index = 0;
  (*pool).particles_initialized = (*pool).current_particle_index;
  (*pool).current_particle_index += 1;
}

int load_particle_pool_from_csv(char* filename, ParticlePool* pool) 
{
    size_t len;
    char *csvdata = readfile(filename, &len);
    if (csvdata == NULL)
    {
        return -1;
    }

    struct csv_parser parser;
    int rc = csv_init(&parser, CSV_APPEND_NULL);
    csv_set_delim(&parser, ';');

    if (rc != 0) 
    {
        free(csvdata);
        printf("failed to initialize CSV parser\n");
        return -1;
    }


    csv_parse(&parser, (void*)csvdata, len, process_field, process_row, pool);
    rc = csv_fini(&parser, process_field, process_row, pool);

    Particle** particles = (*pool).particles;
    Particle* p0 = particles[0];

    csv_free(&parser);
    free(csvdata);

    return 0;
}

