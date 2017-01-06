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

public class InteractionForceCalculator extends ForceCalculator
{
	Formula Fix, Fiy, Fiz;
        ParticlePool pp;
	
        public InteractionForceCalculator(ParticlePool pp, Formula Fix, Formula Fiy, Formula Fiz)
	{
		super();
		this.Fix = Fix;
		this.Fiy = Fiy;
		this.Fiz = Fiz;
                this.pp = pp;
	}
        
	public Force calculateForce(Particle p0)
	{
            double dFix = 0;
            double dFiy = 0;
            double dFiz = 0;
        
            for( Particle pn : this.pp.active_particles)
            {
                if( pn == p0)
                {
                    continue; // no interaction with itself
                }
            
                dFix += Fix.evaluate(
                        p0.getPosition().getXValue(),
                        p0.getPosition().getYValue(),
                        p0.getPosition().getZValue(),
                        p0.getElectricCharge().getElectricChargeValue(),
                        p0.getVelocity().getVelocityVector().getXValue(),
                        p0.getVelocity().getVelocityVector().getYValue(),
                        p0.getVelocity().getVelocityVector().getZValue(),
                        p0.getVelocity().getSpeedValue(),
                        p0.getVelocity().getDirectionValue().getXValue(),
                        p0.getVelocity().getDirectionValue().getYValue(),
                        p0.getVelocity().getDirectionValue().getZValue(),
                        p0.getMass().getMassValue(),
                        
                        pn.getPosition().getXValue(),
                        pn.getPosition().getYValue(),
                        pn.getPosition().getZValue(),
                        pn.getElectricCharge().getElectricChargeValue(),
                        pn.getVelocity().getVelocityVector().getXValue(),
                        pn.getVelocity().getVelocityVector().getYValue(),
                        pn.getVelocity().getVelocityVector().getZValue(),
                        pn.getVelocity().getSpeedValue(),
                        pn.getVelocity().getDirectionValue().getXValue(),
                        pn.getVelocity().getDirectionValue().getYValue(),
                        pn.getVelocity().getDirectionValue().getZValue(),
                        pn.getMass().getMassValue()
                    );
                dFiy += Fiy.evaluate(
                                                p0.getPosition().getXValue(),
                        p0.getPosition().getYValue(),
                        p0.getPosition().getZValue(),
                        p0.getElectricCharge().getElectricChargeValue(),
                        p0.getVelocity().getVelocityVector().getXValue(),
                        p0.getVelocity().getVelocityVector().getYValue(),
                        p0.getVelocity().getVelocityVector().getZValue(),
                        p0.getVelocity().getSpeedValue(),
                        p0.getVelocity().getDirectionValue().getXValue(),
                        p0.getVelocity().getDirectionValue().getYValue(),
                        p0.getVelocity().getDirectionValue().getZValue(),
                        p0.getMass().getMassValue(),
                        
                        pn.getPosition().getXValue(),
                        pn.getPosition().getYValue(),
                        pn.getPosition().getZValue(),
                        pn.getElectricCharge().getElectricChargeValue(),
                        pn.getVelocity().getVelocityVector().getXValue(),
                        pn.getVelocity().getVelocityVector().getYValue(),
                        pn.getVelocity().getVelocityVector().getZValue(),
                        pn.getVelocity().getSpeedValue(),
                        pn.getVelocity().getDirectionValue().getXValue(),
                        pn.getVelocity().getDirectionValue().getYValue(),
                        pn.getVelocity().getDirectionValue().getZValue(),
                        pn.getMass().getMassValue()                        
                    );
                dFiz += Fiz.evaluate(
                        p0.getPosition().getXValue(),
                        p0.getPosition().getYValue(),
                        p0.getPosition().getZValue(),
                        p0.getElectricCharge().getElectricChargeValue(),
                        p0.getVelocity().getVelocityVector().getXValue(),
                        p0.getVelocity().getVelocityVector().getYValue(),
                        p0.getVelocity().getVelocityVector().getZValue(),
                        p0.getVelocity().getSpeedValue(),
                        p0.getVelocity().getDirectionValue().getXValue(),
                        p0.getVelocity().getDirectionValue().getYValue(),
                        p0.getVelocity().getDirectionValue().getZValue(),
                        p0.getMass().getMassValue(),
                        
                        pn.getPosition().getXValue(),
                        pn.getPosition().getYValue(),
                        pn.getPosition().getZValue(),
                        pn.getElectricCharge().getElectricChargeValue(),
                        pn.getVelocity().getVelocityVector().getXValue(),
                        pn.getVelocity().getVelocityVector().getYValue(),
                        pn.getVelocity().getVelocityVector().getZValue(),
                        pn.getVelocity().getSpeedValue(),
                        pn.getVelocity().getDirectionValue().getXValue(),
                        pn.getVelocity().getDirectionValue().getYValue(),
                        pn.getVelocity().getDirectionValue().getZValue(),
                        pn.getMass().getMassValue()
                    );
            }
            return new Force(dFix, dFiy, dFiz);
	}
}

