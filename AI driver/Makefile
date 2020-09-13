#
# ENSICAEN
# 6 Boulevard Maréchal Juin
# F-14050 Caen Cedex
#
# This file is owned by ENSICAEN students.
# No portion of this document may be reproduced, copied
# or revised without written permission of the authors.
#
# @author Barret Gaetan <gaetan.barret@ecole.ensicaen.fr>
# @version     0.0.1 - 2019-12-02
#
# @todo the list of improvements dor the file.
# @bug the list of known bugs.
#

CC = gcc
CPPFLAGS = -Wall -Wextra
SRC_DIR = ./src/
INC_DIR = ./headers/
BIN_DIR =./bin/
CFLAGS = -c -g -I $(INC_DIR)
LDFLAGS = -lm -o
EXEC = RandomPlayers

all : $(EXEC)

%.o : %.c %.h
	${CC} $(CFLAGS) -o $@ $< $(CPPFLAGS)

RandomPlayers : $(SRC_DIR)vector.o $(SRC_DIR)driver.o
	 $(CC) $^ $(LDFLAGS) $(BIN_DIR)$@

clean:
	rm $(SRC_DIR)*.o $(BIN_DIR)RandomPlayers
