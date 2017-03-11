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

import javax.swing.*;
import de.kechel.particlemovementsimulator.gui.*;

public class ParticleMovementSimulator
{
   public static void main(String[] args)
   {
		JFrame jf = new JFrame();
		jf.setSize(1000,750);
		jf.getContentPane().add(new jpMain());
                jf.setTitle("Particle Movement Simulator");
	  	jf.setVisible(true);
   }
}

