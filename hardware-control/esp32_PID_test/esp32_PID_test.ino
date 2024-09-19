#include <L298NX2.h>

// Motor pin definition
const unsigned int EN_A = 14;
const unsigned int IN1_A = 12;
const unsigned int IN2_A = 13;

const unsigned int IN1_B = 18;
const unsigned int IN2_B = 19;
const unsigned int EN_B = 21;

L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);

// Define PWM pins for ESP32
#define PWM_A_CHANNEL 0  // PWM channel for motor A
#define PWM_B_CHANNEL 1  // PWM channel for motor B
#define PWM_FREQ 5000    // PWM frequency in Hz
#define PWM_RES 8        // PWM resolution (0-255)
#define ENCA 2  // YELLOW
#define ENCB 4  // WHITE
#define ENCC 3  // YELLOW
#define ENCD 5  // WHITE
#define PWM 13
#define IN2 12
#define IN1 11

//#define ENCODER_OPTIMIZE_INTERRUPTS
#include <ESP32Encoder.h>

#define Enc1A 32 // CLK ENCODER 
#define Enc1B 33 // DT ENCODER 
#define Enc2A 34 // CLK ENCODER 
#define Enc2B 35 // DT ENCODER 
 
ESP32Encoder myEnc1;
ESP32Encoder myEnc2;

volatile long posi1 = 0;  // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
volatile long posi2 = 0;  // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long pos1 = 0;
long pos2 = 0;
double PIDpos1 = 0;
double PIDpos2 = 0;
long prevT = 0;
float eprev = 0;
float eintegral = 0;
long pos_prev = 0;
int rot1 = 0;
int rot2 = 0;
int res = 4096;
long target = 0;
long prevTarget = 0;
long rpm = 0;
long maxrpm = 0;
long prevrpm = 0;
float u = 0;
double PIDu = 0;

// #include <AutoPID.h>
// PID constants
// optimised k constants
// double kp = 14;
// double kd = 6;
// double ki = 0;
double kp = 1;
double kd = 0.05;
double ki = 0;
double OUTPUT_MIN = 65;
double OUTPUT_MAX = 210;
// PID will only change the speed if error is > 5
// int PID_range = 20;

// AutoPID myPID(&PIDpos1, &PIDpos2, &PIDu, OUTPUT_MIN, OUTPUT_MAX, kp, ki, kd);

void setup() {
  Serial.begin(115200);

  ledcAttachChannel(EN_A, PWM_FREQ, PWM_RES, PWM_A_CHANNEL);
  ledcAttachChannel(EN_B, PWM_FREQ, PWM_RES, PWM_B_CHANNEL);

  // Motor control pins setup
  pinMode(IN1_A, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  pinMode(IN1_B, OUTPUT);
  pinMode(IN2_B, OUTPUT);

  myEnc1.attachHalfQuad(Enc1A, Enc1B);
  myEnc2.attachHalfQuad(Enc2A, Enc2B);
  myEnc1.setCount(0);
  myEnc2.setCount(0);
}

void loop() {
  // time difference
  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / (1.0e6);
  prevT = currT;

  pos_prev = pos1;

  long posi1 = myEnc1.getCount() / 2;
  // long posi2 = myEnc2.getCount() / 2;

  pos1 = posi1;
  // pos2 = posi2;

  // target = pos1;

  // error
  double e = pos1 - target;

  // derivative
  float dedt = (e - eprev) / (deltaT);

  // integral
  eintegral = eintegral + e * deltaT;
  // if (target != prevTarget){
  //   eintegral = 0;
  // }

  // control signal
  float u = kp * e + kd * dedt + ki * eintegral;

  // motor power
  float pwr = fabs(u);
  if (pwr > OUTPUT_MAX) {
    pwr = OUTPUT_MAX;
  }
  if (pwr < 0) {
    pwr = 0;
  }

  pwr = pwr / OUTPUT_MAX;
  pwr = OUTPUT_MIN + pwr * (OUTPUT_MAX - OUTPUT_MIN);

  // motor direction
  // int dir = 1;
  motors.backwardA();
  if (u < 0) {
    // dir = -1;
    motors.forwardA();
  }

  // signal the motor
  // setMotor(dir,pwr,PWM,IN1,IN2);
  motors.setSpeedA(pwr);

  // store previous error
  eprev = e;
  prevTarget = target;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos1);
  Serial.print(" ");
  Serial.print(e);
  Serial.print(" ");
  Serial.print(pwr);
  Serial.println();

  if (Serial.available() > 0) {
    // Read the input from the serial monitor
    String inputString = Serial.readStringUntil('\n');  // Read until newline character
    inputString.trim(); // Remove any leading/trailing whitespace

    // Convert the string to a float
    target = inputString.toFloat();

    Serial.flush();
  }
}