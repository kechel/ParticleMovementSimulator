/*
 *
 * Jan O. Kechel 
 *
 * Copyright (c) 2008 - jan@kechel.de
 *
 * 25-JUL-2008
 *
 */
package de.kechel.particlemovementsimulator;

public class Velocity
{

	private double speed;
	private Vector direction;

	public Velocity(double speed, Vector direction)
	{
		this.speed = Math.abs(speed);
		this.direction = direction.getUnitVector();
	}
        public Velocity(double vx, double vy, double vz)
	{
                Vector v = new Vector(vx, vy, vz);
		this.speed = v.getLengthValue();
		this.direction = v.getUnitVector();
	}

	public double getSpeedValue()
	{
		return speed;
	}

	public Vector getDirectionValue()
	{
		return direction;
	}

	public Vector getVelocityVector()
	{
		return direction.times(speed);
	}
}
