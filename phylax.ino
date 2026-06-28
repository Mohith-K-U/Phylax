/*

This is the code for fingerprint biometric lock system.
 Author name: Mohith K U
 Author email: mohithmandanna29@gmail.com
 */
 
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ENROLL_BUTTON 4
#define CANCEL_BUTTON 3
#define MASTER_ENROLL_BUTTON 1
#define DELETE_BUTTON 13  
#define RELAY_PIN 15

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

HardwareSerial mySerial(1);
Adafruit_Fingerprint finger(&mySerial);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool enrollmentMode = false;
bool masterEnrollMode = false;
int nextID = 3; // IDs 0,1,2 are reserved for master
const int masterIDs[] = {0, 1, 2};
const int NUM_MASTERS = sizeof(masterIDs) / sizeof(masterIDs[0]);

void setup() {
  pinMode(ENROLL_BUTTON, INPUT_PULLUP);
  pinMode(CANCEL_BUTTON, INPUT_PULLUP);
  pinMode(MASTER_ENROLL_BUTTON, INPUT_PULLUP);
  pinMode(DELETE_BUTTON, INPUT_PULLUP);  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  Serial.begin(115200);
  delay(1000);
  mySerial.begin(57600, SERIAL_8N1, 16, 17);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED init fail");
    while (1);
  }

  showMessage("Initializing...");
  delay(1000);

  finger.begin(57600);
  if (!finger.verifyPassword()) {
    showMessage("Sensor Fail");
    while (1);
  }

  showMessage("Ready\nWaiting...");
}

void loop() {
  if (digitalRead(ENROLL_BUTTON) == LOW) {
    delay(50);
    while (digitalRead(ENROLL_BUTTON) == LOW);
    showMessage("Scan Master");
    if (verifyMaster()) {
      enrollmentMode = true;
      showMessage("Enroll Mode\nAccess Granted");
      delay(1000);
    } else {
      showMessage("Access Denied");
      delay(1000);
    }
  }

  if (digitalRead(MASTER_ENROLL_BUTTON) == LOW) {
    delay(50);
    while (digitalRead(MASTER_ENROLL_BUTTON) == LOW);
    showMessage("Verify Master");
    if (verifyMaster()) {
      masterEnrollMode = true;
      showMessage("Master Enroll\nAccess Granted");
      delay(500);
    } else {
      showMessage("Access Denied");
      delay(1000);
    }
  }

  if (digitalRead(CANCEL_BUTTON) == LOW) {
    delay(50);
    while (digitalRead(CANCEL_BUTTON) == LOW);
    enrollmentMode = false;
    masterEnrollMode = false;
    showMessage("Verify Mode");
    delay(500);
  }

  if (digitalRead(DELETE_BUTTON) == LOW) {
    delay(50);
    while (digitalRead(DELETE_BUTTON) == LOW);
    showMessage("Verify Master");
    if (verifyMaster()) {
      showMessage("Access Granted\nDelete Mode");
      delay(1000);
      handleDelete();
    } else {
      showMessage("Access Denied");
      delay(1000);
    }
  }

  if (masterEnrollMode) {
    handleMasterEnroll();
  } else if (enrollmentMode) {
    handleMultipleEnroll();
  } else {
    handleVerify();
  }

  delay(100);
}

bool verifyMaster() {
  unsigned long start = millis();
  while (millis() - start < 5000) {
    if (finger.getImage() == FINGERPRINT_OK) {
      if (finger.image2Tz() == FINGERPRINT_OK && finger.fingerSearch() == FINGERPRINT_OK) {
        int id = finger.fingerID;
        for (int i = 0; i < NUM_MASTERS; i++) {
          if (id == masterIDs[i]) return true;
        }
        return false;
      }
    }
  }
  return false;
}

bool isIDOccupied(int id) {
  return (finger.loadModel(id) == FINGERPRINT_OK);
}

