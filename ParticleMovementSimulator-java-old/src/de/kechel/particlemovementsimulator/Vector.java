/*
 *
 * Jan O. Kechel 
 *
 * Copyright (c) 2008 - jan@kechel.de
 *
 * 25-JUL-2008, 29-AUG-2008
 *
 */
package de.kechel.particlemovementsimulator;

public class Vector
{

	public static final Vector e1 = new Vector(1,0,0);
	public static final Vector e2 = new Vector(0,1,0);
	public static final Vector e3 = new Vector(0,0,1);

	double x;
	double y;
	double z;

	public Vector(double x, double y, double z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}

	public Vector times(double scalar)
	{
		return new Vector(x * scalar, y * scalar, z * scalar);
	}

	public double getLengthValue()
	{
			return Math.sqrt(x*x + y*y + z*z);
	}

	public Vector getUnitVector()
	{
            if( getLengthValue() != 0)
            {
		return new Vector(x/getLengthValue(), y/getLengthValue(), z/getLengthValue());
            }
            return new Vector(1,0,0);
	}

	public double getXValue()
	{
			return x;
	}
	public double getYValue()
	{
			return y;
	}
	public double getZValue()
	{
			return z;
	}

}

