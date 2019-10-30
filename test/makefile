CC = gcc
CFLAGS = -std=c99
TARGET = a.out

$(TARGET): sample.cpp
	$(CC) $(CFLAGS) $^ -o $(@)

test: $(TARGET)
	@./a.out
	@if cmp ${expect} result.bmp > /dev/null; then \
		echo "test: OK"; \
	else \
		echo "test: NG"; \
		exit 1; \
	fi

clean:
	rm -f $(TARGET)
