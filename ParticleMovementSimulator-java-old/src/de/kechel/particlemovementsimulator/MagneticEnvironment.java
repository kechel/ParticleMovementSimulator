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

public class MagneticEnvironment extends Environment
{
	Formula Bx, By, Bz;
	public MagneticEnvironment(TimeStepGenerator tsg, Formula Bx, Formula By, Formula Bz)
	{
			super(tsg);
			this.Bx = Bx;
			this.By = By;
			this.Bz = Bz;
	}
        
        public Force getFieldStrengthAtPosition(Position p)
        {
            return new Force(
                    Bx.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()),
                    By.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()),
                    Bz.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()));
        }
}

