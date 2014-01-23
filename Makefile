
test: test.out
	@./$<

test.out: test.c amp.c
	$(CC) $^ -o $@

clean:
	rm -f test.out

.PHONY: clean