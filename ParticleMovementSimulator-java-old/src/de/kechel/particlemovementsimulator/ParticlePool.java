/*
 *
 * Jan O. Kechel 
 *
 * Copyright (c) 2008 - jan@kechel.de
 *
 * 25-JUL-2008, 19-AUG-2008, 28-AUG-2008
 *
 */
package de.kechel.particlemovementsimulator;

import java.util.*;

public class ParticlePool implements ParticleEventListener
{
		// Wuerfelfoermiger Raum (fuer Initialisierung), definiert durch zwei gegenueberliegende Ecken
		Position corner1;
		Position corner2;
		boolean destroyLeavingParticles;

		java.util.Vector<Particle> active_particles = new java.util.Vector<Particle>();
		java.util.Vector<Particle> old_particles = new java.util.Vector<Particle>();
                java.util.Vector<Particle> all_particles = new java.util.Vector<Particle>();
                java.util.Vector<Particle> destroy_particles = new java.util.Vector<Particle>();

		private static Random generator = new Random();

		private ParticlePool( Position corner1, Position corner2, boolean destroyLeavingParticles)
		{
				this.corner1 = corner1;
				this.corner2 = corner2;
				this.destroyLeavingParticles = destroyLeavingParticles;
		}

		public static ParticlePool createParticlePool(Position corner1, Position corner2, boolean destroyLeavingParticles)
		{
				return new ParticlePool(corner1, corner2, destroyLeavingParticles);
		}

                public void advanceAllParticlesToFutureState()
                {
                    Iterator<Particle> it = this.active_particles.iterator();
                    while(it.hasNext())
                    {
                        Particle p = it.next();
                        p.advanceToFutureState();
                    }
                }
                
		public void addRandomParticles(Mass m, ElectricCharge q, long howmany, double speed_min, double speed_max, double destroy_after, long save_step_multiples_of)
		{
                    ParticleTemplate pt = new ParticleTemplate(speed_min, speed_max, destroy_after, save_step_multiples_of, m, q, this);
				for( long i = 0; i < howmany; i++)
				{
					addParticle(Particle.createNewParticle(pt));
				}
		}

		public void addParticle(Particle particle)
		{
			active_particles.add(particle);
                        all_particles.add(particle);
		}

		public Position getRandomPosition()
		{
				double x, y, z;
				double between0and1;

				between0and1 = generator.nextDouble();
				x = Math.abs(corner1.getXValue() - corner2.getXValue()) * between0and1 + Math.min(corner1.getXValue(), corner2.getXValue());
				between0and1 = generator.nextDouble();
				y = Math.abs(corner1.getYValue() - corner2.getYValue()) * between0and1 + Math.min(corner1.getYValue(), corner2.getYValue());
				between0and1 = generator.nextDouble();
				z = Math.abs(corner1.getZValue() - corner2.getZValue()) * between0and1 + Math.min(corner1.getZValue(), corner2.getZValue());
				return new Position(x, y, z);
		}
                
                public Velocity getRandomVelocity(double speed_min, double speed_max)
                {
				double x, y, z;

				x = generator.nextDouble() - 0.5;
				y = generator.nextDouble() - 0.5;
				z = generator.nextDouble() - 0.5;

				double speed = speed_min + Math.abs(speed_min - speed_max) * generator.nextDouble();

				return new Velocity(speed, new Vector(x,y,z));
                }

		public int countActiveParticles()
		{
				return active_particles.size();
		}
		public int countOldParticles()
		{
				return old_particles.size();
		}

                public void particleGotDestroyed(Particle particle)
                {
                    this.destroy_particles.add(particle);
                }
                
		public void destroyParticle(Particle particle)
		{
			if( active_particles.contains(particle))
			{
					active_particles.remove(particle);
					old_particles.add(particle);
			}
		}

		public Iterator<Particle> getActiveParticleIterator()
		{
				return active_particles.iterator();
		}
                
                public Iterator<Particle> getAllParticleIterator()
		{
				return all_particles.iterator();
		}
                
                public void updateParticleSelfDestruction()
                {
                    Iterator<Particle> it = this.destroy_particles.iterator();
                    while(it.hasNext())
                    {
                        Particle p = it.next();
                        this.destroyParticle(p);
                        addParticle(Particle.createNewParticle(p.pt));
                    }
                    this.destroy_particles = new java.util.Vector<Particle>();
                }
                public long countAllParticles()
                {
                    return all_particles.size();
                }
}

