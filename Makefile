# includes and flags
CFLAGS = -O3 -ffast-math -Wall -Wextra
LIBS = -lXft -lX11 -lXcursor -lXft -lfontconfig -lXcomposite
FREETYPEINC = /usr/include/freetype2
INCS = -I${FREETYPEINC}

all: copile install

copile:
	cc -g -o ./bin/tupi ./src/tupi.c ./src/util.c ./src/twindow.c ${LIBS} ${INCS}

install:
	cp -f ./bin/tupi /usr/bin
	cp -f wmtupi.desktop /usr/share/applications
	chmod 755 /usr/bin/tupi

.PHONY: all copile install
