#Copyright (C) 2021  Jett Thompson
#
    #This program is free software: you can redistribute it and/or modify
    #it under the terms of the GNU General Public License as published by
    #the Free Software Foundation, either version 3 of the License, or
    #(at your option) any later version.
#
    #This program is distributed in the hope that it will be useful,
    #but WITHOUT ANY WARRANTY; without even the implied warranty of
    #MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    #GNU General Public License for more details.
#
    #You should have received a copy of the GNU General Public License
    #along with this program.  If not, see <https://www.gnu.org/licenses/>.

CXXFLAGS+=-ffreestanding -fno-rtti -fno-exceptions -fno-asynchronous-unwind-tables -fno-use-cxa-atexit
LDFLAGS+=-Wl,--nmagic -T arch/$(ARCH)/linker.ld

CPPFLAGS+=-I arch/$(ARCH)/include -I include

# Architecture-specific properties
include arch/$(ARCH)/Make.properties

# Find the steps
include arch/$(ARCH)/Make.steps
include test/Make.steps
include memory/Make.steps
include display/Make.steps
include error/Make.steps
include acpi/Make.steps
include drivers/Make.steps
include scheduler/Make.steps
include util/Make.steps
include fs/Make.steps

# Enable debug mode if requested.
ifeq ($(DEBUG),ON)
 CPPFLAGS+=-DDEBUG
 STEPS+=$(TEST_STEPS)
endif

OUTPUT:=arch/$(ARCH)/build/mykonos

$(OUTPUT): $(STEPS)
	mkdir -p $(shell dirname $(OUTPUT))
	echo "$(LD) $@"
	$(LD) $(LDFLAGS) $(CXXFLAGS) $^ -o $@ -lgcc

%.o: %.S
	echo "$(AS) $@"
	$(AS) $(ASFLAGS) $(CPPFLAGS) -c $^ -o $@

%.o: %.cpp
	echo "$(CXX) $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -f $(STEPS) $(OUTPUT)
