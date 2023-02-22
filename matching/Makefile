

# Di Zhang
# Feb 10, 2023
# CS5330 - Computer Vision

CC = g++
PROJECT = matchimages
SRC = ../util/csv_util.cpp ../util/histogram_util.cpp match_functions.cpp matchimages.cpp
LIBS = `pkg-config --cflags --libs opencv4`
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)
