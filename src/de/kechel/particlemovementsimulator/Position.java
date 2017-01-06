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

public class Position 
{
	Vector p;

	public Position(double x, double y, double z)
	{
		p = new Vector(x,y,z);
	}
	public Position(Vector p)
	{
		this.p = p;
	}

	public double getXValue()
	{
			return p.getXValue();
	}
	public double getYValue()
	{
			return p.getYValue();
	}

	public double getZValue()
	{
			return p.getZValue();
	}


	public boolean isBetween(Position corner1, Position corner2)
	{
		if( getXValue() > corner1.getXValue() && getXValue() > corner2.getXValue())
			return false;
		if( getYValue() > corner1.getYValue() && getYValue() > corner2.getYValue())
			return false;
		if( getZValue() > corner1.getZValue() && getZValue() > corner2.getZValue())
			return false;
		if( getXValue() < corner1.getXValue() && getXValue() < corner2.getXValue())
			return false;
		if( getYValue() < corner1.getYValue() && getYValue() < corner2.getYValue())
			return false;
		if( getZValue() < corner1.getZValue() && getZValue() < corner2.getZValue())
			return false;

		return true;
	}
}