void handleMasterEnroll() {
  for (int i = 0; i < NUM_MASTERS; i++) {
    int id = masterIDs[i];

    if (isIDOccupied(id)) {
      showMessage("Master ID " + String(id) + "\nAlready Exists");
      showMessage("ENROLL=Skip\nCAN=Exit");

      while (true) {
        if (digitalRead(ENROLL_BUTTON) == LOW) {
          delay(50);
          while (digitalRead(ENROLL_BUTTON) == LOW);
          break;
        }
        if (digitalRead(CANCEL_BUTTON) == LOW) {
          delay(50);
          while (digitalRead(CANCEL_BUTTON) == LOW);
          masterEnrollMode = false;
          showMessage("Verify Mode");
          delay(500);
          return;
        }
      }
      continue;
    }

    showMessage("Enroll Master\nID: " + String(id));
    int result = enrollFingerprint(id);

    if (result == FINGERPRINT_OK) {
      showMessage("Master ID " + String(id) + "\nEnrolled OK");
    } else if (result == -1) {
      showMessage("Master ID " + String(id) + "\nCancelled");
      break;
    } else {
      showMessage("Master ID " + String(id) + "\nEnroll Failed");
    }

    delay(1000);
    showMessage("CAN=Exit\nNEXT=Cont");

    while (true) {
      if (digitalRead(CANCEL_BUTTON) == LOW) {
        delay(50);
        while (digitalRead(CANCEL_BUTTON) == LOW);
        masterEnrollMode = false;
        showMessage("Verify Mode");
        delay(500);
        return;
      }
      if (digitalRead(ENROLL_BUTTON) == LOW) {
        delay(50);
        while (digitalRead(ENROLL_BUTTON) == LOW);
        break;
      }
    }
  }

  masterEnrollMode = false;
  showMessage("Master Setup\nComplete");
  delay(1000);
}
//Enroll mode
void handleMultipleEnroll() {
  while (enrollmentMode) {
    if (isIDOccupied(nextID)) {
      showMessage("ID " + String(nextID) + " Exists\nENROLL=Next  CAN=Exit");

      while (true) {
        if (digitalRead(ENROLL_BUTTON) == LOW) {
          delay(50);
          while (digitalRead(ENROLL_BUTTON) == LOW);
          nextID++;
          break;
        }
        if (digitalRead(CANCEL_BUTTON) == LOW) {
          delay(50);
          while (digitalRead(CANCEL_BUTTON) == LOW);
          enrollmentMode = false;
          showMessage("Verify Mode");
          delay(500);
          return;
        }
      }
      continue;
    }

    showMessage("Enroll User\nID: " + String(nextID));
    int result = enrollFingerprint(nextID);

    if (result == FINGERPRINT_OK) {
      showMessage("ID " + String(nextID) + "\nEnrolled OK");
      nextID++;
    } else if (result == -1) {
      showMessage("Enroll Cancelled");
      enrollmentMode = false;
      delay(1000);
      return;
    } else {
      showMessage("ID " + String(nextID) + "\nEnroll Failed");
    }

    delay(1000);
    showMessage("ENROLL=Next\nCAN=Exit");

    while (true) {
      if (digitalRead(ENROLL_BUTTON) == LOW) {
        delay(50);
        while (digitalRead(ENROLL_BUTTON) == LOW);
        break;
      }
      if (digitalRead(CANCEL_BUTTON) == LOW) {
        delay(50);
        while (digitalRead(CANCEL_BUTTON) == LOW);
        enrollmentMode = false;
        showMessage("Verify Mode");
        delay(500);
        return;
      }
    }
  }
}
//Verify  mode
void handleVerify() {
  showMessage("Waiting Finger...");
  unsigned long start = millis();
  while (millis() - start < 5000) {
    if (finger.getImage() == FINGERPRINT_OK) {
      if (finger.image2Tz() == FINGERPRINT_OK && finger.fingerSearch() == FINGERPRINT_OK) {
        int id = finger.fingerID;
        showMessage("Access Granted\nID: " + String(id));
        digitalWrite(RELAY_PIN, LOW);
        delay(3000);
        digitalWrite(RELAY_PIN, HIGH);
      } else {
        showMessage("Not Found");
      }
      delay(1000);
      return;
    }
  }
}

