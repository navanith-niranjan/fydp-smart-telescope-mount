#!/usr/bin/python3
# -*- coding: utf-8 -*-

'''
install.py

This program is intended to install all
prerequisite packages for the star tracker
software on a Raspberry Pi running Raspbian

'''

################################
#LOAD LIBRARIES
################################
import os
import sys
import time

################################
#MAIN CODE
################################
#ensure user has set locale and time
usr_in = input('Before running this script, you should set the system locale and time.  Have you done this? [Y/N]: ')
usr_in=str(usr_in)
usr_in=usr_in.lower()
if usr_in != 'y' and usr_in != 'yes' and usr_in != 'heck yeah':
    print('\n\nset the system locale and time, restart the system, and then try again')
    sys.exit()

# install/update system stuff
os.system('sudo apt-get --yes update')
os.system('sudo apt-get --yes upgrade')
#os.system('sudo rpi-update')  #maybe people shouldn't do this unless things are really broke anyway
os.system('sudo apt-get --yes install gfortran')
os.system('sudo apt-get --yes install libatlas-base-dev')
os.system('sudo apt-get --yes install libopenblas-dev')

# Install/update python stuff
os.system('sudo apt-get --yes install python3-lxml')
os.system('sudo apt-get --yes install libopencv-dev python3-opencv')

os.system('sudo pip3 install pip --upgrade --break-system-packages')
os.system('sudo pip3 install psutil --break-system-packages')
os.system('sudo pip3 install imageio --break-system-packages') #required for catalog creation
os.system('sudo pip3 install astropy --break-system-packages') #required for catalog creation
os.system('sudo pip3 install pandas --break-system-packages') #required for catalog creation
os.system('sudo pip3 install statistics --break-system-packages')
os.system('sudo pip3 install astroquery --break-system-packages') #required for astrometry verification

# Install module
home = os.getcwd()
os.chdir('..')
os.chdir('..')
os.chdir('..')
os.chdir('py_src/star_tracker')
os.system('sudo pip3 install . --break-system-packages')
os.chdir(home)

# Performance analysis
os.system('sudo pip3 --no-cache-dir install matplotlib --break-system-packages') # otherwise you get a memory error
os.system('sudo pip3 install --upgrade setuptools --break-system-packages')
os.system('sudo pip3 --no-cache-dir install scipy --break-system-packages') # same


print("\n\nInstallation complete.  Please restart the Pi!") 
print("NOTE: no camera interfaces were installed during this process.  Other scripts/software may have to be run to install camera software interfaces and drivers.")

