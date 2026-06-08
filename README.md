# siwx917-hcsr04-ultrasonic
HC-SR04 ultrasonic distance measurement on SiWx917 Dev Kit (BRD2605A) using a custom GPIO driver and sleep timer via Simplicity Studio 5. Arduino generates the trigger pulse; SiWx917 reads the echo and outputs distance over serial using sl_si91x_driver_gpio and sl_sleeptimer SDK components.
