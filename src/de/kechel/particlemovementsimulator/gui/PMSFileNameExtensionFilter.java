/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package de.kechel.particlemovementsimulator.gui;

import java.io.*;
/**
 *
 * @author jan
 */
public class PMSFileNameExtensionFilter extends javax.swing.filechooser.FileFilter 
{

    public boolean accept(File f)
    {
        if( f.getName().endsWith(".pms") || f.isDirectory())
        {
            return true;
        }
        return false;
    }
    
    public String getDescription()
    {
        return "Particle Movement Simulator (.pms)";
    }
    
}
