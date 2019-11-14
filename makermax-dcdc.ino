/** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2019 MAKERMAX INC.
 * https://makermax.ca
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of MAKERMAX INC. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * USE THIS CODE TO RUN THE MAKERMAX DCDC CODE WITH ARDUINO UNO R3. TO TEST THE CODE
 * VARY THE POTENTIOMETER ON THE BOARD, TO GET A VARIABLE VOLTAGE OUTPUT 4 - 20V WITH
 * INPUT VOLTAGE BEING A FIXED 12V INPUT.
 ******************************************************************************/
 
#include <Wire.h> 
//pot on A2, Vin on A4, Iout on A3, Vout on A5
int duty=1;
int duty2=0;
float outputvoltage=0;
float set=analogRead(A2);
float setvoltage=0;
int current=0;
float out=0;
float in=12.2;
int mode=0;
void setup() {
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
  TCCR1B = (TCCR1B & 0b11111000) | 0x01;
  pinMode(13,OUTPUT);
  delay(2000);
  analogWrite(5,duty);
  analogWrite(6,duty2);
  delay(2000);
  

}

void loop() 
{

set=analogRead(A2); //read pot value
set=set/680;
set=set*20;
setvoltage=3.5+set;  //convert pot value to voltage
delay(1000);

current=analogRead(A3);  // read current
outputvoltage=analogRead(A5); //read output voltage from divider
out=outputvoltage;
out=outputvoltage/1023;
out=out*4.9;
out=out*10.5;  // convert to true output voltage


if(setvoltage<in-0.1)
{
  mode=0;
}

while(mode==0){   //mode 0= buck mode


  
set=analogRead(A2); //read pot value
set=set/680;
set=set*20;
setvoltage=3.5+set;  //convert pot value to voltage

delay(1000);

outputvoltage=analogRead(A5); //read output voltage from divider
out=outputvoltage;
out=outputvoltage/1023;
out=out*4.9;
out=out*10.5;  // convert to true output voltage

delay(1000);


if(out<setvoltage)
{
  duty=duty+1;
  analogWrite(5,duty);
  delay(1000);
}

outputvoltage=analogRead(A5); //read output voltage from divider
out=outputvoltage;
out=outputvoltage/1023;
out=out*4.9;
out=out*10.5;  // convert to true output voltage

delay(1000);

if(out>=setvoltage)
{
  duty=duty-1;
  analogWrite(5,duty);
  delay(1000);
}  


if(setvoltage>in+0.2)  //hysterisis of input +0.2 to prevent instability
{
  mode=1;
}
}

while(mode==1)  // loop for boost mode
{
  analogWrite(5,255); //set buck duty to 1



if(duty2>60)
{
  duty2=60;  // limit on max boost duty cycle limit high voltage
}
  
set=analogRead(A2); //read pot value
set=set/680;
set=set*20;
setvoltage=3.5+set;  //convert pot value to voltage

delay(1000);


outputvoltage=analogRead(A5); //read output voltage from divider
out=outputvoltage;
out=outputvoltage/1023;
out=out*4.9;
out=out*10.5;  // convert to true output voltage

delay(1000);




if(out<setvoltage)
{
  duty2=duty2+1;
  if(duty2>60)
  duty2=60;
  analogWrite(6,duty2);
  delay(1000);
}

outputvoltage=analogRead(A5); //read output voltage from divider
out=outputvoltage;
out=outputvoltage/1023;
out=out*4.9;
out=out*10.5;  // convert to true output voltage

if(setvoltage<in-0.1)
{
  mode=0;
}

delay(100);

if(out>=setvoltage)
{
  duty2=duty2-1;
  if(duty2>60)
  duty2=60;
  analogWrite(6,duty2);
  delay(1000);
}

if(setvoltage<in-0.1)
{
  mode=0;
}

}
}



//Use below code if required
//current protection
//while(current>800)
//duty=duty-1;
//duty2=duty-1;
