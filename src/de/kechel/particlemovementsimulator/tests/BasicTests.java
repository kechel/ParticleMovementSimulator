/*
 * 	EnviroMed OHG, Stockach, Germany - info@enviromed.de
 * 	Jan O. Kechel - jan@kechel.de
 * 	Copyright (c) 2008 - All rights reserved
 *
 */

package de.kechel.particlemovementsimulator.tests;

import junit.framework.TestCase;
import de.kechel.particlemovementsimulator.*;
import java.util.*;

public class BasicTests extends TestCase 
{
		public BasicTests(String arg0) 
		{
				super(arg0);
		}

		protected void setUp() throws Exception 
		{
				super.setUp();
		}

		public void testTimeStepGenerator()
		{
				TimeStepGenerator tsg = new TimeStepGenerator(0.001, 0, 0.005);
				assertEquals(0.0, tsg.getCurrentTime());
				assertTrue(tsg.setNextStep());
				assertEquals(0.001, tsg.getCurrentTime());
				assertTrue(tsg.setNextStep());
				assertTrue(tsg.setNextStep());
				assertTrue(tsg.setNextStep());
				assertEquals(0.004, tsg.getCurrentTime());
				assertTrue(tsg.setNextStep());
				assertEquals(0.005, tsg.getCurrentTime());
				assertFalse(tsg.setNextStep());
				assertEquals(0.006, tsg.getCurrentTime());

		}
		public void testPosition()
		{
				Position p1 = new Position(0,0,0);
				assertTrue(p1.isBetween(p1,p1));

				Position p2 = new Position(1,1,1);
				assertTrue(p1.isBetween(p1,p2));

				Position p3 = new Position(2,3,4);
				assertFalse(p3.isBetween(p1,p2));

				Position p4 = new Position(0.1,0.5,0.8);
				assertTrue(p4.isBetween(p1,p2));
		}

		public void testParticlePool()
		{
				ParticlePool pp = ParticlePool.createParticlePool( new Position(0,0,0), new Position(1,1,1), false);
				pp.addRandomParticles(Mass.MassOfElectron, ElectricCharge.ElectricChargeOfElectron,100, 0, 10,0,1);
				assertEquals(100, pp.countActiveParticles());
				assertEquals(0,pp.countOldParticles());
				Iterator<Particle> it = pp.getActiveParticleIterator();

				it.next().destroyParticle();
				Particle b = it.next();
				Particle c = it.next();
				pp.updateParticleSelfDestruction();
				assertEquals(100, pp.countActiveParticles());
				assertEquals(1,pp.countOldParticles());

				b.destroyParticle();
				c.destroyParticle();
				pp.updateParticleSelfDestruction();
				assertEquals(100, pp.countActiveParticles());
				assertEquals(3,pp.countOldParticles());

				it = pp.getActiveParticleIterator();
				while( it.hasNext())
				{
						Particle p = it.next();
						assertTrue(p.getPosition().isBetween(new Position(0,0,0), new Position(1,1,1)));
						assertTrue(p.getVelocity().getSpeedValue() >= 0);
						assertTrue(p.getVelocity().getSpeedValue() <= 10);
						assertFalse(p.getVelocity().getSpeedValue() == 0);
				}
		}

		protected void tearDown() throws Exception 
		{
				super.tearDown();
		}
}


