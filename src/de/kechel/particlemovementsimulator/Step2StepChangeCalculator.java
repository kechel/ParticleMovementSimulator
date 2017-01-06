/*
 *
 * Jan O. Kechel 
 *
 * Copyright (c) 2008 - jan@kechel.de
 *
 * 25-JUL-2008, 19-AUG-2008
 *
 */
package de.kechel.particlemovementsimulator;

import java.util.*;
import ru.formula.*;

public class Step2StepChangeCalculator
{
		java.util.Vector<ForceCalculator> forces;

		Formula Vx, Vy, Vz;
		Formula Px, Py, Pz;
                TimeStepGenerator tsg;

		public Step2StepChangeCalculator(Formula Vx, Formula Vy, Formula Vz, Formula Px, Formula Py,Formula Pz, TimeStepGenerator tsg)
		{
				forces = new java.util.Vector<ForceCalculator>();
				this.Vx = Vx;
				this.Vy = Vy;
				this.Vz = Vz;
				this.Px = Px;
				this.Py = Py;
				this.Pz = Pz;
                                this.tsg = tsg;
		}

		public void addForceCalculator(ForceCalculator forceCalculator)
		{
				forces.add(forceCalculator);
		}

		public Force calculateResultingForceFor(Particle particle)
		{
				Force resultingForce = Force.createZeroForce();

				Iterator<ForceCalculator> it = forces.iterator();
				while( it.hasNext())
				{
					resultingForce.add(it.next().calculateForce(particle));
				}
				return resultingForce;
		}
		
                public Velocity calculateNextVelocity(Particle particle, Force resultingForce)
		{
                    
                    double dNextVx = Vx.evaluate(resultingForce.getFx(), 
                                            resultingForce.getFy(), 
                                            resultingForce.getFz(),
                                            
                                            particle.getVelocity().getVelocityVector().getXValue(),
                                            particle.getVelocity().getVelocityVector().getYValue(),
                                            particle.getVelocity().getVelocityVector().getZValue(),
                                            
                                            particle.getVelocity().getDirectionValue().getXValue(),
                                            particle.getVelocity().getDirectionValue().getYValue(),
                                            particle.getVelocity().getDirectionValue().getZValue(),
                                            
                                            particle.getVelocity().getSpeedValue(),
                                            
                                            particle.getMass().getMassValue(),
                                            
                                            tsg.getStepSize());
                    
                    double dNextVy = Vy.evaluate(resultingForce.getFx(), 
                                            resultingForce.getFy(), 
                                            resultingForce.getFz(),
                                            
                                            particle.getVelocity().getVelocityVector().getXValue(),
                                            particle.getVelocity().getVelocityVector().getYValue(),
                                            particle.getVelocity().getVelocityVector().getZValue(),
                                            
                                            particle.getVelocity().getDirectionValue().getXValue(),
                                            particle.getVelocity().getDirectionValue().getYValue(),
                                            particle.getVelocity().getDirectionValue().getZValue(),
                                            
                                            particle.getVelocity().getSpeedValue(),
                                            
                                            particle.getMass().getMassValue(),
                                            
                                            tsg.getStepSize());
                    
                    double dNextVz = Vz.evaluate(resultingForce.getFx(), 
                                            resultingForce.getFy(), 
                                            resultingForce.getFz(),
                                            
                                            particle.getVelocity().getVelocityVector().getXValue(),
                                            particle.getVelocity().getVelocityVector().getYValue(),
                                            particle.getVelocity().getVelocityVector().getZValue(),
                                            
                                            particle.getVelocity().getDirectionValue().getXValue(),
                                            particle.getVelocity().getDirectionValue().getYValue(),
                                            particle.getVelocity().getDirectionValue().getZValue(),
                                            
                                            particle.getVelocity().getSpeedValue(),
                                            
                                            particle.getMass().getMassValue(),
                                            
                                            tsg.getStepSize());
                    
                    return new Velocity(dNextVx, dNextVy, dNextVz);
		}
                
		public Position calculateNextPosition(Particle particle, Velocity vNext)
		{
                    double dNextPx = Px.evaluate(
                            particle.getVelocity().getVelocityVector().getXValue(),
                            particle.getVelocity().getSpeedValue(),
                            particle.getVelocity().getDirectionValue().getXValue(),
                            vNext.getVelocityVector().getXValue(),
                            vNext.getSpeedValue(),
                            vNext.getDirectionValue().getXValue(),
                            particle.getMass().getMassValue(),
                            particle.getPosition().getXValue(),
                            tsg.getStepSize()
                            );
                    
                    double dNextPy = Py.evaluate(
                            particle.getVelocity().getVelocityVector().getYValue(),
                            particle.getVelocity().getSpeedValue(),
                            particle.getVelocity().getDirectionValue().getYValue(),
                            vNext.getVelocityVector().getYValue(),
                            vNext.getSpeedValue(),
                            vNext.getDirectionValue().getYValue(),
                            particle.getMass().getMassValue(),
                            particle.getPosition().getYValue(),
                            tsg.getStepSize());
                    
                    double dNextPz = Pz.evaluate(
                            particle.getVelocity().getVelocityVector().getZValue(),
                            particle.getVelocity().getSpeedValue(),
                            particle.getVelocity().getDirectionValue().getZValue(),
                            vNext.getVelocityVector().getZValue(),
                            vNext.getSpeedValue(),
                            vNext.getDirectionValue().getZValue(),
                            particle.getMass().getMassValue(),
                            particle.getPosition().getZValue(),
                            tsg.getStepSize());
                    
                    return new Position(dNextPx, dNextPy, dNextPz);
                    
		}
                
                public void doNextStepForParticle(Particle particle)
                {
                    Force force = calculateResultingForceFor(particle);
                    Velocity velocity = calculateNextVelocity(particle, force);
                    Position position = calculateNextPosition(particle, velocity);
                    particle.setNextStep(position, velocity, force);
                }
}

