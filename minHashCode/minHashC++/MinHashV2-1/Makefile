CC = g++
CFLAGS =
EXEC_NAME_CREATE = createDatabase
EXEC_NAME_TEST = testDatabase
INCLUDES =
LIBS =
OBJ_FILES_CREATE = createDatabase.o auxiliary.o shingles.o minHash.o
OBJ_FILES_TEST = testDatabase.o auxiliary.o shingles.o minHash.o distribution.o

all : $(EXEC_NAME_CREATE) $(EXEC_NAME_TEST)

clean :
	rm $(EXEC_NAME_CREATE) $(EXEC_NAME_TEST) $(OBJ_FILES_TEST) createDatabase.o

$(EXEC_NAME_CREATE) : $(OBJ_FILES_CREATE)
	$(CC) -o $(EXEC_NAME_CREATE) $(OBJ_FILES_CREATE)

$(EXEC_NAME_TEST) : $(OBJ_FILES_TEST)
	$(CC) -o $(EXEC_NAME_TEST) $(OBJ_FILES_TEST)

%.o: %.cpp
	$(CC) -o $@ -c $<
