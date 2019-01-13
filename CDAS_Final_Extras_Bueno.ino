/* Con este sketch controlaremos el encendido y apagado de un led con un botón.  */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h> 
#include <Ethernet.h> // para la conexion de internet
#include <base64.h> // para la encriptacion
 
 

// definición de constantes de los números de pines
const int pinBoton =  7;     // pin del botón   pin 7
const int pinLed   =  2;     // pin del led pin 9
const int pinBuzzer   =  9;     // pin del buzzer pin 9
 /* El zumbador lo pongo negro pin GND y rojo Pin 9
 led  pin Anodo (Mas Largo) pin 2 el corto resistencia  AZUL y pin GND
 boton los dos pines de una cara uno voltaje 5V y el otro resistencia cafe y GND 
 por el otro lado del boton el mismo de la resistencia a pin 7 
o*/
// variable para el valor del estado del botón
int estadoBoton = 0;

//variable del aleatorio
long randomNumber;

// variable de errores
char* desError[]={"P0304", "P0305", "P0306","P0307"};

// variable de almacenamiento
//String errorMostrar;
String errorEnviar;

/* conexion con db_adas */
 

//Direccion mac de mi tarjeta Arduino
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// ip del servidor MySQL
IPAddress server(192, 168, 0, 104); 
//IPAddress server(10, 14, 2, 174); 
 
// Creo un cliente que se conecta mediante IP y puerto
EthernetClient client;
 
//inicializamos la varible
String error_enviar;
 
LiquidCrystal_I2C lcd(0x27,16,2);//definimos objeto LiquidCrystal
void inicio_LCD(){
    lcd.init();//iniciamos lcd
    lcd.backlight();//luz de fondo
    lcd.clear();//limpiar
    lcd.setCursor(0,0);
    lcd.print("  C. D. A. S. ");
    lcd.setCursor(0,1);
    lcd.print("Iniciando.......");
    delay(3000);
}
void inicio_pines(){
      // pin del buzzer como salida
      pinMode(pinBuzzer, OUTPUT);
       // pin del led como salida
      pinMode(pinLed, OUTPUT);
      // pin del botón como entrada
      pinMode(pinBoton, INPUT);
  }
  
void setup() {
  // inicio el puerto serial
    Serial.begin(9600);
    inicio_LCD();
    inicio_RED();
    inicio_pines();
}


void manejo_error(){
        digitalWrite(pinLed, HIGH); // así que enciende el pin
        tone (9, 440, 200);
        //Genera un numero aleatorio entre 1 y 100
        randomNumber = random(1,5);
        // Comprobamos cual es el error a enviar
        for (int i = 1; i < 5; i++){
          if (i == (randomNumber-1) ){
           Serial.println(" El error es : ");
           error_enviar = (desError[i]);
           //para la encriptacion
             String toEncode = error_enviar ;
              String encoded = base64::encode(toEncode);
 
           lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Enviando  Error");
            lcd.setCursor(0,1);
            lcd.print("a Base de Datos");
            delay(3000);
            Serial.println(error_enviar);
            Serial.println("  y su numero corresponde  : ");
            Serial.println(randomNumber);
            Serial.println(error_enviar);
            client.print("GET /insertar.php?bastidor=HG4HY589DNBVE45UR&error=");
            client.print(encoded);
            client.println(" HTTP/1.0");
            client.println();
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Error en BD");
            lcd.setCursor(0,1);
            lcd.print("Registrado");
            delay(3000);
          comprobarrecepcion ();
           i==5;
             //Esperamos 5 segundo para poder enviarlo a la base de datos
          }
       }
  }

void loop() {
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Pulsar Boton ");
     lcd.setCursor(0,1);
     lcd.print("para Continuar");
     delay(3000);
  
      // lee el valor del estado del botón con pinBoton y lo guarda en la variable estadoBoton
      estadoBoton = digitalRead(pinBoton);
      // revisa el estado con un if
      Serial.println(estadoBoton);
      if (estadoBoton == HIGH) {     // si es HIGH, el botón esta oprimido
         manejo_error();
    }
    else // si no es el caso, entonces
      {
          digitalWrite(pinBuzzer, LOW); // el botón no esta oprimido, así que apaga el led
          digitalWrite(pinLed, LOW); // el botón no esta oprimido, así que apaga el led
      }
}


// funciones especificas

void inicio_RED(){
  
  // Pregunto si esta escuchando.
  
  if (Ethernet.begin(mac) == 0) {
    // si no me puedo conectar porque no me han dado una ip valida
    Serial.println("Fallo con el servidor DHCP");
    // no ejecuto el ciclo ya que no me he podido conectar
    for(;;)
      ;
  }
  // lw doy un segundo para que se inicialice la shield de Ethernet
  delay(1000);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Conectando con");
            lcd.setCursor(0,1);
            lcd.print("el Servidor...");
            delay(3000);
  Serial.println("connectando...");
 
  // pregunto si tiene unaconexion que se conecte atravez delpuerto 80:
  if (client.connect(server, 80)) {
    lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Conectado con");
            lcd.setCursor(0,1);
            lcd.print("el Servidor...");
            delay(3000);
    Serial.println("Conectado con el servidor......");
    // hago la solicitud http:
    
  } 
  else {
    // si no me conecto:
    Serial.println("conexion fallida ");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Conexion");
    lcd.setCursor(0,1);
    lcd.print("FALLIDA...");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Deve desconectar");
    lcd.setCursor(0,1);
    lcd.print(" el Enchucfe....");
    delay(3000);
for(;;){/*vacio para terminar*/}
  }
  
  }
  void comprobarrecepcion (){
    {
  //si hay datos entrantes desde el servidor imprimelos
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
 
  // si el servidor no esta en servicio
  if (!client.connected()) {
    Serial.println();
    Serial.println("desconectado......");
    client.stop();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Desconectado");
    lcd.setCursor(0,1);
    lcd.print("Por el servidor");
    delay(3000);
    // sin no 
    for(;;)
      ;
  }
}

    }
