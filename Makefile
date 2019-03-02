OBJS 	= main.o ReadParameters.o ReadFiles.o bitcoinsHT.o SRHashT.o walletHT.o list.o structs.o transactions.o tree.o
SOURCE	= main.c Frontend/ReadParameters.c Frontend/ReadFiles.c DataStructures/HashTable/bitcoinsHT.c DataStructures/HashTable/SRHashT.c DataStructures/HashTable/walletHT.c DataStructures/List/list.c DataStructures/Structs/structs.c DataStructures/Structs/transactions.c DataStructures/Tree/tree.c
HEADER  = HeaderFiles/Input.h HeaderFiles/Structs.h HeaderFiles/HashTables.h HeaderFiles/LinkedLists.h HeaderFiles/Tree.h
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

ReadFiles.o: Frontend/ReadFiles.c
	$(CC) $(FLAGS) Frontend/ReadFiles.c

bitcoinsHT.o: DataStructures/HashTable/bitcoinsHT.c
	$(CC) $(FLAGS) DataStructures/HashTable/bitcoinsHT.c

SRHashT.o: DataStructures/HashTable/SRHashT.c
	$(CC) $(FLAGS) DataStructures/HashTable/SRHashT.c

walletHT.o: DataStructures/HashTable/walletHT.c
	$(CC) $(FLAGS) DataStructures/HashTable/walletHT.c

list.o: DataStructures/List/list.c
	$(CC) $(FLAGS) DataStructures/List/list.c

structs.o: DataStructures/Structs/structs.c
	$(CC) $(FLAGS) DataStructures/Structs/structs.c

transactions.o: DataStructures/Structs/transactions.c
	$(CC) $(FLAGS) DataStructures/Structs/transactions.c

tree.o: DataStructures/Tree/tree.c
	$(CC) $(FLAGS) DataStructures/Tree/tree.c
	
# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)