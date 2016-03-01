ifneq ($V,1)
Q ?= @
endif

CC	= gcc
LDFLAGS	= -L/usr/local/bin
LDLIBS	= -lwiringPi -lwiringPiDev

SRC	= lab_2/get_temperature.c
OBJ	= $(SRC:.c=.o)
BINS	= $(SRC:.c=)

lab2: lab_2/get_temperature.o
	$Q $(CC) -o $@ lab_2/get_temperature.o $(LDFLAGS) $(LDLIBS)
	rm -f lab_2/get_temperature.o
