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

public abstract class Environment
{
	TimeStepGenerator tsg;

	public Environment(TimeStepGenerator tsg)
	{
			this.tsg = tsg;
	}

	public abstract Force getFieldStrengthAtPosition(Position position);

}

