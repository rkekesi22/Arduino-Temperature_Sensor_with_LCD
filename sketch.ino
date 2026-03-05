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
   bool actualDownState = digitalRead(downButton);

   if (actualDownState != lastDownState)
   {
      lastDownPressTime = millis();
   }

   if((millis() - lastDownPressTime) > debounce)
   {
    if(actualDownState !=  stableStateForDown)
    {
      stableStateForDown = actualDownState;

      if( stableStateForDown == LOW )
      {
        menu++;
        if ( menu > maxmenu)
            menu = 1;
            
        updatemenu();
      }
    }
   }

   bool actualUpState = digitalRead(upButton);

   if( actualUpState !=  lastUpState)
   {
    lastUpPressTime = millis();
   }

   if ( (millis()-lastUpPressTime) > debounce)
   {
    if( actualUpState != stableStateForUp)
    {
      stableStateForUp = actualUpState;

      if( stableStateForUp == LOW && currentState == STATE_MENU )
      {
        menu--;
        if ( menu < 1)
             menu = maxmenu;
        
        updatemenu();
      }
    }
   }

   bool actualSelectState = digitalRead(selectButton);

   if( actualSelectState != lastSelectState)
   {
    lastSelectPressTime = millis();
   }

   if( (millis() - lastSelectPressTime) > debounce )
   {
    if( actualSelectState !=  stableStateForSelect)
    {
      stableStateForSelect = actualSelectState;

      if( stableStateForSelect == LOW && currentState == STATE_MENU )
      {
        executionAction();
        //delay(2000);

        currentState = STATE_DATA;
        dataStartTime = millis();
      }
    }
   }

   if( currentState == STATE_DATA)
   {
    if( (millis() - dataStartTime) > 2000 )
    {
      currentState = STATE_MENU;
      updatemenu();
    }
   }

   lastUpState = actualUpState;
   lastDownState = actualDownState;
   lastSelectState = actualSelectState;
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
