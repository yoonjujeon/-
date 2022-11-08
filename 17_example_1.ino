#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
#define PIN_SERVO 10// Add IR Sensor Definition Here !!!
#define PIN_IR 0
#define PIN_LED 9

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 400.0   // maximum distance to be measured (unit: mm)


#define LOOP_INTERVAL 25   // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5
Servo myservo;
unsigned long last_loop_time;   // unit: msec
float dist_ema;
void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
  int a_value, duty;
  
  float dist;
  float dist_prev=250.0;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  a_value=analogRead(PIN_IR);// Read IR Sensor value !!!
  dist = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;// Convert IR sensor value into distance !!!
  if (dist < 100.0) {
    dist = dist_prev;           // cut lower than minimum
    digitalWrite(PIN_LED, 1);       // LED OFF
  } else if (100.0<= dist && dist<=250.0) {
    dist = dist_prev;           // Cut higher than maximum
    digitalWrite(PIN_LED, 0); // LED ON
  if (dist_ema <100.0){
      myservo.writeMicroseconds(_DUTY_MIN);
    }
    else if (dist_ema <250.0){
      myservo.writeMicroseconds((1846/180*dist_ema-100)+553);
    }
    else{
      myservo.writeMicroseconds(_DUTY_MAX);
    }
  }   else {    // In desired Range
    dist = dist_prev;
    digitalWrite(PIN_LED, 1);       // LED OFF      
   } 
  }
  
  // we need distance range filter here !!!
  dist_ema = (dist*_EMA_ALPHA) + ((1-_EMA_ALPHA)*dist_ema);// we need EMA filter here !!!

  // map distance into duty
  duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("ADC Read: "); Serial.print(a_value);
  Serial.print(" = ");
  Serial.print((a_value / 1024.0) * 5.0);
  Serial.print(" Volt => Duty : ");
  Serial.print(duty);
  Serial.println("usec");
}
