// Cypress Semiconductor

// Demonstrates use of MBR3 Kit with Adrunio board
// Configures the MBR3 device with 128 bytes of Configuration data generated using Ez-Click over I2C/TWI.
// And reads data from an I2C/TWI slave device-CY8CMBR3116 and displays the status of the Capsense Buttons on the serial terminal window

// Created 05 Feb 2014

// This example code is in the public domain.


#include <Wire.h>

/* CY8CMBR3116 Register Map Offset Address */
#define REGMAP_ORIGIN			0x00
#define SENSOR_PATTERN			0x00
#define FSS_EN					0x02
#define TOGGLE_EN				0x04
#define LED_ON_EN				0x06
#define SENSITIVITY0			0x08
#define SENSITIVITY1			0x09
#define SENSITIVITY2			0x0A
#define SENSITIVITY3			0x0B
#define BASE_THRESHOLD0			0x0C
#define BASE_THRESHOLD1			0x0D
#define FINGER_THRESHOLD2		0x0E
#define FINGER_THRESHOLD3		0x0F
#define FINGER_THRESHOLD4		0x10
#define FINGER_THRESHOLD5		0x11
#define FINGER_THRESHOLD6		0x12
#define FINGER_THRESHOLD7		0x13
#define FINGER_THRESHOLD8		0x14
#define FINGER_THRESHOLD9		0x15
#define FINGER_THRESHOLD10		0x16
#define FINGER_THRESHOLD11		0x17
#define FINGER_THRESHOLD12		0x18
#define FINGER_THRESHOLD13		0x19
#define FINGER_THRESHOLD14		0x1A
#define FINGER_THRESHOLD15		0x1B
#define SENSOR_DEBOUNCE			0x1C
#define BUTTON_HYS				0x1D
#define BUTTON_BUT				0x1E
#define BUTTON_LBR				0x1F
#define BUTTON_NNT				0x20
#define BUTTON_NT				0x21
#define PROX_EN					0x26
#define PROX_CFG				0x27
#define PROX_CFG2				0x28
#define PROX_TOUCH_TH0			0x2A
#define PROX_TOUCH_TH1			0x2C
#define PROX_HYS				0x30
#define PROX_BUT				0x31
#define PROX_LBR				0x32
#define PROX_NNT				0x33
#define PROX_NT					0x34
#define PROX_POSITIVE_TH0		0x35
#define PROX_POSITIVE_TH1		0x36
#define PROX_NEGATIVE_TH0		0x39
#define PROX_NEGATIVE_TH1		0x3A
#define LED_ON_TIME				0x3D
#define BUZZER_CFG				0x3E
#define BUZZER_ON_TIME			0x3F
#define GPO_CFG					0x40
#define PWM_DUTYCYCLE_CFG0		0x41
#define PWM_DUTYCYCLE_CFG1		0x42
#define PWM_DUTYCYCLE_CFG2		0x43
#define PWM_DUTYCYCLE_CFG3		0x44
#define PWM_DUTYCYCLE_CFG4		0x45
#define PWM_DUTYCYCLE_CFG5		0x46
#define PWM_DUTYCYCLE_CFG6		0x47
#define PWM_DUTYCYCLE_CFG7		0x48
#define SPO_CFG					0x4C
#define DEVICE_CFG0				0x4D
#define DEVICE_CFG1				0x4E
#define DEVICE_CFG2				0x4F
#define I2C_ADDR				0x51
#define REFRESH_CTRL			0x52
#define STATE_TIMEOUT			0x55
#define SLIDER_CFG				0x5D
#define SLIDER1_CFG				0x61
#define SLIDER1_RESOLUTION		0x62
#define SLIDER1_THRESHOLD		0x63
#define SLIDER2_CFG				0x67
#define SLIDER2_RESOLUTION		0x68
#define SLIDER2_THRESHOLD		0x69
#define SLIDER_DEBOUNCE			0x6F
#define SLIDER_BUT				0x70
#define SLIDER_LBR				0x71
#define SLIDER_NNT				0x72
#define SLIDER_NT				0x73
#define CONFIG_CRC				0x7E
#define GPO_OUTPUT_STATE		0x80
#define SENSOR_ID				0x82
#define CTRL_CMD				0x86
#define BUTTON_STATUS			0xAA
#define PROX_STAT                       0xAE

#define DEBUG_RAW_COUNT0     0xE2
#define DEBUG_SENSOR_ID      0xDC

/* Command Codes */
#define CMD_NULL				0x00
#define SAVE_CHECK_CRC          0x02
#define CALC_CRC                0x03
#define LOAD_FACTORY            0x04
#define LOAD_PRIMARY            0x05
#define LOAD_SECONDARY          0x06
#define SLEEP                   0x07
#define CLEAR_LATCHED_STATUS    0x08
#define CMD_RESET_PROX0_FILTER	0x09
#define CMD_RESET_PROX1_FILTER	0x0A
#define ENTER_CONFIG_MODE       0x0B
#define EXIT_CONTROL_RUN        0xFE
#define SW_RESET                0xFF

