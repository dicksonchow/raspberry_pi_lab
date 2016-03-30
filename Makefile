ifneq ($V,1)
Q ?= @
endif

CC	= gcc
PP	= g++
WILDFLAGS	= -L/usr/local/bin
WILDLIBS	= -lwiringPi -lwiringPiDev
CVLDLIBS	= -I/usr/include/opencv -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lpthread -lm -ldl

SRC	= lab_2/get_temperature.c
OBJ	= $(SRC:.c=.o)
BINS	= $(SRC:.c=)

lab1: lab_1/face_counter.o
	$Q $(PP) -o $@ lab_1/face_counter.o $(CVLDLIBS)
	rm -f lab_1/face_counter.o

lab2: lab_2/get_temperature.o
	$Q $(CC) -o $@ lab_2/get_temperature.o $(WILDFLAGS) $(WILDLIBS)
	rm -f lab_2/get_temperature.o

lab4: lab_4/smart_fan.o
	$Q $(CC) -o $@ lab_4/smart_fan.o $(WILDFLAGS) $(WILDLIBS)
	rm -f lab_4/smart_fan.o
