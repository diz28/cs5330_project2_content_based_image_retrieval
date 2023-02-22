
# Di Zhang
# Feb 10, 2023
# CS5330 - Computer Vision

CC = g++
PROJECT = csv_build
SRC = ../util/csv_util.cpp ../util/histogram_util.cpp build.cpp csv_build.cpp
LIBS = `pkg-config --cflags --libs opencv4`
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
