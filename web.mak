WEB_TARGET = bank.js
SOURCES = llist.cpp user_interface.cpp
EMFLAGS = -s WASM=1 -s EXPORTED_FUNCTIONS='["_init_database","_process_command","_get_output"]' \
          -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s FORCE_FILESYSTEM=1 \
          -s MODULARIZE=1 -s SINGLE_FILE=1

all: $(WEB_TARGET)

$(WEB_TARGET): $(SOURCES)
	em++ $(SOURCES) -o $@ $(EMFLAGS)

clean:
	rm -f $(WEB_TARGET) *.wasm
