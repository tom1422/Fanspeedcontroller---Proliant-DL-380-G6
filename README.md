# Arduino Fan Speed Controller Proliant DL-380-G6
Circuit board + arduino sketch for controlling fan speed on HP proliant DL380 server (Probably works on a bunch of other models as well)

Circuit board is single sided so it should be easy to create your own at home using basic etching and drilling. 

Arduino Nano interfaces over USB serial to computer to get new fan speed from python servce (i'm using it with proxmox).

If no input is recieved fan speed defaults to 100%.

I recommend using alarm cable (8 core) or cat5 cable (alarm cable is easier to work with) for wiring up to the fans.

Each fan is 4 pin and 2 are 12v power while 1 is PWM (25khz) and the other is tacho. See image for wiring. 
