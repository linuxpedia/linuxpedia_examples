#
#  This program explains POSIX Message Queue APIs
#
#  http://www.linuxpedia.org
#
#  Copyright (C) 2014  LinuxPedia.org (anoojgopi@linuxpedia.org)
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

obj-m := my_kernel_threads.o

KERNEL_RELEASE=`uname --kernel-release`
KERNELDIR = /usr/src/linux-headers-${KERNEL_RELEASE}

all:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) clean
