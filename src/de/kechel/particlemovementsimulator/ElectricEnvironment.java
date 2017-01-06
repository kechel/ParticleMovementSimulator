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

public class ElectricEnvironment extends Environment
{
	Formula Ex, Ey, Ez;
	public ElectricEnvironment(TimeStepGenerator tsg, Formula Ex, Formula Ey, Formula Ez)
	{
			super(tsg);
			this.Ex = Ex;
			this.Ey = Ey;
			this.Ez = Ez;
	}
        
        public Force getFieldStrengthAtPosition(Position p)
        {
            return new Force(
                    Ex.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()),
                    Ey.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()),
                    Ez.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()));
        }
}

