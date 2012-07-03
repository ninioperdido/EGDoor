

#include <SoftwareSerial.h>   
#include <Bluetooth.h>

#define RxD 2
#define TxD 3
#define BR 38400
#define DEBUG_ENABLED  1

//Start symbol when returning INQuiry result
String retSymb = "+RTINQ=";
//PIN Code of this device
String pinCode = "5678";
//Name of this device:
String deviceName = "EGDoorProto1";
//List of authorized devices
String authorizedDevices = "00,24,BA,FB,67,16";
//String authDevices = "90,C1,15,80,57,F5|C8,BC,C8,B1,F2,46|";
String authDevices = "00,24,BA,FB,67,16|90,C1,15,80,57,F5|C8,BC,C8,B1,F2,46|";
// caution that ';'must be included, and make sure the slave name is right.
int nameIndex = 0;
int addrIndex = 0;

String recvBuf;
String slaveAddr;

String connectCmd = "\r\n+CONN=";

SoftwareSerial BTSerial(RxD,TxD);
Bluetooth bt(RxD, TxD, BR);


void setup() 
{ 
  Serial.begin(9600);
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBT();
  //wait 1s and flush the serial buffer
  delay(1000);
  Serial.flush();
  BTSerial.flush();
} 
 
void loop() 
{
  tryToConnectToAuthDev(authDevices);
  char recvChar;
  while(1){
    if(BTSerial.available()){
      //check if there's any data sent from the remote bluetooth shield
      recvChar = BTSerial.read();
      Serial.print(recvChar);
    }
    if(Serial.available()){
      //check if there's any data sent from the local serial terminal
      //you can add the other applications here
      recvChar  = Serial.read();
      BTSerial.print(recvChar);
    }	
 } 
} 
 
void setupBT()
{
  //Set BT BaudRate
  BTSerial.begin(38400);
  //Set the BT work in master mode
  BTSerial.print("\r\n+STWMOD=1\r\n");
  //Set the BT name
  deviceName = "\r\n+STNA=" + deviceName + "\r\n";
  BTSerial.print(deviceName);
  //Set the BT pin code
  pinCode = "\r\n+STPIN=" + pinCode + "\r\n";
  BTSerial.print(pinCode);
  // Auto-connection is forbidden here
  BTSerial.print("\r\n+STAUTO=0\r\n");
  delay(2000); // This delay is required.
  BTSerial.flush();
  //BTSerial.print("\r\n+INQ=1\r\n");//make the master inquire
  //Serial.println("Inquiring!");
  delay(2000); // This delay is required.
}

void tryToConnectToAuthDev(String authDevices)
{
  int lastIndex = 0;
  int length = authDevices.length();
  int index = authDevices.indexOf("|");
  String statusMessage = "Devices Length " + String(length) + " trying in position " + String(index);
  Serial.println(statusMessage);
  while (index < length)
  {
    String myConnectCmd = connectCmd;
    slaveAddr = authDevices.substring(lastIndex,index);
    //form the full connection command
    myConnectCmd += slaveAddr;
    myConnectCmd += "\r\n";
    //int connectOK = 0;
    Serial.print("Trying to connect to ...");
    Serial.println(slaveAddr);
    Serial.println(myConnectCmd);
    int connectOK = 0;

    do {
      BTSerial.print(myConnectCmd);//send connection command
      String recvBuf = "";
      char recvChar;
      while(1){
        if(BTSerial.available()){
          recvChar = BTSerial.read();
 	  recvBuf += recvChar;
          Serial.print(recvChar);
        if (recvBuf.indexOf("CONNECT:OK") != -1){
          connectOK = 1;
 	  Serial.println("Connected!");
 	  BTSerial.print("Connected!");
 	  break;
 	}
        else if(recvBuf.indexOf("CONNECT:FAIL") != -1){
 	  Serial.println("Connect again!");
 	  break;
 	}
      }
    }
  }while(0 == connectOK);
  }
}

void inQuiRy()
{
//find the target slave
  char recvChar;
  while(1){
    if(BTSerial.available()){
      recvChar = BTSerial.read();
      recvBuf += recvChar;
    }
    nameIndex = recvBuf.indexOf(authorizedDevices);//get the position of slave name
      //nameIndex -= 1;//decrease the ';' in front of the slave name, 
      // to get the position of the end of the slave address
      if ( nameIndex != -1 ){
        Serial.println("We found a match inside authorizedDevices");
 	slaveAddr = recvBuf.substring(nameIndex, nameIndex+18);//get the string of slave address 			
 	break;
      }
  }
  //form the full connection command
  connectCmd += slaveAddr;
  connectCmd += "\r\n";
  int connectOK = 0;
  Serial.print("Connecting to slave:");
  Serial.print(slaveAddr);
  //connecting the slave till they are connected
  do{
    BTSerial.print(connectCmd);//send connection command
    recvBuf = "";
    while(1){
      if(BTSerial.available()){
        recvChar = BTSerial.read();
 	recvBuf += recvChar;
        Serial.print(recvChar);
        if (recvBuf.indexOf("CONNECT:OK") != -1){
          connectOK = 1;
 	  Serial.println("Connected!");
 	  BTSerial.print("Connected!");
 	  break;
 	}else if(recvBuf.indexOf("CONNECT:FAIL") != -1){
 	  Serial.println("Connect again!");
 	  break;
 	}
      }
    }
  }while(0 == connectOK);
}
 

