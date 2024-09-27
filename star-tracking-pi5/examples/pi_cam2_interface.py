#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
pi_cam_interface.py

This script is intended to provide a 
simple interface to a raspberry pi camera

"""

################################
# LOAD LIBRARIES
################################
import os
from picamera2 import Picamera2
from time import sleep

################################
# USER INPUT
################################
# Select camera type
pi_cam = "v2" # v1, v2, hq
image_extension = ".jpg"

################################
# MAIN CODE
################################
# Create camera object
picam2 = Picamera2()

# Camera options
v1_res = [2592, 1944]
v2_res = [3280, 2464]

if pi_cam == "v1":
    selected_cam_res = v1_res
if pi_cam == "v2":
    selected_cam_res = v2_res
if pi_cam == "hq":
    selected_cam_res = v2_res #TODO update to actual HQ res, but somehow fix memory issues

# Configure the camera settings
config = picam2.create_still_configuration(main={"size": (selected_cam_res[0], selected_cam_res[1])})
picam2.configure(config)

# Start the camera
picam2.start()
sleep(2)

usr_in = ''
capture_number = 0

while usr_in == '':
    
    usr_in = input('Press enter to capture, input any character to exit.\n')
    
    capture_name = 'pi_test_'+str(capture_number)+image_extension
    
    #Rename until name is not taken
    while os.path.exists(os.path.join(os.getcwd(), capture_name)):
        capture_number += 1
        capture_name = 'pi_test_'+str(capture_number)+'.jpg'
                
    if usr_in == '':
        picam2.capture_file(os.path.join(os.getcwd(),capture_name))
        print(capture_name+' saved!\n')

# Release camera resources
picam2.stop()
