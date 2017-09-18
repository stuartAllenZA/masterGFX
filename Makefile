TARGET = game

I = -I include

CFLAGS = g++ -std=c++11

LFLAGS = -lGL -lGLU -lGLEW -L/usr/include/GL -lglfw -L/usr/include/GLFW -lX11 -lXxf86vm -lXrandr -lpthread -lXi

SP = ./src/

SRC = main.cpp\
	  $(SP)GraphicsEngine.cpp\
      $(SP)Model.cpp\
      $(SP)Shader.cpp\
      $(SP)util.cpp\
	  $(SP)stb_image.cpp\
	  $(SP)window.cpp\
	  $(SP)material.cpp\
	  $(SP)animation.cpp\
	  $(SP)input.cpp\
	  $(SP)camera.cpp

all : $(TARGET)

$(TARGET):
	@$(CFLAGS) $(I) $(SRC) -o $(TARGET) $(LFLAGS)

clean:
	@rm -rf $(TARGET)

re: clean all

