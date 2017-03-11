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

public class ElectricForceCalculator extends ForceCalculator
{
	Formula Fex, Fey, Fez;

	public ElectricForceCalculator(ElectricEnvironment env, Formula Fex, Formula Fey, Formula Fez)
	{
		super(env);
		this.Fex = Fex;
		this.Fey = Fey;
		this.Fez = Fez;
	}

	public Force calculateForce(Particle particle)
	{
		Force E = env.getFieldStrengthAtPosition(particle.getPosition());
                
                double dEx = Fex.evaluate(
                            particle.getElectricCharge().getElectricChargeValue(),
                            E.getFx(),
                            E.getFy(),
                            E.getFz());
                double dEy = Fey.evaluate(
                            particle.getElectricCharge().getElectricChargeValue(),
                            E.getFx(),
                            E.getFy(),
                            E.getFz());
                double dEz = Fez.evaluate(
                            particle.getElectricCharge().getElectricChargeValue(),
                            E.getFx(),
                            E.getFy(),
                            E.getFz());
                
                return new Force(dEx, dEy, dEz);
	}
}

