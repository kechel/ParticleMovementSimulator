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

public class GravitationalForceCalculator extends ForceCalculator
{
	Formula Fgx, Fgy, Fgz;
	
        public GravitationalForceCalculator(GravitationalEnvironment env, Formula Fgx, Formula Fgy, Formula Fgz)
	{
		super(env);
		this.Fgx = Fgx;
		this.Fgy = Fgy;
		this.Fgz = Fgz;
	}
        
	public Force calculateForce(Particle particle)
	{
            Force G = env.getFieldStrengthAtPosition(particle.getPosition());
            
            double dFgx = Fgx.evaluate(
                        particle.getMass().getMassValue(),
                        particle.getVelocity().getSpeedValue(),
                        G.getFx(),
                        G.getFy(),
                        G.getFz()                        
                    );
            double dFgy = Fgy.evaluate(
                        particle.getMass().getMassValue(),
                        particle.getVelocity().getSpeedValue(),
                        G.getFx(),
                        G.getFy(),
                        G.getFz()                        
                    );
            double dFgz = Fgz.evaluate(
                        particle.getMass().getMassValue(),
                        particle.getVelocity().getSpeedValue(),
                        G.getFx(),
                        G.getFy(),
                        G.getFz()                        
                    );
            return new Force(dFgx, dFgy, dFgz);
	}
}

