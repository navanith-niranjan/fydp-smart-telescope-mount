#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
pi_cam_interface.py

This script is intended to provide a 
simple interface to a Raspberry Pi camera with live preview.

"""

################################
# LOAD LIBRARIES
################################
import os
from picamera2 import Picamera2, Preview
from time import sleep

################################
# USER INPUT
################################
# Select camera type
pi_cam = "v2"  # Options: "v1", "v2", "hq"
image_extension = ".jpg"

# Set exposure time and ISO-like value (AnalogueGain)
exposure_time = 15000  # Exposure time in microseconds
analogue_gain = 4.0  # Higher value for higher sensitivity (acts like ISO)

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
elif pi_cam == "v2":
    selected_cam_res = v2_res
elif pi_cam == "hq":
    selected_cam_res = v2_res  # Adjust HQ resolution if needed

# Configure the camera settings for preview
config = picam2.create_preview_configuration(main={"size": (1280, 720)})  # Reduced resolution for faster preview
picam2.configure(config)

# Start the camera preview in a separate window
picam2.start_preview(Preview.QTGL)  # Uses QTGL window for preview

# Start the camera
picam2.start()

# Apply manual exposure and gain settings
picam2.set_controls({"ExposureTime": exposure_time, "AnalogueGain": analogue_gain})

usr_in = ''
capture_number = 0

while usr_in == '':
    usr_in = input('Press Enter to capture, or type anything to exit.\n')
    
    # Change to a still configuration for high-res image capture
    still_config = picam2.create_still_configuration(main={"size": (selected_cam_res[0], selected_cam_res[1])})
    picam2.configure(still_config)
    
    capture_name = 'pi_test_' + str(capture_number) + image_extension
    
    # Rename until name is not taken
    while os.path.exists(os.path.join(os.getcwd(), capture_name)):
        capture_number += 1
        capture_name = 'pi_test_' + str(capture_number) + '.jpg'
                
    if usr_in == '':
        # Capture the image file
        picam2.capture_file(os.path.join(os.getcwd(), capture_name))
        print(capture_name + ' saved!\n')
        picam2.configure(config)  # Reconfigure to preview mode
        picam2.start()  # Restart preview

# Stop the camera and preview
picam2.stop_preview()
picam2.stop()
