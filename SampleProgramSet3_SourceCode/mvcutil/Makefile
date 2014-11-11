CPP = g++
C_FLAGS = -fPIC -O -c -DGL_GLEXT_PROTOTYPES -I../cryphutil -I../glslutil

LINK = g++ -fPIC

OBJS = Controller.o ModelView.o GLFWController.o

libmvc.so: $(OBJS)
	$(LINK) -shared -o libmvc.so $(OBJS) -L../lib -lcryph -lglsl
	cp libmvc.so ../lib/

Controller.o: Controller.h Controller.c++
	$(CPP) $(C_FLAGS) Controller.c++

GLFWController.o: GLFWController.h GLFWController.c++
	$(CPP) $(C_FLAGS) GLFWController.c++

ModelView.o: ModelView.h ModelView.c++
	$(CPP) $(C_FLAGS) ModelView.c++
