#
#    Jan Kechel - jan@kechel.de
#    Copyright (c) 2017
#
#    This File is part of Particle Movement Simulator with the The GNU Multiple Precision Arithmetic Library (PMS with GMP)
#
#    PMS with GMP is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    PMS with GMP is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with PMS with GMP.  If not, see <http://www.gnu.org/licenses/>. 
#

CC = gcc -g3
CC_FLAGS = -Wall -std=c11
CC_LIBS = -lgmp -lm -lcsv -lconfig
 
EXEC = pms
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
 
compile: $(OBJECTS)
	$(CC) $(CC_LIBS) $(CC_FLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.c
	$(CC) $(CC_LIBS) $(CC_FLAGS) -c $< -o $@
 
clean:
	rm -f $(EXEC) $(OBJECTS)

