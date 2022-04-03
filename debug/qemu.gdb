file build/mykonos.debug
target remote | qemu-system-x86_64 -machine q35 -cpu qemu64,rdtscp,monitor -S -gdb stdio -cdrom build/mykonos.iso
