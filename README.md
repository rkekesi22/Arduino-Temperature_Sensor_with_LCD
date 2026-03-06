# Arduino - Temperature_Sensor_with_LCD

DHT22 szenzor és LCD 16x2 I2C kijelzővel kialakítottam egy menürendszert, melyben 2 lehetőség közül választhatunk:

- Hőmérséklet
- Páratartalom

A választáshoz 3 gomb áll rendelkezésre.

- Up Button
- Down Button
- Select Button

A menüben való választás után megjelenik a szenzor által mutatott  hőmérséklet/páratartalom adat.

Extra:
- Millis() használata
- Checkbutton függvény használata
- Összetett adattípus (struct) használata a pin, lastRead, stableState, lastChange változók helyettesítésére. Így a kód sokkal átláthatóbb

<img width="952" height="554" alt="image" src="https://github.com/user-attachments/assets/cce0dce0-f5c5-44be-a524-d7b198bc3bd6" />
