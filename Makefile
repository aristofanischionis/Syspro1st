OBJS 	= main.o ReadParameters.o
SOURCE	= main.c Frontend/ReadParameters.c
HEADER  = HeaderFiles/Input.h
OUT  	= bitcoin
CC	= gcc
FLAGS   = -g -c -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

ReadParameters.o: Frontend/ReadParameters.c
	$(CC) $(FLAGS) Frontend/ReadParameters.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)