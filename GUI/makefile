CC = clang++

ARGS = -std=c++11 -o GUITEST -lSDL2 -lSDL2_image
ARGS2 = -std=c++11 -o GUITEST_2 -lSDL2 -lSDL2_image
ARGS3 = -std=c++11 -o GUI_TEST_THREE -lSDL2 -lSDL2_image

TEST_FILES = testing3.cpp font.cpp guipane.cpp button.cpp graph.cpp select_widget.cpp
TEST_FILES2 =  testing2.cpp font.cpp guipane.cpp button.cpp graph.cpp ../source/diversify.cpp ../source/private/storedata.cpp ../source/private/ymath.cpp ../source/private/dataframe.cpp ../source/private/date.cpp ../source/private/frontieralgo.cpp ../source/private/risktopointalgo.cpp 
test: $(TEST_FILES2)
	$(CC) $(TEST_FILES2) $(ARGS)

test2: $(TEST_FILES2)
	$(CC) $(TEST_FILES2) $(ARGS2)

test3: $(TEST_FILES)
	$(CC) $(TEST_FILES) $(ARGS3)
