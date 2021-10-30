# README

1. `readUltrasonicDistance()`

   - Used to send and receive the Ultra sonic sound.
   - Returns the time taken for roundtrip of sound   

2. `loop()`

   - Calls ` Blynk.run()` to configure the Blynk Server

   - In each iteration of `loop()`,  we calculate the height of the coil from the HS CR04 sensor using the formula

     `float x = 0.0172 * readUltrasonicDistance(TRIGGER_PIN, ECHO_PIN)`

     Time taken by sound to travel a round trip of 1 cm = 57 us
     Let time taken to travel x cm round trip be t cm. 
     Then the distance `x = t/57  = 0.0172*t`

   - After 3 iterations of `loop()` , the average height is calculated and sent to the Blynk dashboard using ` Blynk.virtualWrite(V0,RST_HT-avg)`.

     Here `RST_HT` is set to 12 cm, which is the height of the sensor from the ground level.

3. `BLYNK_WRITE(V1)`

   - Used to access the data sent from Blynk using `param.asInt()` 

     Here we have used it to control the current using PWM

   

