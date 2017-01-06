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

public class GravitationalEnvironment extends Environment
{
	Formula Gx, Gy, Gz;
	public GravitationalEnvironment(TimeStepGenerator tsg, Formula Gx, Formula Gy, Formula Gz)
	{
			super(tsg);
			this.Gx = Gx;
			this.Gy = Gy;
			this.Gz = Gz;
	}

        public Force getFieldStrengthAtPosition(Position p)
        {
            return new Force(Gx.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()),
                    Gy.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()),
                    Gz.evaluate(p.getXValue(), p.getYValue(), p.getZValue(), tsg.getCurrentTime()));
        }
}

