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

import ru.formula.*;

public class MagneticForceCalculator extends ForceCalculator
{
	Formula Fmx, Fmy, Fmz;
	public MagneticForceCalculator(Environment env, Formula Fmx, Formula Fmy, Formula Fmz)
	{
		super(env);
		this.Fmx = Fmx;
		this.Fmy = Fmy;
		this.Fmz = Fmz;
	}

	public Force calculateForce(Particle particle)
	{
            Force B = env.getFieldStrengthAtPosition(particle.getPosition());
            
            double dFx = Fmx.evaluate(
                        particle.getVelocity().getVelocityVector().getXValue(),
                        particle.getVelocity().getVelocityVector().getYValue(),
                        particle.getVelocity().getVelocityVector().getZValue(),
                        particle.getVelocity().getSpeedValue(),
                        particle.getVelocity().getDirectionValue().getXValue(),
                        particle.getVelocity().getDirectionValue().getYValue(),
                        particle.getVelocity().getDirectionValue().getZValue(),
                        B.getFx(),
                        B.getFy(),
                        B.getFz(),
                        particle.getElectricCharge().getElectricChargeValue());
            
            double dFy = Fmy.evaluate(
                        particle.getVelocity().getVelocityVector().getXValue(),
                        particle.getVelocity().getVelocityVector().getYValue(),
                        particle.getVelocity().getVelocityVector().getZValue(),
                        particle.getVelocity().getSpeedValue(),
                        particle.getVelocity().getDirectionValue().getXValue(),
                        particle.getVelocity().getDirectionValue().getYValue(),
                        particle.getVelocity().getDirectionValue().getZValue(),
                        B.getFx(),
                        B.getFy(),
                        B.getFz(),
                        particle.getElectricCharge().getElectricChargeValue());
            
            double dFz = Fmz.evaluate(
                        particle.getVelocity().getVelocityVector().getXValue(),
                        particle.getVelocity().getVelocityVector().getYValue(),
                        particle.getVelocity().getVelocityVector().getZValue(),
                        particle.getVelocity().getSpeedValue(),
                        particle.getVelocity().getDirectionValue().getXValue(),
                        particle.getVelocity().getDirectionValue().getYValue(),
                        particle.getVelocity().getDirectionValue().getZValue(),
                        B.getFx(),
                        B.getFy(),
                        B.getFz(),
                        particle.getElectricCharge().getElectricChargeValue());
            
            return new Force(dFx, dFy, dFz);
	}
}

