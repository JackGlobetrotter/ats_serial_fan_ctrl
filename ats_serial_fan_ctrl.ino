#define CPU 6
#define FAN_PWM 9
#define FAN 5

const byte OC1A_PIN = 9;
const byte OC1B_PIN = 10;

const word PWM_FREQ_HZ = 20000; //Adjust this value to adjust the frequency
const word TCNT1_TOP = 16000000/(2*PWM_FREQ_HZ);

void setup() {
  
  pinMode(CPU, OUTPUT);  
  pinMode(FAN, OUTPUT);
  pinMode(OC1A_PIN, OUTPUT);

  // Clear Timer1 control and count registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Set Timer1 configuration
  // COM1A(1:0) = 0b10   (Output A clear rising/set falling)
  // COM1B(1:0) = 0b00   (Output B normal operation)
  // WGM(13:10) = 0b1010 (Phase correct PWM)
  // ICNC1      = 0b0    (Input capture noise canceler disabled)
  // ICES1      = 0b0    (Input capture edge select disabled)
  // CS(12:10)  = 0b001  (Input clock select = clock/1)
  
  TCCR1A |= (1 << COM1A1) | (1 << WGM11);
  TCCR1B |= (1 << WGM13) | (1 << CS10);
  ICR1 = TCNT1_TOP;
  digitalWrite(CPU, 1);
  digitalWrite(FAN, 1);
  digitalWrite(FAN_PWM, 1); 
  Serial.begin(115200);
}

void loop() {
    if(Serial.available()>1){
      byte type = Serial.read();
      if(type == 1)
      {
        analogWrite(CPU, Serial.read());
      }else if(type==2){
        byte speed = Serial.read();
        setPwmDuty((speed*100)/255);
        analogWrite(FAN, speed);
      }
    }
    Serial.flush();
    delay(10);

}

void setPwmDuty(byte duty) {
  OCR1A = (word) (duty*TCNT1_TOP)/100;
}
