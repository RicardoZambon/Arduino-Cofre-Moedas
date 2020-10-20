#include <LiquidCrystal.h>

LiquidCrystal lcd(11, 10, 6, 5, 4, 3);

const int iPinIn[6] = {A0,A1,A2,A3,A4,A5};
const int iPinOut = 1;

int iVermelho = 2, iAzul = 7, iVerde = 8;
int iBuzzer = 12;
double iContador = 0;

int iSensorValue[5];


#define LED_PIN 13
#define TOGGLE_LED_STATE 't'
#define EOL_DELIMITER "\n"
int incoming = 0;
boolean shouldBlinkLED = false;


void setup() {
    Serial.begin(9600);
    
    pinMode(iVermelho, OUTPUT);
    pinMode(iAzul, OUTPUT);
    pinMode(iVerde, OUTPUT);
    pinMode(iBuzzer, OUTPUT);
    
    lcd.begin(16, 2);
    lcd.setCursor(0, 0); //Coluna 0, Linha 0
    lcd.print("Cofre digital");
    
    Serial.begin(57600);
    Serial.print("INITIALIZING");
    Serial.print(EOL_DELIMITER);
    pinMode(LED_PIN, OUTPUT);
    blinkLED(5);
    Serial.print("READY");
    Serial.print(EOL_DELIMITER);
}

void loop() {
    LeMoeda();
    //LeNota();
    
    delay(2000);
    Serial.println("teste");
    delay(2000);
    Serial.println("atividade1");
    delay(2000);
    Serial.println("atividade2");
    delay(2000);
    Serial.println("atividade3");
    delay(2000);
    Serial.println("atividade4");
    
    if(shouldBlinkLED) blinkLED(1);
    if(Serial.available() > 0) {
        incoming = Serial.read();
        if(incoming == TOGGLE_LED_STATE) {
            shouldBlinkLED = !shouldBlinkLED;
	    Serial.print("LED BLINK STATE: "); 
	    if(shouldBlinkLED)
                Serial.print("ON");
	    else
          	Serial.print("OFF");
            Serial.print(EOL_DELIMITER);
    	}
    }
}

void LeMoeda() {
    for (int i=0; i<5; i++) {
        iSensorValue[i] = analogRead(iPinIn[i]);
    }
    int iOutputValue = map(iSensorValue[0], 0, 1023, 0, 255);
    analogWrite(iPinOut, iOutputValue);
    
    for (int i=0; i<5; i++) {
        Serial.print("Sensor " + String(i) + " = " + String(iSensorValue[i]));
        if (i!=4) Serial.print("; ");
    }
    Serial.println("");
    
    if (iSensorValue[0] < 250 && iSensorValue[1] < 200 && iSensorValue[2] < 200 && iSensorValue[3] < 200 && iSensorValue[4] < 200) {
        iContador = iContador + 1;
        Serial.println(">> + R$ 1,00");// - Total R$ " + String(iContador));
        EscreveLCD();
    } else if (iSensorValue[1] < 200 && iSensorValue[2] < 200 && iSensorValue[3] < 200 && iSensorValue[4] < 200) {
        iContador = iContador + 0.5;
        Serial.println(">> + R$ 0,50");// - Total R$ " + String(iContador));
        EscreveLCD();
    } else if (iSensorValue[2] < 200 && iSensorValue[3] < 200 && iSensorValue[4] < 200) {
        iContador = iContador + 0.25;
        Serial.println(">> + R$ 0,25");// - Total R$ " + String(iContador));
        EscreveLCD();
    } else if (iSensorValue[3] < 200 && iSensorValue[4] < 200) {
        iContador = iContador + 0.1;
        Serial.println(">> + R$ 0,10");// - Total R$ " + String(iContador));
        EscreveLCD();
    } else if (iSensorValue[2] < 250 && iSensorValue[3] < 200 && iSensorValue[4] < 200) {
        iContador = iContador + 0.05;
        Serial.println(">> + R$ 0,05");// - Total R$ " + String(iContador));
        EscreveLCD();
    }
    delay(10);
}

void LeNota() {
    String sCores[3] = {"Vermelho","Azul","Verde"};
    int iPinos[3] = {iVermelho, iAzul, iVerde};
    
    for (int i = 0; i<3; i++) {        
        digitalWrite(iPinos[i], HIGH);
        int iValorSensor = analogRead(A5);
        int iOutputValue = map(iValorSensor, 0, 1023, 0, 255);
        analogWrite(A5, iOutputValue);
        Serial.println("Valor do " + sCores[i] + " = " + String(iValorSensor) + ";\t Valor saida = " + String(iOutputValue) + ";");
        delay(1000);
        digitalWrite(iPinos[i], LOW);
    }
    delay(10);
}

void EscreveLCD() {
    lcd.setCursor(0, 1); //Coluna 0, Linha 1
    lcd.print(iContador);
    TocarSom();
}

void TocarSom() {
    tone(iBuzzer,1500);
    delay(500);
    noTone(iBuzzer);
}

void blinkLED(int count) {
    for(int i = 0; i < count; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
}
