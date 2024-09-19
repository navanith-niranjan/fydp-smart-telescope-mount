import math

# Function to convert RA/DEC to local coordinates with custom zero point and roll angle
def convert_ra_dec_to_local(ra, dec, ra_zero_point, dec_zero_point, roll_angle, ra_to_motor_x_ratio, dec_to_motor_y_ratio):
    # Step 1: Apply the zero point offset to RA/DEC coordinates
    adjusted_ra = ra - ra_zero_point
    adjusted_dec = dec - dec_zero_point

    # Ensure RA is wrapped around properly (since RA is circular from 0 to 360 degrees)
    if adjusted_ra < 0:
        adjusted_ra += 360
    elif adjusted_ra >= 360:
        adjusted_ra -= 360

    # Step 2: Convert adjusted RA and DEC to intermediate motor coordinates
    motor_x = adjusted_ra * ra_to_motor_x_ratio
    motor_y = adjusted_dec * dec_to_motor_y_ratio

    # Step 3: If there is a roll (misalignment), apply the rotation angle (roll)
    if roll_angle != 0:
        # Convert roll angle from degrees to radians for trigonometric functions
        roll_angle_rad = math.radians(roll_angle)
        corrected_x = motor_x * math.cos(roll_angle_rad) - motor_y * math.sin(roll_angle_rad)
        corrected_y = motor_x * math.sin(roll_angle_rad) + motor_y * math.cos(roll_angle_rad)
    else:
        corrected_x = motor_x
        corrected_y = motor_y

    return corrected_x, corrected_y

# Input the initial zero points and roll angle
ra_zero_point = float(input("Enter the RA zero point (in degrees, 0 to 360): "))
dec_zero_point = float(input("Enter the DEC zero point (in degrees, -90 to 90): "))
roll_angle = float(input("Enter the Roll (misalignment correction angle in degrees): "))

# Define conversion ratios (these can be adjusted as needed)
ra_to_motor_x_ratio = 1.0  # Example ratio for RA to motor X-axis rotations
dec_to_motor_y_ratio = 1.0 # Example ratio for DEC to motor Y-axis rotations

# Loop to input RA/DEC coordinates and return converted coordinates
while True:
    # Input RA/DEC coordinates
    ra_input = float(input("\nEnter the RA coordinate (in degrees, 0 to 360): "))
    dec_input = float(input("Enter the DEC coordinate (in degrees, -90 to 90): "))

    # Perform the conversion
    local_x, local_y = convert_ra_dec_to_local(
        ra_input, dec_input, ra_zero_point, dec_zero_point, roll_angle, ra_to_motor_x_ratio, dec_to_motor_y_ratio
    )

    # Output the result
    print(f"Local X (motor rotations): {local_x:.2f}")
    print(f"Local Y (motor rotations): {local_y:.2f}")
