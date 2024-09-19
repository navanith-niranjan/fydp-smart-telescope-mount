// Global variables
double ra;  // Right Ascension (input)
double dec; // Declination (input)
long localX, localY; // Local coordinates (output, motor rotations)
double raToMotorRatio;  // Conversion ratio for RA to motor Y-axis rotations
double decToMotorRatio; // Conversion ratio for DEC to motor X-axis rotations
double rotationAngle;    // Misalignment correction angle (in degrees or radians)
double raZeroPoint = 0;      // Custom zero point for RA (in degrees, 0 to 360)
double decZeroPoint = 0;     // Custom zero point for DEC (in degrees, -90 to 90)

// Add code to limit view angles (90 degrees in any direction from setpoint) after finding the zero/location on Earth

// Setting ratios
int encoderPPR = 1024;
float outputGearRatio = 78 * 60;
float degreesPerPulse = 360 / encoderPPR;
float outputDegreesPerPulse = degreesPerPulse / outputGearRatio;
// RA given in hours/minutes/seconds (from hours to pulses); DEC given in degrees (from degrees to pulses)
// RA: 360 degrees = 24 hours; 15 degrees = 1 hour
// Input coordinates will be multiplied by the ratios to get encoder pulses
double raToMotorRatio = 1 * (15 / 1) / outputDegreesPerPulse;
double decToMotorRatio = 1 / outputDegreesPerPulse;

// Subtract the coordinates of the telescope "zero" position to align the coordinate systems
// Then apply rotation matrix
// With rotation, moving up or down only in RA will require both motors to rotate

// Order of operations:
// 1) Subtract offset (zero position of telescope)
// 2) Multiply by the unit conversion ratios to get relative encoder positions
// 3) Apply rotation matrix (rotate the RA/DEC coordinate system to align with encoder coordinates)

// Function to convert RA/DEC to local coordinates (motor rotations) with custom zero point
void convertRaDecToLocal(float ra, float dec) {
  // Step 1: Apply the zero point offset to RA/DEC coordinates
  double adjustedRa = ra - raZeroPoint;
  double adjustedDec = dec - decZeroPoint;
  
  // Ensure RA is wrapped around properly (since RA is circular from 0 to 360 degrees)
  if (adjustedRa < 0) {
    adjustedRa += 360;
  } else if (adjustedRa >= 360) {
    adjustedRa -= 360;
  }

  if (adjustedDec < -90) {
    adjustedDec += 180;
  } else if (adjustedDec >= 90) {
    adjustedDec += 180;
  }


  // Step 2: Convert adjusted RA and DEC to intermediate motor coordinates
  float motorX = adjustedRa * raToMotorRatio;
  float motorY = adjustedDec * decToMotorRatio;
  
  // Step 3: If there is a misalignment, apply the rotation angle
  if (rotationAngle != 0) {
    // Use rotation matrix to apply the correction
    float correctedX = motorX * cos(rotationAngle) - motorY * sin(rotationAngle);
    float correctedY = motorX * sin(rotationAngle) + motorY * cos(rotationAngle);
    
    // Update local coordinates with corrected values
    localX = correctedX;
    localY = correctedY;
  } else {
    // If no rotation correction is needed, simply assign the intermediate values
    localX = motorX;
    localY = motorY;
  }
}

// Setup function (runs once)
void setup() {
  Serial.begin(115200);

  // Set the initial conversion ratios and rotation angle
  // raToMotorRatio = 1.0;  // Example ratio, adjust accordingly
  // decToMotorRatio = 1.0; // Example ratio, adjust accordingly
  rotationAngle = 0;      // No misalignment by default (can set to a non-zero value)
  
  // Set the custom zero points for RA/DEC (these can be set dynamically)
  raZeroPoint = 0;  // Example RA zero point (set 0 degrees as origin)
  decZeroPoint = 0.0;   // Example DEC zero point (set 0 degrees as origin)
  
  // Set initial/target RA/DEC values (for testing purposes)
  ra = 210.0;  // Example RA in degrees
  dec = 15.0;  // Example DEC in degrees
  
  // Perform the conversion
  convertRaDecToLocal(ra, dec);

  // Output the result for debugging
  Serial.print("Local X (encoder rotations): ");
  Serial.println(localX);
  Serial.print("Local Y (encoder rotations): ");
  Serial.println(localY);
}

// Loop function (runs repeatedly)
void loop() {
  // Additional logic can be placed here to handle real-time updates,
  // read new RA/DEC values, and perform transformations dynamically.
}
