# ParticleMovementSimulator
A very flexible Simulator where you can enter your own formulas to experiment.

## Features
### Particles & Space
- Pool Space dimensions
- Particles can be created by random within configured parameters
- Particles can be specified individually
### Environment
- supports electric, magnetic and gravitational fields
### Environmental forces
- supports forces due to the electric, magnetic and gravitational fields
### Particle interaction forces
- supports direct forces due to other particles (like e.g. electrostatic forces)
### Time 
- supports timeframes of any length/shorthness, step-sizes for simulation and start-/end-time
- supports free definition on how to calculate the particles next velocity based on the calculated forces
- supports free definition on how to calculate the particles next position based on the calculated forces and velocities
### Simulation
- supports logging to a file
- supports gnuplot do display the moving path of the particles graphically
- runs as thread in background, so while simulating you can tweak the formulas for your next simulation

## Requirements
- Java
- gnuplot

## Binary Installation
1. copy dist/ParticleMovementSimulator.jar and the subdirectory dist/lib/ with all files in some place on your disc
2. make sure lib is a direct subdirectory of your ParticleMovementSimulator.jar
3. double-click or run java -jar ParticleMovementSimulator.jar

## Compile from Source
- ant make
- or open project with Netbeans (or your favorite IDE)

