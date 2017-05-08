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

//import java.lang.*;
import java.text.*;
import java.io.*;
import java.util.*;
import ru.formula.*;

public class SimulatorThread extends Thread
{
		public static final int StatusRunning = 1;
		public static final int StatusAbortedWithError = -1;
		public static final int StatusFinished = 2;

		Properties props;
                String outdir;
		SimulatorThreadEventListener stel;
                
                public double displacementx, displacementy, displacementz;
                public double pathlengthx, pathlengthy, pathlengthz;
                public long realnumberofparticles;

		public SimulatorThread(Properties props, SimulatorThreadEventListener stel)
		{
                    this.props = props;
                    this.stel = stel;
		}

		public void run()
		{
                    stel.SimulatorThreadEvent("Starting simulation", 1, SimulatorThread.StatusRunning);
                    try
                    {
                        initOutputDirectory();
                    }
                    catch(IOException e)
                    {
                        stel.SimulatorThreadEvent("Could not save to file ..", 0, SimulatorThread.StatusAbortedWithError);
                        return;
                    }
                    stel.SimulatorThreadEvent("Creating Particle Pool", 2, SimulatorThread.StatusRunning);
                    
                    ParticlePool pp;
                    try
                    {
                        Position corner1 = new Position(
                                Double.parseDouble(props.getProperty("ppCorner1X")),
                                Double.parseDouble(props.getProperty("ppCorner1Y")),
                                Double.parseDouble(props.getProperty("ppCorner1Z"))
                            );
                        Position corner2 = new Position(
                                Double.parseDouble(props.getProperty("ppCorner2X")),
                                Double.parseDouble(props.getProperty("ppCorner2Y")),
                                Double.parseDouble(props.getProperty("ppCorner2Z"))
                            );
                        pp = ParticlePool.createParticlePool(corner1, corner2,
                            props.getProperty("ppDestroyLeavingParticles").equalsIgnoreCase("true") ? true : false);
                    }
                    catch( NumberFormatException nfe)
                    {
                        stel.SimulatorThreadEvent("ERROR: Wrong number format in particle pool corners", 0, SimulatorThread.StatusAbortedWithError);
                        return;
                    }
                    
                    stel.SimulatorThreadEvent("Creating random particles", 3, SimulatorThread.StatusRunning);
                    try
                    {    
                        Mass m = null;
                        ElectricCharge q = null;
                        if( Integer.parseInt(props.getProperty("ppAmountOfParticlesToCreate")) > 0)
                        {
                            int particleType = Integer.parseInt(props.getProperty("ppParticleType"));
                            if( particleType == 0)
                            {   m = Mass.MassOfElectron;
                                q = ElectricCharge.ElectricChargeOfElectron;
                            }
                            if( particleType == 1)
                            {
                                // positrons
                                m = Mass.MassOfPositron;
                                q = ElectricCharge.ElectricChargeOfPositron;
                            }
                            if( particleType == 2)
                            {
                                // neutrons
                                m = Mass.MassOfNeutron;
                                q = ElectricCharge.ElectricChargeOfNeutron;
                            }
                            if( particleType == 3)
                            {
                                // proton
                                m = Mass.MassOfProton;
                                q = ElectricCharge.ElectricChargeOfProton;
                            }
                            
                            if( m != null)
                            {
                                if( props.getProperty("ppDestroy").equalsIgnoreCase("true"))
                                {
                                    pp.addRandomParticles(m,q,
                                        Integer.parseInt(props.getProperty("ppAmountOfParticlesToCreate")), 
                                        Double.parseDouble(props.getProperty("ppSpeedMin")),
                                        Double.parseDouble(props.getProperty("ppSpeedMax")),
                                        Double.parseDouble(props.getProperty("ppDestroyAfter")),
                                        Long.parseLong(props.getProperty("sStepNumberMultiples"))
                                        );
                                }
                                else
                                {
                                    pp.addRandomParticles(m,q,
                                        Integer.parseInt(props.getProperty("ppAmountOfParticlesToCreate")), 
                                        Double.parseDouble(props.getProperty("ppSpeedMin")),
                                        Double.parseDouble(props.getProperty("ppSpeedMax")),
                                        0,
                                        Long.parseLong(props.getProperty("sStepNumberMultiples"))
                                        );
                                }
                            }
                        }
                    }
                    catch( NumberFormatException nfe)
                    {
                        stel.SimulatorThreadEvent("ERROR: Wrong number format in random particle definition", 0, SimulatorThread.StatusAbortedWithError);
                        return;
                    }
                    
                    stel.SimulatorThreadEvent("Creating single particles", 3, SimulatorThread.StatusRunning);
                    String pref = "single_particle_"; 
                    for( int i = 0; i <= 20; i++)
                    {
                        try
                        {
                            Position p = new Position(
                                    Double.parseDouble(props.getProperty(pref + i + "_position_x")),
                                    Double.parseDouble(props.getProperty(pref + i + "_position_y")),
                                    Double.parseDouble(props.getProperty(pref + i + "_position_z"))
                            );
                            Velocity v = new Velocity(
                                    Double.parseDouble(props.getProperty(pref + i + "_velocity_x")),
                                    Double.parseDouble(props.getProperty(pref + i + "_velocity_y")),
                                    Double.parseDouble(props.getProperty(pref + i + "_velocity_z"))
                            );
                            Mass m = new Mass(Double.parseDouble(props.getProperty(pref + i + "_mass")));
                            ElectricCharge q = new ElectricCharge(Double.parseDouble(props.getProperty(pref + i + "_charge")));
                            Particle particle = Particle.createNewParticle(m, q, p, v);
                            pp.addParticle(particle);
                        }
                        catch(NumberFormatException e)
                        {
                            continue;
                        }
                        catch(NullPointerException e)
                        {
                            continue;
                        }
                    }
                    
                    TimeStepGenerator tsg = null;
                    stel.SimulatorThreadEvent("Creating Time Step Generator", 4, SimulatorThread.StatusRunning);
                    try
                    {
                        tsg = new TimeStepGenerator(
                                Double.parseDouble(props.getProperty("tStepSize")),
                                Double.parseDouble(props.getProperty("tStartTime")),
                                Double.parseDouble(props.getProperty("tEndTime")));
                    }
                    catch( NumberFormatException nfe)
                    {
                        stel.SimulatorThreadEvent("ERROR: Wrong number format in Timeframe definition", 0, SimulatorThread.StatusAbortedWithError);
                        return;
                    }
                    
                    
                    // Now compile environment formulas
                    ElectricEnvironment electricField = null;
                    MagneticEnvironment magneticField = null;
                    GravitationalEnvironment gravityField = null;
                    try
                    {
                        stel.SimulatorThreadEvent("Parsing magnetic field formulas", 5, SimulatorThread.StatusRunning);
                        Formula eFormulaBx = null;
                        Formula eFormulaBy = null;
                        Formula eFormulaBz = null;
                    
                        if( props.getProperty("eFormulaBx").length() > 0)
                        {
                            eFormulaBx = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaBx"));
                        }
                        if( props.getProperty("eFormulaBy").length() > 0)
                        {
                            eFormulaBy = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaBy"));
                        }
                        if( props.getProperty("eFormulaBz").length() > 0)
                        {
                            eFormulaBz = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaBz"));
                        }
                        
                        if( eFormulaBx != null && eFormulaBy != null && eFormulaBz != null)
                        {
                            magneticField = new MagneticEnvironment(tsg, eFormulaBx, eFormulaBy, eFormulaBz);
                        }
                        
                        stel.SimulatorThreadEvent("Parsing electric field formulas", 6, SimulatorThread.StatusRunning);
                        Formula eFormulaEx = null;
                        Formula eFormulaEy = null;
                        Formula eFormulaEz = null;
                    
			if( props.getProperty("eFormulaEx").length() > 0)
                        {
                            eFormulaEx = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaEx"));
                        }
                        if( props.getProperty("eFormulaEy").length() > 0)
                        {
                            eFormulaEy = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaEy"));
                        }
                        if( props.getProperty("eFormulaEz").length() > 0)
                        {
                            eFormulaEz = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaEz"));
                        }
                        if( eFormulaEx != null && eFormulaEy != null && eFormulaEz != null)
                        {
                            electricField = new ElectricEnvironment(tsg, eFormulaEx, eFormulaEy, eFormulaEz);
                        }

                        stel.SimulatorThreadEvent("Parsing gravity field formulas", 7, SimulatorThread.StatusRunning);
                        Formula eFormulaGx = null;
                        Formula eFormulaGy = null;
                        Formula eFormulaGz = null;
			if( props.getProperty("eFormulaGx").length() > 0)
                        {
                            eFormulaGx = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaGx"));
                        }
                        if( props.getProperty("eFormulaGy").length() > 0)
                        {
                            eFormulaGy = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaGy"));
                        }
                        if( props.getProperty("eFormulaGz").length() > 0)
                        {
                            eFormulaGz = Formula.compile("f(x,y,z,t) = " + props.getProperty("eFormulaGz"));
                        }
                        if( eFormulaGx != null && eFormulaGy != null && eFormulaGz != null)
                        {
                            gravityField = new GravitationalEnvironment(tsg, eFormulaGx, eFormulaGy, eFormulaGz);
                        }
                    }
                    catch(Exception e)
                    {
                        stel.SimulatorThreadEvent("Error parsing environment formulas", 0, SimulatorThread.StatusAbortedWithError);
                        return;
                    }


                    // Now compile force formulas
                    ElectricForceCalculator electricForceCalculator = null;
                    MagneticForceCalculator magneticForceCalculator = null;
                    GravitationalForceCalculator gravityForceCalculator = null;
                    InteractionForceCalculator interactionForceCalculator = null;
                    try
                    {
                        stel.SimulatorThreadEvent("Parsing electric force formulas", 8, SimulatorThread.StatusRunning);
                        Formula fFormulaFex = null;
                        Formula fFormulaFey = null;
                        Formula fFormulaFez = null;
                    
                        if( props.getProperty("fFormulaFex").length() > 0)
                        {
                            fFormulaFex = Formula.compile("f(q,x,y,z) = " + props.getProperty("fFormulaFex"));
                        }
                        if( props.getProperty("fFormulaFey").length() > 0)
                        {
                            fFormulaFey = Formula.compile("f(q,x,y,z) = " + props.getProperty("fFormulaFey"));
                        }
                        if( props.getProperty("fFormulaFez").length() > 0)
                        {
                            fFormulaFez = Formula.compile("f(q,x,y,z) = " + props.getProperty("fFormulaFez"));
                        }
                        if( fFormulaFex != null && fFormulaFey != null && fFormulaFez != null)
                        {
                            electricForceCalculator = new ElectricForceCalculator(electricField, fFormulaFex, fFormulaFey, fFormulaFez);
                        }
                        
                        stel.SimulatorThreadEvent("Parsing magnetic force formulas", 9, SimulatorThread.StatusRunning);
                        Formula fFormulaFmx = null;
                        Formula fFormulaFmy = null;
                        Formula fFormulaFmz = null;
                    
                        if( props.getProperty("fFormulaFmx").length() > 0)
                        {
                            fFormulaFmx = Formula.compile("f(a,b,c,s,d,e,f,x,y,z,q) = " + props.getProperty("fFormulaFmx"));
                        }
                        if( props.getProperty("fFormulaFmy").length() > 0)
                        {
                            fFormulaFmy = Formula.compile("f(a,b,c,s,d,e,f,x,y,z,q) = " + props.getProperty("fFormulaFmy"));
                        }
                        if( props.getProperty("fFormulaFmz").length() > 0)
                        {
                            fFormulaFmz = Formula.compile("f(a,b,c,s,d,e,f,x,y,z,q) = " + props.getProperty("fFormulaFmz"));
                        }
                        if( fFormulaFmx != null && fFormulaFmy != null && fFormulaFmz != null)
                        {
                            magneticForceCalculator = new MagneticForceCalculator(magneticField, fFormulaFmx, fFormulaFmy, fFormulaFmz);
                        }
                        
                        stel.SimulatorThreadEvent("Parsing gravity force formulas", 10, SimulatorThread.StatusRunning);
                        Formula fFormulaFgx = null;
                        Formula fFormulaFgy = null;
                        Formula fFormulaFgz = null;
                    
                        if( props.getProperty("fFormulaFgx").length() > 0)
                        {
                            fFormulaFgx = Formula.compile("f(m,s,x,y,z) = " + props.getProperty("fFormulaFgx"));
                        }
                        if( props.getProperty("fFormulaFgy").length() > 0)
                        {
                            fFormulaFgy = Formula.compile("f(m,s,x,y,z) = " + props.getProperty("fFormulaFgy"));
                        }
                        if( props.getProperty("fFormulaFgz").length() > 0)
                        {
                            fFormulaFgz = Formula.compile("f(m,s,x,y,z) = " + props.getProperty("fFormulaFgz"));
                        }
                        if( fFormulaFgx != null && fFormulaFgy != null && fFormulaFgz != null)
                        {
                            gravityForceCalculator = new GravitationalForceCalculator(gravityField, fFormulaFgx, fFormulaFgy, fFormulaFgz);
                        }
                        
                        stel.SimulatorThreadEvent("Parsing interaction force formulas", 10, SimulatorThread.StatusRunning);
                        Formula interactingforceFormulaFex = null;
                        Formula interactingforceFormulaFey = null;
                        Formula interactingforceFormulaFez = null;
                    
                        if( props.getProperty("interactingforceFormulaFex").length() > 0)
                        {
                            interactingforceFormulaFex = Formula.compile("f(x,y,z,q,a,b,c,s,d,e,f,m,u,v,w,p,g,h,i,t,j,k,l,n) = " + props.getProperty("interactingforceFormulaFex"));
                        }
                        if( props.getProperty("interactingforceFormulaFey").length() > 0)
                        {
                            interactingforceFormulaFey = Formula.compile("f(x,y,z,q,a,b,c,s,d,e,f,m,u,v,w,p,g,h,i,t,j,k,l,n) = " + props.getProperty("interactingforceFormulaFey"));
                        }
                        if( props.getProperty("interactingforceFormulaFez").length() > 0)
                        {
                            interactingforceFormulaFez = Formula.compile("f(x,y,z,q,a,b,c,s,d,e,f,m,u,v,w,p,g,h,i,t,j,k,l,n) = " + props.getProperty("interactingforceFormulaFez"));
                        }
                        if( interactingforceFormulaFex != null && interactingforceFormulaFey != null && interactingforceFormulaFez != null)
                        {
                            interactionForceCalculator = new InteractionForceCalculator(pp, interactingforceFormulaFex, interactingforceFormulaFey, interactingforceFormulaFez);
                        }
                    }
                    catch(Exception e)
                    {
                        stel.SimulatorThreadEvent("Error parsing force formulas", 0, SimulatorThread.StatusAbortedWithError);
                        return;
                    }

                    // Now compile step2step formulas
                    Step2StepChangeCalculator s2s = null;
                    try
                    {
                        stel.SimulatorThreadEvent("Parsing step 2 step change formulas", 11, SimulatorThread.StatusRunning);
                        Formula tFormulaPx = null;
                        Formula tFormulaPy = null;
                        Formula tFormulaPz = null;
                    
                        if( props.getProperty("tFormulaPx").length() > 0)
                        {
                            tFormulaPx = Formula.compile("f(a,d,e,h,k,l,m,x,t) = " + props.getProperty("tFormulaPx"));
                        }
                        if( props.getProperty("tFormulaPy").length() > 0)
                        {
                            tFormulaPy = Formula.compile("f(b,d,f,i,k,n,m,y,t) = " + props.getProperty("tFormulaPy"));
                        }
                        if( props.getProperty("tFormulaPz").length() > 0)
                        {
                            tFormulaPz = Formula.compile("f(c,d,g,j,k,o,m,z,t) = " + props.getProperty("tFormulaPz"));
                        }
                        
                        Formula tFormulaVx = null;
                        Formula tFormulaVy = null;
                        Formula tFormulaVz = null;
                    
                        if( props.getProperty("tFormulaVx").length() > 0)
                        {
                            tFormulaVx = Formula.compile("f(a,b,c,d,e,f,g,h,i,s,m,t) = " + props.getProperty("tFormulaVx"));
                        }
                        if( props.getProperty("tFormulaVy").length() > 0)
                        {
                            tFormulaVy = Formula.compile("f(a,b,c,d,e,f,g,h,i,s,m,t) = " + props.getProperty("tFormulaVy"));
                        }
                        if( props.getProperty("tFormulaVz").length() > 0)
                        {
                            tFormulaVz = Formula.compile("f(a,b,c,d,e,f,g,h,i,s,m,t) = " + props.getProperty("tFormulaVz"));
                        }
                        
                        s2s = new Step2StepChangeCalculator(tFormulaVx, tFormulaVy, tFormulaVz, tFormulaPx, tFormulaPy, tFormulaPz,tsg);
                        
                        if( magneticForceCalculator != null)
                        {
                            s2s.addForceCalculator(magneticForceCalculator);
                        }
                        if( electricForceCalculator != null)
                        {
                            s2s.addForceCalculator(electricForceCalculator);
                        }
                        if( gravityForceCalculator != null)
                        {
                            s2s.addForceCalculator(gravityForceCalculator);
                        }
                        if( interactionForceCalculator != null)
                        {
                            s2s.addForceCalculator(interactionForceCalculator);
                        }
                    }
                    catch(Exception e)
                    {
                        stel.SimulatorThreadEvent("Error parsing step 2 step change formulas", 0, SimulatorThread.StatusAbortedWithError);
                        return;
                    }    
                        
                    //------------- basic initialisation finished, now run simulation
                    stel.SimulatorThreadEvent("running simulation", 12, SimulatorThread.StatusRunning);
                    while( tsg.hasNext())
                    {
                        stel.SimulatorThreadEvent("running simulation", tsg.getPercentTime(), SimulatorThread.StatusRunning);
                        
                        Iterator<Particle> it = pp.getActiveParticleIterator();
                        while( it.hasNext())
                        {
                            Particle particle = it.next();
                            s2s.doNextStepForParticle(particle);
                            
                        }
                        pp.updateParticleSelfDestruction();
                        tsg.setNextStep();
                        pp.advanceAllParticlesToFutureState();
                    }
                    //------------- simulation finished, now run history analyzer and write output
                   stel.SimulatorThreadEvent("Writing output data files and calculating summary", 0, SimulatorThread.StatusRunning);
                   Iterator<Particle> it = pp.getAllParticleIterator();
                   long i = 0;
                   String gnuplot = "splot ";
                   
                   displacementx = 0; // adds first x - last x positions of all particles
                   displacementy = 0;
                   displacementz = 0;
                   pathlengthx = 0;
                   pathlengthy = 0;
                   pathlengthz = 0;
                   realnumberofparticles = pp.countAllParticles();
                   
                   boolean sWriteDataFiles = false;
                   if(props.getProperty("sWriteDataFiles").equalsIgnoreCase("true"))
                   {
                       sWriteDataFiles = true;
                   }
                   
                   while( it.hasNext())
                   {   
                       i++;
                       stel.SimulatorThreadEvent("Writing output data files and calculating summary", (int)((double)i/(double)pp.countAllParticles() * 100), SimulatorThread.StatusRunning);
                       try
                       {
                           Particle particle = it.next();
                           
                           //--- displacement
                           if( particle.getHistoryPosition().size() > 2)
                           {
                               Position first = particle.getHistoryPosition().get(0);
                               Position last = particle.getHistoryPosition().get(particle.getHistoryPosition().size()-1);
                               displacementx += last.getXValue() - first.getXValue();
                               displacementy += last.getYValue() - first.getYValue();
                               displacementz += last.getZValue() - first.getZValue();
                           }
                           //--- end displacement
                           
                           FileWriter fstream = null;
                           BufferedWriter out = null;
                           if(sWriteDataFiles)
                           {
                                fstream = new FileWriter(outdir + i);
                                out = new BufferedWriter(fstream);
                                
                           }
                           
                           Iterator<Position> it2 = particle.getHistoryPosition().iterator();
                           Iterator<Velocity> itv2 = particle.getHistoryVelocity().iterator();
                           Iterator<Force> itf2 = particle.getHistoryForce().iterator();

                           Position lastp = null;
                           while( it2.hasNext())
                           {    
                               Position p = it2.next();
                               Velocity v = itv2.next();
                               Force f = itf2.next();
                               
                               if(sWriteDataFiles)
                               {
                                    out.write(p.getXValue() + " " + p.getYValue() + " " + p.getZValue() + " " + v.getSpeedValue() + " " + v.getVelocityVector().getXValue() + " " + v.getVelocityVector().getYValue() + " " + v.getVelocityVector().getZValue() + " " + f.getNewton() + " " + f.getFx() + " " + f.getFy() + " " + f.getFz() + "\n");
                               }
                               
                               if( lastp != null)
                               {
                                   pathlengthx += Math.abs(p.getXValue() - lastp.getXValue());
                                   pathlengthy += Math.abs(p.getYValue() - lastp.getYValue());
                                   pathlengthz += Math.abs(p.getZValue() - lastp.getZValue());
                               }
                               lastp = p;
                           }
                           if( sWriteDataFiles)
                           {
                                out.close();
                           }
                        }
                       catch (Exception e)
                       {
                          stel.SimulatorThreadEvent("Error writing output files" + e, 0, SimulatorThread.StatusAbortedWithError);
                          return;
                       }
                       
                       if(sWriteDataFiles)
                       {
                           gnuplot += "\"" + i + "\" with lines";
                           if( it.hasNext())
                           {
                               gnuplot += ", ";
                           }
                       }
                   }
                   
                   if(sWriteDataFiles)
                   {
                       try
                       {
                            FileWriter fstream = new FileWriter(outdir + "gnuplot.script");
                            BufferedWriter gp = new BufferedWriter(fstream);
                            gp.write(gnuplot);
                            gp.close();
                       }
                       catch (Exception e)
                       {
                          stel.SimulatorThreadEvent("Error writing output files", 0, SimulatorThread.StatusAbortedWithError);
                          return;
                       }

                       try 
                       {
                            String command = "gnuplot gnuplot.script -";
                            Runtime.getRuntime().exec(command, null, new File(outdir));
                       } catch (IOException e) {}
                   }
                   
                   stel.SimulatorThreadEvent("Finished Simulation", 100, SimulatorThread.StatusFinished);
		}
                
                private void initOutputDirectory() throws IOException
                {
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd-HHmmss");
                    String datetimestring = sdf.format(GregorianCalendar.getInstance().getTime());
                    outdir = props.getProperty("sOutputDirectory") + "/" + datetimestring + "/";

                    new File(outdir).mkdirs();

                     // now save all options into new directory
                     
                     OutputStream os = new FileOutputStream(outdir + datetimestring + "-simulation.pms");
                     props.store(os, "Particle Movement Simulator - Jan Kechel");
                     os.close();    
                }
}