int enrollFingerprint(int id) {
  showMessage("Place Finger\nID: " + String(id));
  unsigned long start = millis();
  while (millis() - start < 10000) {
    if (digitalRead(CANCEL_BUTTON) == LOW) return -1;
    if (finger.getImage() == FINGERPRINT_OK) break;
  }

  if (finger.image2Tz(1) != FINGERPRINT_OK) return -2;

  showMessage("Lift Finger");
  delay(1500);
  while (finger.getImage() != FINGERPRINT_NOFINGER);

  showMessage("Place Again\nID: " + String(id));
  start = millis();
  while (millis() - start < 10000) {
    if (digitalRead(CANCEL_BUTTON) == LOW) return -1;
    if (finger.getImage() == FINGERPRINT_OK) break;
  }

  if (finger.image2Tz(2) != FINGERPRINT_OK) return -3;
  if (finger.createModel() != FINGERPRINT_OK) return -4;
  if (finger.storeModel(id) != FINGERPRINT_OK) return -5;

  return FINGERPRINT_OK;
}

void handleDelete() {
  int idToDelete = 0;

  while (true) {
    showMessage("Del ID: " + String(idToDelete) + "\nENR=Next CAN=Exit");

    while (digitalRead(DELETE_BUTTON) == HIGH && digitalRead(CANCEL_BUTTON) == HIGH);

    if (digitalRead(DELETE_BUTTON) == LOW) {
      unsigned long pressStart = millis();
      bool longPress = false;

      while (digitalRead(DELETE_BUTTON) == LOW) {
        if (millis() - pressStart > 1000) {
          longPress = true;
          break;
        }
      }

      if (longPress) {
        // Check if it's a master ID
        bool isMaster = false;
        for (int i = 0; i < NUM_MASTERS; i++) {
          if (idToDelete == masterIDs[i]) {
            isMaster = true;
            break;
          }
        }

        if (isMaster) {
          // Count how many masters are enrolled
          int enrolledMasters = 0;
          for (int i = 0; i < NUM_MASTERS; i++) {
            if (finger.loadModel(masterIDs[i]) == FINGERPRINT_OK) {
              enrolledMasters++;
            }
          }

          // Prevent deleting last master
          if (enrolledMasters <= 1) {
            showMessage("Cannot Delete\nLast Master!");
            delay(1500);
            continue; // skip deletion and continue loop
          }

          // Double confirmation for master deletion
          showMessage("Confirm Delete\nMaster ID?");
          delay(1000);

          while (digitalRead(DELETE_BUTTON) == HIGH && digitalRead(CANCEL_BUTTON) == HIGH);

          if (digitalRead(DELETE_BUTTON) == LOW) {
            pressStart = millis();
            longPress = false;
            while (digitalRead(DELETE_BUTTON) == LOW) {
              if (millis() - pressStart > 1000) {
                longPress = true;
                break;
              }
            }

            if (!longPress) {
              showMessage("Cancelled");
              delay(1000);
              continue;
            }
          } else {
            showMessage("Cancelled");
            delay(1000);
            continue;
          }
        }

        // Perform deletion
        if (finger.deleteModel(idToDelete) == FINGERPRINT_OK) {
          showMessage("Deleted ID: " + String(idToDelete));
        } else {
          showMessage("Del Failed ID: " + String(idToDelete));
        }
        delay(1500);

        // Prompt to delete next or exit
        while (true) {
          showMessage("ENR=Next ID\nCAN=Exit Del");
          while (digitalRead(DELETE_BUTTON) == HIGH && digitalRead(CANCEL_BUTTON) == HIGH);

          if (digitalRead(DELETE_BUTTON) == LOW) {
            while (digitalRead(DELETE_BUTTON) == LOW);
            idToDelete++;
            if (idToDelete >= 128) idToDelete = 0;
            break;
          }

          if (digitalRead(CANCEL_BUTTON) == LOW) {
            while (digitalRead(CANCEL_BUTTON) == LOW);
            showMessage("Exit Delete\nMode");
            delay(1000);
            return;
          }
        }
      } else {
        idToDelete++;
        if (idToDelete >= 128) idToDelete = 0;
      }
    }

    if (digitalRead(CANCEL_BUTTON) == LOW) {
      while (digitalRead(CANCEL_BUTTON) == LOW);
      showMessage("Exit Delete\nMode");
      delay(1000);
      return;
    }
  }
}

void showMessage(String msg) {
  Serial.println(msg);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  int line = 0;
  int start = 0;
  int len = msg.length();
  for (int i = 0; i <= len; i++) {
    if (i == len || msg[i] == '\n') {
      String part = msg.substring(start, i);
      display.setCursor(0, line * 16);
      display.print(part);
      start = i + 1;
      line++;
    }
  }
  display.display();
}
