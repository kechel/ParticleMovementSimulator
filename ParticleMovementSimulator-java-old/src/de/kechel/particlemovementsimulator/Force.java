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

public class Force
{
	private double newton;
	private Vector direction;

	public Force(double newton, Vector direction)
	{
			this.newton = newton;
			this.direction = direction;
	}
        public Force(double Fx, double Fy, double Fz)
	{
		Vector v = new Vector(Fx, Fy, Fz);
		newton = v.getLengthValue();
                direction = v.getUnitVector();
	}

	public void add(Force force)
	{
		Force f = new Force(getFx() + force.getFx(),
                                    getFy() + force.getFy(),
                                    getFz() + force.getFz());
                this.direction = f.getDirection();
                this.newton = f.getNewton();
	}

	public static Force createZeroForce()
	{
			return new Force(0, new Vector(0,0,0));
	}
        public double getFx()
        {
            return direction.getXValue() * newton;
        }
        public double getFz()
        {
            return direction.getZValue() * newton;
        }
        public double getFy()
        {
            return direction.getYValue() * newton;
        }
        public Vector getDirection()
        {
            return direction;
        }
        public double getNewton()
        {
            return newton;
        }
}

