# mykonos
Mykonos is a toy operating system.
## Tool Chain
If you do not do this, you will probably get an error like "Checking for x86_64-mykonos-g++: Not found".
- gcc: [mykonos-gcc](https://github.com/Jett59/mykonos-gcc)
- binutils: [mykonos-binutils](https://github.com/Jett59/mykonos-binutils)
- [grub](https://ftp.gnu.org/gnu/grub/)
## Building
Once you have installed the tool chain, building is simple:
```bash
make isoimage
```
Or:
```bash
make isoimage TEST=ON
```
To enable runtime tests.
## Running
### In an emulator
Run a command like this:
```bash
qemu-system-x86_64 -cpu qemu64,rdtscp -machine q35 -cdrom build/mykonos.iso
```
### On physical hardware
Burn the live ISO image to a USB drive and boot from it on the machine.
