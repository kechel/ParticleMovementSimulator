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

public class ParticleTemplate
{
    double speed_min;
    double speed_max;
    double destroy_after;
    long save_step_multiples_of;
    Mass m;
    ElectricCharge q;
    ParticlePool pp;
	
    public ParticleTemplate(double speed_min, double speed_max, double destroy_after, long save_step_multiples_of, Mass m, ElectricCharge q, ParticlePool pp)
    {
        this.speed_min = speed_min;
        this.speed_max = speed_max;
        this.destroy_after = destroy_after;
        this.save_step_multiples_of = save_step_multiples_of;
        this.m = m;
        this.q = q;
        this.pp = pp;
    }
}

