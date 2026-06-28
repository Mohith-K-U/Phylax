# 🔐 PHYLAX

### ESP32-Based Fingerprint Biometric Access Control System



> **PHYLAX** is a biometric access control system developed using the **ESP32** microcontroller and an optical fingerprint sensor. The system provides secure authentication through fingerprint recognition, displays user feedback on an OLED screen, and controls an electronic locking mechanism via a relay.

---

## 📌 Overview

PHYLAX is designed as a smart access control solution that replaces conventional keys with biometric authentication. The project demonstrates the integration of embedded systems, biometric security, and user interface design into a compact and portable device.

The system provides two primary operating modes: **Master Enrollment** for securely registering authorized users and **Fingerprint Verification** for authenticating users during normal operation. This design ensures that only an administrator can enroll new fingerprints, enhancing the overall security of the access control system.

---

## ✨ Features

* 🔒 Fingerprint-based authentication
* 👑 Master Enrollment feature for administrator-controlled user registration
* 👤 Secure enrollment of authorized users
* 🔍 Verify registered fingerprints
* 📟 OLED display for status messages and user guidance
* ⚡ ESP32-based embedded controller
* 🔌 Relay-controlled electronic lock output
* 🎛️ Dedicated buttons for Enrollment and Verification modes
* 💾 Supports storage of 500+ fingerprint templates (sensor dependent)
* 🔋 Compatible with external 12V power supply or battery operation
* 🧱 Compact 3D printed enclosure

---

## ⚙️ System Workflow

### Master Enrollment

1. Administrator enters Master Enrollment mode.
2. New user fingerprint is scanned.
3. The user is prompted to scan the same finger again for verification.
4. The fingerprint template is generated.
5. The template is securely stored in the fingerprint sensor memory.
6. The system confirms successful enrollment.

---

### Verification Mode

1. User places a registered finger on the sensor.
2. The fingerprint is matched against stored templates.
3. If authentication succeeds:
   - User ID is displayed on the OLED.
   - The relay is energized.
   - The electronic lock is unlocked for a predefined duration.
4. If authentication fails:
   - Access is denied.
   - The system returns to standby mode.

---

---

## 🔐 Security Features

- Administrator-controlled Master Enrollment
- Biometric authentication using fingerprint recognition
- Secure template storage within the fingerprint sensor
- Unauthorized users cannot access the locking mechanism
- Dedicated Enrollment and Verification operating modes

---

## 🛠 Hardware Components

| Component                  | Purpose                  |
| -------------------------- | ------------------------ |
| ESP32                      | Main controller          |
| Optical Fingerprint Sensor | Biometric authentication |
| 0.96" OLED Display         | User interface           |
| Relay Module               | Lock control             |
| Push Buttons               | Mode selection           |
| 12V Power Supply           | System power             |
| 3D Printed Enclosure       | Mechanical housing       |

---

## 💻 Software

* Arduino IDE
* ESP32 Arduino Framework
* C++
* Adafruit Fingerprint Library
* Adafruit SSD1306 Library
* Adafruit GFX Library
* Wire Library

---
## 📚 Libraries Used

* Adafruit Fingerprint Sensor Library
* Adafruit SSD1306
* Adafruit GFX
* Wire

---

## 👨‍💻 Author

**Mohith K U**

Email: **[mohithmandanna29@gmail.com](mailto:mohithmandanna29@gmail.com)**

---

## 📄 License

This project is released under the **MIT License**.

---

## ⭐ Acknowledgements

This project was developed as an embedded systems and IoT solution demonstrating secure biometric authentication using the ESP32 platform. It showcases the integration of hardware, firmware, and user interface design into a practical smart-lock application suitable for academic and prototyping purposes.

If you found this project useful, consider giving it a ⭐ on GitHub.

## 📜 Copyright Notice

This repository is shared for portfolio, demonstration, and educational
reference purposes only.

The source code, documentation, and associated project materials are
copyright © 2026 Mohith K U. All rights are reserved.

No permission is granted to copy, modify, redistribute, or use this work,
in whole or in part, without prior written permission from the author.
