void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
}

void loop() {
  Serial.print("A0");
  Serial.print("\t");
  Serial.print(analogRead(A0));
  Serial.print("\n");
  Serial.print("A1");
  Serial.print("\t");
  Serial.print(analogRead(A1));
  Serial.print("\n");
  Serial.print("A2");
  Serial.print("\t");
  Serial.print(analogRead(A2));
  Serial.print("\n");
  Serial.print("A9");
  Serial.print("\t");
  Serial.print("D9");
  Serial.print("\t");
  Serial.print(digitalRead(9));
  Serial.print("\n");
  Serial.print("D10");
  Serial.print("\t");
  Serial.print(digitalRead(10));
  Serial.print("\n");
  Serial.print("D11");
  Serial.print("\t");
  Serial.print(digitalRead(11));
  Serial.print("\n");
  delay(100);
}

