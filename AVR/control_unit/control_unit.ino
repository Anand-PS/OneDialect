#include <SoftwareSerial.h>

bool developer_mode   = 1;    // make it 0 for normal mode
int debouncing_delay  = 30;   // in milli seconds

int motor_pin     = A0;
int buzzer_pin    = 5;
int bt_state_pin  = 4;
int button_pin    = 2;
int led_pin       = 13;

bool button_state     = 0;
bool bluetooth_state  = 0;

int data                = 0;
bool data_send          = 0;
bool bt_connected_flag  = 0;

SoftwareSerial btSerial(12, 11); // RX, TX

void setup() 
{
  Serial.begin(9600);
  btSerial.begin(9600);
  
  pinMode(motor_pin,    OUTPUT);
  pinMode(buzzer_pin,   OUTPUT);
  pinMode(bt_state_pin, INPUT);
  pinMode(button_pin,   INPUT_PULLUP);
  pinMode(led_pin,      OUTPUT);


//  while (!Serial) 
//  {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }


  for(int i=0;i<3;i++)  // Boot successful
  {
    digitalWrite(buzzer_pin,HIGH);
    digitalWrite(led_pin,HIGH);
    delay(100);
    digitalWrite(buzzer_pin,LOW);
    digitalWrite(led_pin,LOW);
    delay(80);
  }

}

void loop() 
{
  read_sensors();

  send_data();

  receive_data();

  if(developer_mode)
  {
    serial_feedback();
  }

  bt_connection_status_feedback();
}


void read_sensors()
{
  bluetooth_state = digitalRead(bt_state_pin);
  button_state    = digitalRead(button_pin);
}


void send_data()
{
  if(button_state == HIGH)
  {
    delay(debouncing_delay);
    data_send = 1;
    
    btSerial.write('a');

    if(developer_mode)
    {
      digitalWrite(buzzer_pin,HIGH);
    }
  
  }
  else
  {
    delay(debouncing_delay);
    
    data_send = 0;
    
    btSerial.write('z');
  
    if(developer_mode)
    {
      digitalWrite(buzzer_pin,LOW);
    }
  
  }
}


void receive_data()
{
  if (btSerial.available()>0)
  {
    data = btSerial.read();

    if(data == '0')
    {
      digitalWrite(buzzer_pin,LOW);
      digitalWrite(motor_pin,LOW);
    }
    if(data == '1')
    {
      digitalWrite(motor_pin,HIGH);
    }
  
    if(data == '2')
    {
     digitalWrite(buzzer_pin,HIGH);
    }
  }
}


void bt_connection_status_feedback()
{
  if(bluetooth_state == 1 && bt_connected_flag == 0)
  {
    for(int i=0;i<2;i++)
    {
      digitalWrite(buzzer_pin,HIGH);
      digitalWrite(motor_pin,HIGH);
      delay(100);
      digitalWrite(buzzer_pin,LOW);
      digitalWrite(motor_pin,LOW);
      delay(80);
    }

    bt_connected_flag = 1;
  }

  if(bluetooth_state == 0 && bt_connected_flag == 1)
  {
      digitalWrite(buzzer_pin,HIGH);
      digitalWrite(motor_pin,HIGH);
      delay(500);
      digitalWrite(buzzer_pin,LOW);
      digitalWrite(motor_pin,LOW);
    

    bt_connected_flag = 0;
  }
}

void serial_feedback()
{
  Serial.print("BT state: ");
  Serial.print(bluetooth_state);
  Serial.print(" | Button: ");
  Serial.print(button_state);

  Serial.print(" | Rx Data: ");
  Serial.print(data);

  Serial.print(" | Tx Data: ");
  Serial.println(data_send);
}
