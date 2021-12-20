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

CXXFLAGS+=-ffreestanding
LDFLAGS+=-T arch/$(ARCH)/linker.ld

CPPFLAGS+=-I arch/$(ARCH)/include

# Architecture-specific properties
include arch/$(ARCH)/Make.properties

# Find the steps
include arch/$(ARCH)/Make.steps

OUTPUT:=arch/$(ARCH)/build/mykonos

$(OUTPUT): $(STEPS)
	mkdir -p $(shell dirname $(OUTPUT))
	echo "$(LD) $@"
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.S
	echo "$(AS) $@"
	$(AS) $(ASFLAGS) $(CPPFLAGS) -c $^ -o $@

%.o: %.cpp
	echo "$(CXX) $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -f $(STEPS) $(OUTPUT)
