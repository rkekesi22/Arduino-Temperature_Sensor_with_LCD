#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16,2);

const int upButton = 2;
const int downButton = 3;
const int selectButton = 4;


int menu = 1;
const int maxmenu = 2;

void setup() {
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  dht.begin();

  updatemenu();
}

void loop() {


  if ( !digitalRead(downButton))
  {
    menu++;
    if ( menu > maxmenu)
      menu = 1;

    updatemenu();
    delay(2000);
  }

  if ( !digitalRead(upButton))
  {
    menu--;
    if ( menu < 1)
      menu = maxmenu;

    updatemenu();
    delay(2000);
  }

  if( !digitalRead(selectButton))
  {
    executionAction();
    delay(2000);
  }

}

void updatemenu() {
  lcd.clear();

  switch (menu)
  {
    case 1:
      lcd.print("> Homerseklet");
      break;
    case 2:
      lcd.print("> Paratartalom");
      break;
  }
}

void executionAction()
{
  lcd.clear();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature)) {
    Serial.println("Szenzor hiba!");
    return;
  }

  switch (menu)
  {
    case 1:
      lcd.setCursor(0,0);
      lcd.print("> Homerseklet");
      lcd.setCursor(0,1);
      lcd.print(temperature);
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("> Paratartalom");
      lcd.setCursor(0,1);
      lcd.print(humidity);
      break;
  }
}
