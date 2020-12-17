# RPi kernel module

***A kernel module for toggling an io on a Raspberry Pi.***

---


## TODO

- [X] toggle 2 ios
- [X] count edges
- [ ] use IOCTL's

---


## Usage

Wire up leds and button.

Commands for RPi.

```bash
# Install RPi kernel headers & clone repo.
sudo apt-get install raspberrypi-kernel-headers
git clone https://github.com/bandydos/kernelmod-rpi-c.git

# Navigate to leds_edges_kmod folder.
cd kernelmod-rpi-c
cd leds_edges_kmod

# Make & remove existing module if needed.
make
sudo rmmod clargmod.ko

# Insmod with args (choose toggleSpeed, ioPins... (for x)).
sudo insmod clargmod.ko toggleSpeed=x ioPins=x,x 

# Check kernel messages.
dmesg
```
