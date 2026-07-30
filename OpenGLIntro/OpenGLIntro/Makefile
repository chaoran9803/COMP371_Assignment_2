CXX      := g++
CXXFLAGS := $(shell pkg-config --cflags glew glfw3)
LDLIBS   := $(shell pkg-config --libs glew glfw3) -lGL
TARGET   := a2
SRC      := OpenGLIntro.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean
