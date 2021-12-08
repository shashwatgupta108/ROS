#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>

ros::NodeHandle nh;

sensor_msgs::Range range_msg;
ros::Publisher pub_range( "ultrasound_range", &range_msg);
char frameid[] = "/ultrasound";

// Declaring constants.
#define echoPin 2
#define trigPin 3
const boolean centimeters = true;

void setup() {
  // initialising serial communication
  nh.initNode();
  nh.advertise(pub_range);

  range_msg.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_msg.header.frame_id = frameid;
  range_msg.field_of_view = 0.1;
  range_msg.min_range = 0.002;
  range_msg.max_range = 0.150;
  
  // Serial.begin(9600)

}

long getRange(int trig, int echo, boolean in_centimeters){
  long duration, distance_cm;

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin,LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  distance_cm = microsecondsToCentimeters(duration);

  return distance_cm;
}

void loop() {
  // put your main code here, to run repeatedly:
  range_msg.range = getRange(trigPin,echoPin,centimeters);
  range_msg.header.stamp = nh.now();
  pub_range.publish(&range_msg);
  nh.spinOnce();
  delay(500);
}

long microsecondsToCentimeters(long duration){
  
  return duration / 29 / 2;
}