/* Total number of configuration registers */
#define TOTAL_CONFIG_REG_COUNT	0x80

/* Length of Register Map */
#define REG_MAP_LEN	256

/* Slave Address (Default) */
#define SLAVE_ADDR	0x37
	
#define NO_OF_KIT_BUTTONS	4

#define BUZ_ON_1_33kHz                  0x85
#define BUZ_OFF_1_33kHz                 0x05
#define SPO_ENABLE              0x03
#define SPO_DISABLE             0x00

   
const int ledpinnumber = 13;      // the number of the LED pin

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
   // initialize the digital pin as an output.
  pinMode(ledpinnumber, OUTPUT);
  pinMode(2, INPUT);     // Pin 2 is input to which a switch is connected = INT0
   
  
  blinkledfast();
  blinkledfast();

  Serial.begin(115200);  // start serial for output
  configureMBR3();

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(SENSOR_ID);
  Wire.write(2);
  Wire.endTransmission();

  int dbg_sensor_id;
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(DEBUG_SENSOR_ID);
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR, 1);
  dbg_sensor_id = Wire.read();
  Wire.endTransmission();

  Serial.print("DEBUG_SENSOR_ID: ");
  Serial.print(dbg_sensor_id);
  Serial.print("\n");
  
  Serial.print(" Touch any of the Buttons (BTN1,BTN2,BTN3 BTN4) \n");


}


void loop()
{
  int raw_count;
  int sample_count;
  while (true) {
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(DEBUG_RAW_COUNT0);
    Wire.endTransmission();
  
    Wire.requestFrom(SLAVE_ADDR, 2);
    raw_count = Wire.read();
    raw_count = raw_count | (int(Wire.read()) << 8);
    Wire.endTransmission();
  //
  //  Serial.print("raw_count: ");
    sample_count++;
    Serial.print(sample_count);
    Serial.print(",");
    Serial.print(raw_count);
    Serial.print("\n");
    delay(82.5);
  }
}

void configureMBR3()
{
  //Paste/Initialize the configuration array copied from the IIC file of Ez-Click here under the configData array  
  unsigned char configData[128] = {
  //Buzzer and Host Int Enabled: Jumper J15 in Configuration A on CY3280-MBR3 Kit
    0x7Cu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x7Fu, 0x7Fu, 0x80u, 0x80u,
    0x80u, 0x80u, 0x80u, 0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu,
    0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x03u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x80u,
    0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x81u, 0x01u,
    0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0x00u, 0x00u, 0x00u, 0x43u, 0x03u, 0x01u, 0x58u,
    0x00u, 0x37u, 0x06u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x70u, 0xA5u
  };
  
  //2 Dummy Writes to Wake up the MBR3 device
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(REGMAP_ORIGIN);          // sends Offset byte 
  Wire.write(00);
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(REGMAP_ORIGIN);          // sends Offset byte 
  Wire.write(00);
  Wire.endTransmission();    // stop transmitting
  
  //Arduino function can send only 32 bytes at a time hence the ConfigData is sent to MBR3 in chunks
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(REGMAP_ORIGIN);          // sends Offset byte 
  Wire.write(&configData[0],31);        // sends 31 bytes
  Wire.endTransmission();    // stop transmitting
  
  
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(31);          // sends Offset byte 
  Wire.write(&configData[31],31);        // sends next 31 bytes
  Wire.endTransmission();    // stop transmitting
  
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(62);          // sends Offset byte 
  Wire.write(&configData[62],31);        // sends further 31 bytes
  Wire.endTransmission();    // stop transmitting
  
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(93);          // sends Offset byte 
  Wire.write(&configData[93],31);        // sends 31 bytes
  Wire.endTransmission();    // stop transmitting
  
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(124);          // sends Offset byte 
  Wire.write(&configData[124],4);        // sends remaining 4 bytes
  Wire.endTransmission();    // stop transmitting
  
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(CTRL_CMD);
  Wire.write(SAVE_CHECK_CRC);
  Wire.endTransmission();    // stop transmitting
  
  delay(200);
  
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #0x37
  Wire.write(CTRL_CMD);
  Wire.write(SW_RESET);
  Wire.endTransmission();    // stop transmitting
  
  //Provide a delay to calculate and Save CRC 
  delay(200);
}

void blinkledfast()
{
  digitalWrite(ledpinnumber, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);               // wait for a second
  digitalWrite(ledpinnumber, LOW);    // turn the LED off by making the voltage LOW
  delay(200);               // wait for a second
}