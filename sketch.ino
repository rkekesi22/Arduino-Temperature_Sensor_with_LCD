#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16,2);

enum State {
  STATE_MENU,
  STATE_DATA
};

struct Button {
  uint8_t pin;
  bool lastRead;
  bool stableState;
  unsigned long lastChange;
};

Button btnUp = {2, HIGH, HIGH, 0};
Button btnDown = {3, HIGH, HIGH, 0};
Button btnSelect = {4, HIGH, HIGH, 0};

State currentState = STATE_MENU;
unsigned long dataStartTime = 0;

unsigned long debounce = 50;

int menu = 1;
const int maxmenu = 2;

void setup() {
  pinMode(btnUp.pin, INPUT_PULLUP);
  pinMode(btnDown.pin, INPUT_PULLUP);
  pinMode(btnSelect.pin, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  Serial.begin(9600);
  dht.begin();

  updatemenu();
}

void loop() {

  //Down
  if( checkButton(btnDown,debounce) && currentState == STATE_MENU)
  {
      menu++;
      if ( menu > maxmenu)
            menu = 1;
            
      updatemenu();
  }

  //Up
  if( checkButton(btnUp,debounce) && currentState == STATE_MENU)
  {
      menu--;
        if ( menu < 1)
             menu = maxmenu;
        
        updatemenu();
  }

  //Select
  if( checkButton(btnSelect,debounce) )
  {
    executionAction();
    currentState = STATE_DATA;
    dataStartTime = millis();
  }
   
  if( currentState == STATE_DATA)
  {
    if( (millis() - dataStartTime) > 2000 )
    {
      currentState = STATE_MENU;
      updatemenu();
    }
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

  if (isnan(temperature) || isnan(humidity)) {
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

bool checkButton( Button &button,
unsigned long debounceTime)
{
  bool reading = digitalRead(button.pin);

   if( reading !=  button.lastRead)
   {
    button.lastChange = millis();
   }

   if ( (millis()-button.lastChange) > debounceTime)
   {
    if( reading != button.stableState)
    {
      button.stableState = reading;

      if( button.stableState == LOW )
      {
        button.lastRead = reading;
        return true;
      }
    }
   }

   button.lastRead = reading;
   return false;

}

