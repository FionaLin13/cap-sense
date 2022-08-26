void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  String cmd;
  if (Serial.available()) {
    cmd = Serial.read();
    if (cmd == "s\n") {
      Serial.print("start command received\n");
    }
    if (cmd == "e\n") {
      Serial.print("end command received\n");
    }
  }
}
