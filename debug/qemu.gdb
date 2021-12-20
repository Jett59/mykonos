file build/mykonos
target remote | qemu-system-x86_64 -S -gdb stdio -cdrom build/mykonos.iso
