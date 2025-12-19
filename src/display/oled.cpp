#include "config/config.h"

void updateOLED()
{
    display.clearDisplay();
    display.setCursor(0, 0);
    display.printf("TEMP %.1fC\n", temperature);
    display.printf("SET  %.0fC\n", setpoint);
    display.printf("PID  %.0f%%\n", pidOutput);
    display.printf("MODE %s\n",
                   state == IDLE ? "IDLE" : state == PREHEAT ? "PRE"
                                        : state == SOAK      ? "SOAK"
                                        : state == REFLOW    ? "REF"
                                        : state == COOLDOWN  ? "COOL"
                                        : state == DONE      ? "DONE"
                                                             : "ERR");
    display.display();
}