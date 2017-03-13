/*
    Copied from http://pastebin.com/UrYF7rL2 (CC-BY-SA 3.0) and then modified:

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


#include "load_particle_pool_from_csv.h"

void csv_line_finished(int delim __attribute__((unused)), void* p_pool) 
{
  ParticlePool* pool = ((ParticlePool*)p_pool);
  unsigned int particle_index = (*pool).current_particle_index;

  if( particle_index >= MAX_PARTICLES)
  {
    return;
  }

  Particle** particles = (*pool).particles;
  Particle* p = particles[particle_index];

  (*pool).current_field_index = 0;
  (*pool).particles_initialized = (*pool).current_particle_index;
  (*pool).current_particle_index += 1;

  (*p).force_x = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*p).force_x);

  (*p).force_y = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*p).force_y);

  (*p).force_z = (mpf_t*)malloc(sizeof(mpf_t));
  mpf_init(*(*p).force_z);
}

void process_csv_field(void *field, size_t field_len __attribute__((unused)), void *p_pool)
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


char* readfile(const char *filename, size_t *len) 
{
    FILE *fh = fopen(filename, "r");
    if (fh == NULL) 
    {
        return NULL;
    }

    int rc;

    rc = fseek(fh, 0, SEEK_END);

    if (rc < 0) 
    {
        return NULL;
    }

    long l = ftell(fh);

    if (l < 0) 
    {
        return NULL;
    }

    *len = l;

    rc = fseek(fh, 0, SEEK_SET);
    if (rc < 0) 
    {
        return NULL;
    }

    char *contents = malloc(*len);
    if (contents == NULL) 
    {
        return NULL;
    }

    size_t read = 0;

    while (read < *len) 
    {
        size_t r = fread(contents + read, 1, *len - read, fh);
        if (r == 0) 
        {
            if (ferror(fh)) 
            {
                free(contents);
                fclose(fh);
                return NULL;
            } 
            else if (feof(fh)) 
            {
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


int load_particle_pool_from_csv(const char* filename, ParticlePool* pool) 
{
    size_t len;
    char *csvdata = readfile(filename, &len);
    if (csvdata == NULL)
    {
        return -1;
    }

    struct csv_parser parser;
    printf("Parsing Particle Pool %s", filename);
    int rc = csv_init(&parser, CSV_APPEND_NULL);
    csv_set_delim(&parser, ';');

    if (rc != 0) 
    {
        free(csvdata);
        printf("failed to initialize CSV parser\n");
        return -1;
    }

    csv_parse(&parser, (void*)csvdata, len, process_csv_field, csv_line_finished, pool);
    rc = csv_fini(&parser, process_csv_field, csv_line_finished, pool);

    csv_free(&parser);
    free(csvdata);

    printf("\nParsing Particle Pool %s done\n", filename);
    return 0;
}

void print_particle_values(int precision, const Particle* p)
{
    gmp_printf("X  %.*Fe ", precision, *((*p).position_x));
    gmp_printf("Y  %.*Fe ", precision, *((*p).position_y));
    gmp_printf("Z  %.*Fe\n", precision, *((*p).position_z));

    gmp_printf("Vx %.*Fe ", precision, *((*p).velocity_x));
    gmp_printf("Vy %.*Fe ", precision, *((*p).velocity_y));
    gmp_printf("Vz %.*Fe\n", precision, *((*p).velocity_z));

    gmp_printf("Fx %.*Fe ", precision, *((*p).force_x));
    gmp_printf("Fy %.*Fe ", precision, *((*p).force_y));
    gmp_printf("Fz %.*Fe\n", precision, *((*p).force_z));

    gmp_printf("M  %.*Fe ", precision, *((*p).mass));
    gmp_printf("C  %.*Fe\n", precision, *((*p).charge));
}

void print_particle_pool_values(int precision, const ParticlePool* pool)
{
  printf("Dumping Particle Pool:\n");

  for( int particle_index = 0; particle_index <= (*pool).particles_initialized; particle_index++)
  {
    printf("\nParticle %d:\n", particle_index);

    Particle* p = (*pool).particles[particle_index];

    print_particle_values(precision, p);
  }

  printf("\nDumping Particle Pool done\n");
}
