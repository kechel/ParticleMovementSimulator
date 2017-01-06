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

public abstract class ForceCalculator
{

	Environment env;

	public ForceCalculator(Environment env)
	{
		this.env = env;
	}
        
        public ForceCalculator()
	{
		this.env = null;
	}

	public abstract Force calculateForce(Particle particle);

}

