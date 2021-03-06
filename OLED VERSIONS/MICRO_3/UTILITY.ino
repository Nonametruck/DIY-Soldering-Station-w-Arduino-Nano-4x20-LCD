//=======================================================================
//    UTILITIES
//=======================================================================
void beepBuzzer(unsigned long hz, unsigned long ms) 
{ 
  /*unsigned long us = (750000 / hz);  
   unsigned long rep = (ms * 500L) / us; 
   
   for (int i = 0; i < rep; i++) 
   {  
   digitalWrite(BUZZER_PIN, HIGH);  
   delayMicroseconds(us);  
   digitalWrite(BUZZER_PIN, LOW);  
   delayMicroseconds(us);  
   }*/
}
//=======================================================================
void setPwmFrequency(int pin, int divisor) 
{
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) 
  {
    switch(divisor) {
    case 1: 
      mode = 0x01; 
      break;
    case 8: 
      mode = 0x02; 
      break;
    case 64: 
      mode = 0x03; 
      break;
    case 256: 
      mode = 0x04; 
      break;
    case 1024: 
      mode = 0x05; 
      break;
    default: 
      return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } 
    else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } 
  else if(pin == 3 || pin == 11) 
  {
    switch(divisor) 
    {
    case 1: 
      mode = 0x01; 
      break;
    case 8: 
      mode = 0x02; 
      break;
    case 32: 
      mode = 0x03; 
      break;
    case 64: 
      mode = 0x04; 
      break;
    case 128: 
      mode = 0x05; 
      break;
    case 256: 
      mode = 0x06; 
      break;
    case 1024: 
      mode = 0x7; 
      break;
    default: 
      return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
//=======================================================================
int getTemperature()
{  
  analogWrite(PWMpin, 0);		//switch off heater
  delay(DELAY_MEASURE);			//wait for some time (to get low pass filter in steady state)

  total = total - readings[readIndex];
  readings[readIndex] = analogRead(TEMPin);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) 
  {
    readIndex = 0;
  }
  adcValue = total / numReadings;
  //  adcValue = 128;
  analogWrite(PWMpin, pwm);	//switch heater back to last value
  return round(((float) adcValue)*ADC_TO_TEMP_GAIN+ADC_TO_TEMP_OFFSET); //apply linear conversion to actual temperature
}
//=======================================================================

void splashScreen()
{
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  u8g.drawStr(0, 0, "  ARDUINO MICRO PRO  ");
  u8g.drawStr(0, 15, "  SOLDERING STATION  ");
  u8g.drawStr(0, 30, "   MANOS MAR. 2016   ");
  u8g.drawStr(0, 45, "    96W/25-400 oC    ");    
}  
//=======================================================================
void checkTimer()
{
  if(mins >= TIMER_10MIN)
  {

  }  
  if(mins >= TIMER_20MIN)
  {
    pwm = 0;
    digitalWrite(HEAT_LED, LOW);
    heater = LOW;   

    sw_millis.reset();  
    sw_millis.stop();    
    seconds = 0;
    minutes = 0;
    sw_countdownmillis.reset();
    sw_countdownmillis.stop();
    secs = 0;
    mins = 0;
  }  
}
//=======================================================================
void showTemps()
{
  u8g.setFont(u8g_font_6x13);
  u8g.drawStr( 0, 22, "SHOW TEMPS");
  showTime();
}   
//=======================================================================
void showTime()
{
  u8g.setFont(u8g_font_6x13);
  u8g.drawStr( 0, 44, "SHOW TIME");
  //lcd.setCursor(line, row);
  if(sw_millis.elapsed() > 999)
  {
    seconds++;
    sw_millis.reset();
    sw_millis.start();
  }
  if(seconds > 59)
  {
    seconds = 0;
    minutes++;
  }

  if(minutes < 10)
  {
    //lcd.print("0");
  } 
  //  lcd.print(minutes,DEC);
  //  lcd.print(":");
  if(seconds < 10)
  {
    //    lcd.print("0");
  } 
  //  lcd.print(seconds,DEC);  
}
//=======================================================================
void showCountdownTime(int row, int line)
{
  //lcd.setCursor(row, line);
  if(sw_countdownmillis.elapsed() > 999)
  {
    secs++;
    sw_countdownmillis.reset();
    sw_countdownmillis.start();
  }  
  if(secs > 59)
  {
    secs = 0;
    mins++;
  }

  if(mins < 10)
  {
    //lcd.print("0");
  } 
  // lcd.print(mins,DEC);
  // lcd.print(":");
  if(secs < 10)
  {
    //  lcd.print("0");
  } 
  //lcd.print(secs,DEC);
  u8g.setFont(u8g_font_6x13); 
  u8g.drawStr(0, 0, "COUNTDOWN");

} 
//=======================================================================
void checkUnplugged()
{
  if((actual_temperature >= MAX_TEMP) && (actual_temperature < MAX_TEMP+100))
  {    
    pwm = 0;
    digitalWrite(HEAT_LED, LOW);
    actual_temperature = 0; 
    //lcd.clear();

    /*     
     lcd.setCursor(5,0);
     lcd.print(F("UNPLUGGED!"));
     lcd.setCursor(0,1);
     lcd.print(F("   PLEASE CONNECT   "));
     lcd.setCursor(0,2);
     lcd.print(F("        PLUG!       "));
     lcd.setBacklight(state);
     */
    actual_temperature = getTemperature();

    //    lcd.clear();

    //    lcd.setCursor(14,3); 
    //    lcd.print(will_temp);

    /*    lcd.setCursor(4,2);
     if (t1 < 100)
     lcd.print(" ");
     if (t1 <10)
     lcd.print(" ");
     lcd.print(t1); 
     
     lcd.setCursor(4,3);
     if (t2 < 100)
     lcd.print(" ");
     if (t2 <10)
     lcd.print(" ");
     lcd.print(t2); 
     
     lcd.setCursor(14,2);
     if (p1 < 100)
     lcd.print(" ");
     if (p1 <10)
     lcd.print(" ");
     lcd.print(p1); */
  }
}
//=======================================================================
void writeHEATING(int tempWILL, int tempVAL, int pwmVAL)
{  
  static int d_tempWILL = 1;//2		
  static int tempWILL_OLD = 1;//10
  static int tempVAL_OLD = 1;//10
  static int pwmVAL_OLD	= 1;//10 
  pwmVAL = map(pwmVAL, 0, 255, 0, 99);  

  u8g.firstPage();  
  do 
  {  
    //ACTUAL TEMPERATURE DISPLAY
    if (tempVAL_OLD != tempVAL)
    {
      u8g.setFont(u8g_font_helvR24r);
      u8g.setPrintPos(64, 26);
      u8g.print(tempVAL_OLD/100);

      if (((tempVAL_OLD/10)%10) != ((tempVAL/10)%10))
      {
        u8g.setPrintPos(84, 26);
        u8g.print((tempVAL_OLD/10)%10);
      } 

      if ((tempVAL_OLD%10) != (tempVAL%10))
      {
        u8g.setPrintPos(104, 26);
        u8g.print(tempVAL_OLD%10);      
      }

      u8g.setPrintPos(64, 26);
      if (tempVAL < 100)
        u8g.print(" ");

      if (tempVAL <10)
        u8g.print(" ");

      u8g.print(tempVAL); 
      t1 = tempVAL;
      tempVAL_OLD = tempVAL; 
    }
  } 
  while(u8g.nextPage());

  u8g.firstPage();  
  do 
  {  
    //DESIRED TEMPERATURE DISPLAY
    if ((tempWILL_OLD+d_tempWILL < tempWILL) || (tempWILL_OLD-d_tempWILL > tempWILL))
    { 
      if ((tempWILL_OLD/100) != (tempWILL/100))
      {
        u8g.setPrintPos(64, 50);
        u8g.print(tempWILL_OLD/100);
      }

      if (((tempWILL_OLD/10)%10) != ((tempWILL/10)%10))
      {
        u8g.setPrintPos(84, 50);
        u8g.print((tempWILL_OLD/10)%10);
      }

      if ((tempWILL_OLD%10) != (tempWILL%10))
      {
        u8g.setPrintPos(104, 50);
        u8g.print(tempWILL_OLD%10);
      }  

      u8g.setPrintPos(64, 50);
      if (tempWILL < 100)
        u8g.print(" ");
      if (tempWILL <10)
        u8g.print(" ");
      u8g.print(tempWILL); 
      t2 = tempWILL;
      tempWILL_OLD = tempWILL;
    }
  } 
  while(u8g.nextPage()); 

  u8g.firstPage();  
  do 
  {  
    //PWM PERCENTAGE DISPLAY
    if (pwmVAL_OLD != pwmVAL)
    {
      u8g.setFont(u8g_font_helvR14r);
      if ((pwmVAL_OLD/100) != (pwmVAL/100))
      {
        u8g.setPrintPos(0, 22);
        u8g.print(pwmVAL_OLD/100);
      }      
      if (((pwmVAL_OLD/10)%10) != ((pwmVAL/10)%10))
      {
        u8g.setPrintPos(0, 42);
        u8g.print((pwmVAL_OLD/10)%10 );
      }  
      if ((pwmVAL_OLD%10) != (pwmVAL%10))
      {
        u8g.setPrintPos(0, 62);
        u8g.print(pwmVAL_OLD%10 );
      }        

      u8g.print("%");
      u8g.setPrintPos(0, 22);
      if (pwmVAL < 100)
        u8g.print(" ");
      if (pwmVAL <10)
        u8g.print(" ");
      u8g.print(pwmVAL);
      p1 = pwmVAL; 
      pwmVAL_OLD = pwmVAL;
    } 
  } 
  while(u8g.nextPage());
}
//=======================================================================
//=======================================================================







