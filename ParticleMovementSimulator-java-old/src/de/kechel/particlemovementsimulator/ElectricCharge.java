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

public class ElectricCharge
{

	public static final ElectricCharge ElectricChargeOfElectron = new ElectricCharge(1.602176462E10-19);
        public static final ElectricCharge ElectricChargeOfNeutron = new ElectricCharge(0);
        public static final ElectricCharge ElectricChargeOfPositron = new ElectricCharge(1.602176462E10-19);
        public static final ElectricCharge ElectricChargeOfProton = new ElectricCharge(1.602176462E10-19);

	private double C;

	public ElectricCharge(double C)
	{
			this.C = C;
	}

	public double getElectricChargeValue()
	{
			return C;
	}

}

