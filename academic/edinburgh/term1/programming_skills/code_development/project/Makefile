CC=gcc
CFLAGS=-g
LIBS=-fopenmp -lm
LIBS_TEST=$(LIBS) -lcunit
POPULATION_SRC=src/ranlux.c src/arralloc.c src/population.c src/main.c
POPULATION_O=$(patsubst %.c, %.o, $(POPULATION_SRC))
PNMREADER_SRC=src/pnmreader.c
PNMREADER_O=$(patsubst %.c, %.o, $(PNMREADER_SRC))
TEST_SRC=test/test_main.c
TEST_O=$(filter-out src/main.o, $(POPULATION_O))


.PHONY: population
population: $(POPULATION_O)
	$(CC) $(CFLAGS) -o $@ $(POPULATION_O) $(LIBS)

.PHONY: test
test: clean population
	$(CC) $(CFLAGS_TEST) -o population_test $(TEST_O) $(TEST_SRC) $(LIBS_TEST) && ./population_test

.PHONY: animation
animation: run
	convert -delay 10 -loop 0 PPM* animation.mp4

.PHONY: run
run: clean population
	./population file.dat

.PHONY: pnmreader
pnmreader: $(PNMREADER_O)
	$(CC) $(CFLAGS) -o $@ $(PNMREADER_O) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(LIBS)

.PHONY: clean
clean:
	rm -fr population population_test pnmreader $(POPULATION_O) $(PNMREADER_O) core outputs animation.mp4
