#define MPU_9250_ADDRESS  0x68
#define SMPLRT_DIV  0x19 //陀螺仪采样率典型值为0X07 1000/(1+7)=125HZ
#define CONFIG 0x1A //低通滤波器  典型值0x06 5hz
#define GYRO_CONFIG 0x1B //陀螺仪测量范围 0X18 正负2000度
#define ACCEL_CONFIG 0x1C //加速度计测量范围 0X18 正负16g
#define ACCEL_CONFIG2 0x1D //加速度计低通滤波器 0x06 5hz
#define INT_PIN_CFG 0x37
#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B        //电源管理1 
#define PWR_MGMT_2 0x6C        //电源管理2
#define ACCEL_XOUT_H 0x3B      //加速度计输出数据
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41        //温度计输出数据
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define CNTL1 0x0A   //control 1
#define ASAX 0x10    //MAG calibrate data
#define ST1 0x02  //status 1
#define HXL 0x03  //mag data addr
#define ST2 0x09  //status 1

int avgNumber = 1000;

//from module
float asaX, asaY, asaZ;
int16_t ACCEL_X, ACCEL_Y, ACCEL_Z;
int16_t GYRO_X, GYRO_Y, GYRO_Z;
int16_t MXL, MXH, MYL, MYH ,MZL, MZH;


//real number
double G_FORCE_X,G_FORCE_Y,G_FORCE_Z;
double ROT_X,ROT_Y,ROT_Z;
float MAG_X, MAG_Y, MAG_Z;

//function
void setupMPU( int I2C_ADDRESS )  //初始化MPU9250
{

  Wire.beginTransmission( I2C_ADDRESS );  //向0x68开始一次传输数据，发送一个I2C开始字符
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);//MPU WAKE UP
  Wire.endTransmission();
  Serial.println("PWR_MGMT_1 Fin!");
  
  Wire.beginTransmission( I2C_ADDRESS );  // enable accelerometer and gyro
  Wire.write(PWR_MGMT_2);
  Wire.write(0x00);//MPU WAKE UP
  Wire.endTransmission();
  Serial.println("PWR_MGMT_2 Fin!");

  Wire.beginTransmission( I2C_ADDRESS );
  Wire.write(SMPLRT_DIV);
  Wire.write(0x07);
  Wire.endTransmission();
  Serial.println("SMPLRT_DIV Fin!");

  Wire.beginTransmission( I2C_ADDRESS );
  Wire.write(CONFIG);
  Wire.write(0x06);
  Wire.endTransmission();
  Serial.println("CONFIG Fin!");

  Wire.beginTransmission( I2C_ADDRESS );
  Wire.write(ACCEL_CONFIG);
  Wire.write(0x18);
  Wire.endTransmission();
  Serial.println("ACCEL_CONFIG Fin!");

  Wire.beginTransmission( I2C_ADDRESS );
  Wire.write(GYRO_CONFIG);
  Wire.write(0x18);
  Wire.endTransmission();
  Serial.println("GYRO_CONFIG Fin!");

  
  Wire.beginTransmission( I2C_ADDRESS );
  Wire.write(USER_CTRL);
  Wire.write(0x00); //close sub I2C
  Wire.endTransmission();
  Serial.println("USER_CTRL Fin!");
  
  Wire.beginTransmission( I2C_ADDRESS );
  Wire.write(INT_PIN_CFG);
  Wire.write(0x02);//bit 1 write 1 to enable bypass
  Wire.endTransmission();
  Serial.println("INT_PIN_CFG Fin!");

  Wire.beginTransmission( 0x0C );
  Wire.write(CNTL1);
  Wire.write(0x1F);//0001 1111 in binary
  Wire.endTransmission();
  Serial.println("CNTL1 Fin!");
  
  delay(100);
  
  Wire.beginTransmission( 0x0C ); // Begin a transmission to the I2C Slave device with the given address.
  Wire.write( ASAX );
  Wire.endTransmission();
  Wire.requestFrom( 0x0C, 3 );
  asaX = (Wire.read()-128)*0.5/128+1;
  asaY = (Wire.read()-128)*0.5/128+1;
  asaZ = (Wire.read()-128)*0.5/128+1;
  Serial.print(asaX);Serial.print("\t");
  Serial.print(asaY);Serial.print("\t");
  Serial.print(asaZ);Serial.println("\t");
  Serial.println("ASA_READ Fin!");
  
  Wire.beginTransmission( 0x0C );
  Wire.write(CNTL1);
  Wire.write(0x00);//0001 1111 in binary
  Wire.endTransmission();
  delay(100);
  Serial.println("CNTL1 reset Fin!");

  Wire.beginTransmission( 0x0C );
  Wire.write(CNTL1);
  Wire.write(0x16);//0001 1111 in binary
  Wire.endTransmission();
  delay(100);
  Serial.println("CNTL1 200Hz Fin!");
}
void recordAccelRegister( int I2C_ADDRESS )  //获取加速度计数据
{
  Wire.beginTransmission( I2C_ADDRESS ); // Begin a transmission to the I2C Slave device with the given address.
  Wire.write( ACCEL_XOUT_H );
  Wire.endTransmission();
  Wire.requestFrom( I2C_ADDRESS, 6);
  ACCEL_X = Wire.read()<<8|Wire.read();
  ACCEL_Y = Wire.read()<<8|Wire.read();
  ACCEL_Z = Wire.read()<<8|Wire.read(); 
}
void recordGryoRegister( int I2C_ADDRESS )  //获取角速度计数据
{
  Wire.beginTransmission( I2C_ADDRESS ); // Begin a transmission to the I2C Slave device with the given address.
  Wire.write( GYRO_XOUT_H );
  Wire.endTransmission();
  Wire.requestFrom( I2C_ADDRESS, 6 );
  GYRO_X = Wire.read()<<8|Wire.read();
  GYRO_Y = Wire.read()<<8|Wire.read();
  GYRO_Z = Wire.read()<<8|Wire.read();
}
void recordMagRegister( int Mag_ADDRESS )  //获取磁强计数据
{

  /*
  Wire.beginTransmission( Mag_ADDRESS );
  Wire.write( 0x0A );
  Wire.write( 0x12 );
  Wire.endTransmission();
  */
  
  Wire.beginTransmission( Mag_ADDRESS );
  Wire.write( ST1 );
  Wire.endTransmission();
  Wire.requestFrom( Mag_ADDRESS, 1 );

  Wire.beginTransmission( Mag_ADDRESS );
  Wire.endTransmission();
  Wire.requestFrom( Mag_ADDRESS, 7 );
  MXL = Wire.read();
  MXH = Wire.read();
  MYL = Wire.read();
  MYH = Wire.read();
  MZL = Wire.read();
  MZH = Wire.read();
  int ST2num = Wire.read();
      
  MAG_X = asaX * short (MXH<<8|MXL);
  MAG_Y = asaY * short (MYH<<8|MYL);
  MAG_Z = asaZ * short (MZH<<8|MZL);
}
void processAccelData() //计算真实加速度 m/s^2
{
  G_FORCE_X = 16*9.798*ACCEL_X / 32767.5;
  G_FORCE_Y = 16*9.798*ACCEL_Y / 32767.5;
  G_FORCE_Z = 16*9.798*ACCEL_Z / 32767.5;
}

void processGryoData()  //计算真实角速度 deg/s
{
  ROT_X = 2000.0*GYRO_X / 32767.5;
  ROT_Y = 2000.0*GYRO_Y / 32767.5;
  ROT_Z = 2000.0*GYRO_Z / 32767.5;
}
