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

State currentState = STATE_MENU;
unsigned long dataStartTime = 0;

const int upButton = 2;
const int downButton = 3;
const int selectButton = 4;

unsigned long lastDownPressTime = 0;
unsigned long lastUpPressTime = 0;
unsigned long lastSelectPressTime = 0;
unsigned long debounce = 50;

bool lastDownState = HIGH;
bool lastUpState = HIGH;
bool lastSelectState = HIGH;

bool stableStateForDown = HIGH;
bool stableStateForUp = HIGH;
bool stableStateForSelect = HIGH;

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

  //Down
  if( checkButton(downButton,lastDownState,stableStateForDown,lastDownPressTime,debounce) && currentState == STATE_MENU)
  {
      menu++;
      if ( menu > maxmenu)
            menu = 1;
            
      updatemenu();
  }

  //Up
  if( checkButton(upButton,lastUpState,stableStateForUp,lastUpPressTime,debounce) && currentState == STATE_MENU)
  {
      menu--;
        if ( menu < 1)
             menu = maxmenu;
        
        updatemenu();
  }

  //Select
  if( checkButton(selectButton,lastSelectState,stableStateForSelect,lastSelectPressTime,debounce) )
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

bool checkButton( int pin, 
bool &lastRead, 
bool &stableState, 
unsigned long &lastChange, 
unsigned long debounceTime)
{
  bool reading = digitalRead(pin);

   if( reading !=  lastRead)
   {
    lastChange = millis();
   }

   if ( (millis()-lastChange) > debounceTime)
   {
    if( reading != stableState)
    {
      stableState = reading;

      if( stableState == LOW )
      {
        lastRead = reading;
        return true;
      }
    }
   }

   lastRead = reading;
   return false;

}

