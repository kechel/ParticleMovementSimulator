/*
 *
 * Jan O. Kechel 
 *
 * Copyright (c) 2008 - jan@kechel.de
 *
 * 25-JUL-2008, 19-AUG-2008, 28-AUG-2008
 *
 */
package de.kechel.particlemovementsimulator;

import java.util.*;

public class Particle
{
	Position p_1;
        Position p_2;
	Velocity v_1;
        Velocity v_2;
        
        // if true: p_1/v_1 ist current, p_2/v_2 is future
        // if false: p_2/v_2 is current, p_1/v_1 is future
        boolean current_state_is_1 = true; 
        
	Mass m;
	ElectricCharge q;
        
        double max_way_until_selfdestruct;

	ParticleEventListener pel;

	java.util.Vector<Position> history_position = new java.util.Vector<Position>();
        java.util.Vector<Velocity> history_velocity = new java.util.Vector<Velocity>();
        java.util.Vector<Force> history_force = new java.util.Vector<Force>();
	Long step = new Long(0);
        long save_step_multiples_of;
        
        ParticleTemplate pt;

	private Particle(Position p, Velocity v, Mass m, ElectricCharge q)
	{
			this.p_1 = p;
			this.v_1 = v;
			this.m = m;
			this.q = q;
                        this.max_way_until_selfdestruct = 0;
                        this.save_step_multiples_of = 1;
	}

	private Particle(Position p, Velocity v, Mass m, ElectricCharge q, double max_way_until_selfdestruct, ParticleEventListener pel, long save_step_multiples_of, ParticleTemplate pt)
	{
			this(p,v,m,q);
                        this.max_way_until_selfdestruct = max_way_until_selfdestruct;
			this.pel = pel;
                        this.save_step_multiples_of = save_step_multiples_of;
                        this.pt = pt;
	}

	public void advanceToFutureState()
        {
            if( this.current_state_is_1)
            {
                this.v_1 = null;
                this.p_1 = null;
            }
            else
            {
                this.v_2 = null;
                this.p_2 = null;
            }
            this.current_state_is_1 = !this.current_state_is_1;
        }
        
        public void destroyParticle()
	{
                history_position.add(getPosition());
                history_velocity.add(getVelocity());
                history_force.add(new Force(0,0,0));
                
                if( pel != null)
                        pel.particleGotDestroyed(this);
	}

	public static Particle createNewParticle(Mass m, ElectricCharge q, Position p, Velocity v)
	{
			return new Particle(p, v, m, q);
	}

	public static Particle createNewParticle(Mass m, ElectricCharge q, Position p, Velocity v, double max_way_until_self_destruct, ParticleEventListener pel, long save_step_multiples_of, ParticleTemplate pt)
	{
			return new Particle(p, v, m, q, max_way_until_self_destruct, pel, save_step_multiples_of, pt);
	}
        public static Particle createNewParticle(ParticleTemplate pt)
        {
            Velocity v = pt.pp.getRandomVelocity(pt.speed_min, pt.speed_max);
            return new Particle(pt.pp.getRandomPosition(), v, pt.m, pt.q, pt.destroy_after, pt.pp, pt.save_step_multiples_of, pt);
        }

	public Position getPosition()
	{
            if( this.current_state_is_1)
            {
                return p_1;
            }
            else
            {
                return p_2;
            }
	}
	public Velocity getVelocity()
	{
            if( this.current_state_is_1)
            {
			return v_1;
            }
            else
            {
                return v_2;
            }
	}
	public Mass getMass()
	{
			return m;
	}
	public ElectricCharge getElectricCharge()
	{
			return q;
	}
	public void setNextStep(Position p, Velocity v, Force f) // velocity & force only for history
	{               step++;
                        if( this.save_step_multiples_of == 0 )
                        {
                            history_position.add(getPosition());
                            history_velocity.add(getVelocity());
                            history_force.add(f);
                        }
                        else
                        {
                            if( step % this.save_step_multiples_of == 0 || step == 1)
                            {
                                history_position.add(getPosition());
                                history_velocity.add(getVelocity());
                                history_force.add(f);
                            }
                        }
                        if( this.current_state_is_1)
                        {
                            this.p_2 = p;
                            this.v_2 = v;
                        }
                        else
                        {
                            this.p_1 = p;
                            this.v_1 = v;
                        }
                        
                        if( max_way_until_selfdestruct > 0)
                        {
                            if( getPathLength() > max_way_until_selfdestruct)
                            {
                                this.destroyParticle();
                            }
                        }
	}
        
        
        public double getPathLength()
        {
            Iterator<Position> it = history_position.iterator();
            double pathlength = 0;
            Position last;
            if( !it.hasNext())
            {
                return 0;
            }
            
            last = it.next();
            while( it.hasNext())
            {
                Position cur = it.next();
                // a^2 + b^2 + c^2 = d^2
                pathlength += Math.sqrt((last.getXValue() - cur.getXValue())*(last.getXValue() - cur.getXValue()) 
                                        + (last.getYValue() - cur.getYValue())*(last.getYValue() - cur.getYValue())
                                        + (last.getZValue() - cur.getZValue())*(last.getZValue() - cur.getZValue()));
                last = cur;
            }
            
            return pathlength;
        }
        
	public java.util.Vector<Position> getHistoryPosition()
	{
			return history_position;
	}
        public java.util.Vector<Velocity> getHistoryVelocity()
	{
			return history_velocity;
	}
        public java.util.Vector<Force> getHistoryForce()
	{
			return history_force;
	}
}

