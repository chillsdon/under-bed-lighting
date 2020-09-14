# under-bed-lighting
Lights activated by motion (PIR) when dark (LDR) - Arduino

If it's dark and there is movement, the LED strip is faded up for 3 minutes and then faded down again. Any movement while the LEDs are on will reset the 3 minute count down.

The LED strip used is 12v so the PWM output (0v to 5v) is pushed through a boost converter, scaling the output to 0v to 12v accordingly.
