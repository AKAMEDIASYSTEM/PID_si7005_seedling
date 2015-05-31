
void updateDisplay(float temp, float humid, boolean isActive) {

  // Heater State display
  display.setCursor(0, 0);
  if (isActive) {
    display.print("Heater ON");
  }
  else {
    display.print("Heater OFF");
  }

  // temperature display
  display.setCursor(0, 9);
  char tempF[4]; // buffer for temp incl. decimal point & possible minus sign
  String tempString = "";
  dtostrf(temp, 4, 2, tempF); // Min. 6 chars wide incl. decimal point, 2 digits right of decimal
  for (int i = 0; i < sizeof(tempF); i++)
  {
    tempString += tempF[i];
  }
  display.print("TempC: ");
  display.print(tempString);

  // humidity display
  display.setCursor(0, 18);
  char humS[4]; // buffer for temp incl. decimal point & possible minus sign
  String humString = "";
  dtostrf(humid, 4, 2, humS); // Min. 6 chars wide incl. decimal point, 2 digits right of decimal//convert chararray to string
  for (int i = 0; i < sizeof(humS); i++)
  {
    humString += humS[i];
  }
  display.print("Humid: ");
  display.print(humString);

  // PID state display
  display.setCursor(72, 0);
  display.print("Set: ");
  display.print(int(Setpoint));

  // PID state display
  display.setCursor(72, 18);
  display.print("Out: ");
  display.print(int(Output));

  display.display();
}
