/*
 This is code for GML meteorological station - Mateo
------------------------------------------------------------------------
 Here is the code of Arduino ethernet website, please, rewrite it every
 time you will change it in the code bellow.
* <!DOCTYPE HTML>
* <html>
* 	<head>
*     <meta content="charset=utf-8;" />
* 		<title>Mateo</title>
* 	</head>
* 	<body style="background-color:#FF0000;color=#3333FF">
* 		<h1>Mateo - meteorologická stanice GML</h1>
* 		<p style="font-size=20pt;">q[i].name je q[i].value q[i].unit</p>
* 	</body>
* </html>
*/

#include <SPI.h>
#include <EthernetENC.h>
//#include <SD.h>
//#include <bme280.h>

// structure for the quantities, that will be used on Mateo
// if it is int value, isFloat=0, otherwise isFloat=1
struct quantity{
	//bool isFloat;
	float value1;
	//int value2;
	const char* name;//[20];
	const char* unit;//[3];
};

// number of quantities, used in loops, change automaticly
int qn=0;

// variable for time
long lastTime = 0;

// array fo quantities for better usage in for loop
quantity q[5];

// function for simplify adding new quantities
void initQuantity(int i, const char* n, float v1, const char* u){
	q[i].name=n;
	q[i].value1=v1;
	q[i].unit=u;	
	qn++;
}

// some variables to be used for ethernet
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

IPAddress ip(192, 168, 0, 177);

EthernetServer server(12345);
EthernetClient client;


//----------------------------------------------------------------------

void setup(){
  Ethernet.init(10);
	Serial.begin(9600);
  // wait for serial to start
	while(!Serial){;}
	Serial.println("Mateo");
	//Setup ethernet(CS=10)
	Ethernet.begin(mac, ip);
  delay(10);

  // Two conditions to fastly solve occuring problems
	if(Ethernet.hardwareStatus() == EthernetNoHardware) {
		Serial.println("Zkontroluj zapojení modulu");
		while(true){
			delay(1);
		}
	}
	if(Ethernet.linkStatus() == LinkOFF) {
		Serial.println("Zkontroluj zapojení ethernet kabelu");
		//while(true){
		delay(1);
		//}
	}
 
  // Start server and print its local IP Adr
	server.begin();
	Serial.print("IP Adresa:");
	Serial.println(Ethernet.localIP());
  delay(10);
  // Initialize new quantities
	initQuantity(0, "Teplota", 0.0, "C");
	initQuantity(1, "Tlak", 0.0, "Pa");
	initQuantity(2, "Vlhkost", 0.0, "%");
	initQuantity(3, "Směr větru", 0.0, "pi/4");
	initQuantity(4, "Rychlost větru", 0.0, "m/s");
  //Serial.print("Inicializace SD...");
  /*if (!SD.begin(4)) {
    //Serial.println("Chyba");
    // don't do anything more:
    //while (1);
  }
  //Serial.println("inicializovano");*/
}

void loop(){
  // Create new http client if available
	client = server.available();
	if(client) {
		Serial.println("Nový klient");
		boolean currentLineIsBlank = true;
		while(client.connected()){
			if(client.available()){
				char c = client.read();
				Serial.write(c);
				if (c == '\n' && currentLineIsBlank){
          // Print http communication standards
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Connection: close");  // the connection will be closed after completion of the response
					client.println("Refresh: 20");  // refresh the page automatically every 5 sec
					client.println();
          // Print the website as described up on te page
					client.println("<!DOCTYPE HTML>");
					client.println("<html>");
					client.println("<head>");
					client.println("<meta charset=utf-8>");
					client.println("<title>Mateo</title>");
					client.println("</head>");
					client.println("<body style=\"background-color:#3333FF;\">");
					client.println("<h1 style=\"font-size:35pt;color:#FFFFFF;\">Mateo - meteorologická stanice GML</h1>");
					// For loop used to write all quantities:
					for(int i=0; i<qn; i++){
						client.print("<p style=\"font-size:20pt;color:#FFFFFF;\">");
						client.print(q[i].name);
						client.print(" je ");
					  client.print(q[i].value1);
						client.print(q[i].unit);
						client.println("</p>");
					}
          client.println("<a style=\"background-color:#FFFFFF;color:3333FF;font-size:25pt\" href=\"https://letkagml.space\">Letka GML</a>");
					client.println("</body>");
					client.println("</html>");
					break;
				}
				if (c == '\n') {
					currentLineIsBlank = true;
				} else if (c != '\r') {
					currentLineIsBlank = false;
				}
			}
		}
		delay(1);
    // End communication with client
		client.stop();
		Serial.println("Klient odpojen");
		
	}
  if(millis()-lastTime>20000){
    q[0].value1 = random(-20, 40) + float(random(0, 99))/100;
    q[1].value1 = random(50000, 150000);
    q[2].value1 = random(0, 99) + float(random(0, 9))/10;
    q[3].value1 = random(0, 15);
    q[4].value1 = random(0, 30) + float(random(0, 99))/100;
    lastTime = millis();
    // make a string for assembling the data to log:
    /*String dataString = "";

    // read three sensors and append to the string:
    for (int i = 0; i < qn; i++) {
      dataString += String(q[i].isFloat ? q[i].value1 : q[i].value2);
      if (i < qn-1) {
       dataString += ",";
      }
    }

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      //Serial.println("error opening datalog.txt");
    }*/
  }
}
