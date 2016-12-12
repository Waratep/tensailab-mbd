uint8_t count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t data;
  
  Serial.write(count);
  count += 10;
  count %= 250;
  if (Serial.available()) {
    data = Serial.read();
    if (data == 1) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
  }
  delay(2000);
}

