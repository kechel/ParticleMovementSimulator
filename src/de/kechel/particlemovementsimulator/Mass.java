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

public class Mass
{

	public static final Mass MassOfElectron = new Mass(9.10938215E10-31);
        public static final Mass MassOfPositron = new Mass(9.1093826E10-31);
        public static final Mass MassOfNeutron = new Mass(1.67492729E10-27);
        public static final Mass MassOfProton = new Mass(1.67262158E10-27);

	private double kg;

	public Mass(double kg)
	{
			this.kg = kg;
	}

	public double getMassValue()
	{
			return kg;
	}

}

