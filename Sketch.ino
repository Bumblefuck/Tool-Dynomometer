// Read RPM and calculate average every 'numreadings' readings.
const int numreadings = 10;

// Number of interrupts per rotation.
const int interruptsperrotation = 1;

int readings[numreadings];
unsigned long total = 0; // Rolling total for rolling average

// See http://arduino.cc/en/Reference/Volatile - basically, variables used in interrupts should be volatile
volatile unsigned int interruptcount = 0;

// This code will be executed every time the interrupt 0 (pin2) gets low.
void update_count(){
    ++interruptcount;
}

void setup(){
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(2), update_count, FALLING);

    // Fill in an initial readings array before we start averaging it.
    for (unsigned int i = 0; i < numreadings; ++i){
        detachInterrupt(0); // Disable interrupt when calculating

        // Convert interrupt frecuency to RPM
        readings[i] = interruptcount * (60 / interruptsperrotation);
        total += readings[i];
        interruptcount = 0; // Reset interrupt count to get a new reading

        attachInterrupt(digitalPinToInterrupt(2), update_count, FALLING); // Re-enable interrupt
        delay(1000); // Read values for another second, so the reading frecuency is 1 Hz.
    }
}

void loop(){
    for (unsigned int i = 0; i < numreadings; ++i){
        detachInterrupt(0); // Disable interrupt when calculating

        // A rolling average is accomplished by removing the oldest value (the current index, 'i')
        // from the total, updating the value, and adding in the new value.
        total -= readings[i];
        readings[i] = interruptcount * (60 / interruptsperrotation); // Convert interrupt frecuency to RPM
        total += readings[i];
        interruptcount = 0; // Reset interrupt count to get a new reading

        Serial.print(" RPM = ");
        Serial.println(total / numreadings); // Print the average RPM

        attachInterrupt(digitalPinToInterrupt(2), update_count, FALLING); // Re-enable interrupt
        delay(1000); // Read values for another second, so the reading frecuency is 1 Hz.
    }
}
