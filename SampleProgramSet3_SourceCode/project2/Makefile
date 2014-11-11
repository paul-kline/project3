CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = -L../lib -lcryph -lfont -lglsl -lmvc
OGL_LIBRARIES = -lglfw -lGLU -lGL

OBJS = project2.o Simpleshape.o getMatrices.o ModelViewWithLighting.o Column.o Block.o Stairs.o HalfColumn.o FancyColumn.o SuperFancyColumn.o FirePit.o
all:
	$(MAKE) -C ../mvcutil
	$(MAKE) project2

project2: $(OBJS) ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so
	$(LINK) -o project2 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)

project2.o: project2.c++
	$(CPP) $(C_FLAGS) project2.c++
getMatrices.o: getMatrices.c++
	$(CPP) $(C_FLAGS) getMatrices.c++
ModelViewWithLighting.o: ModelViewWithLighting.h ModelViewWithLighting.c++
	$(CPP) $(C_FLAGS) ModelViewWithLighting.c++
Column.o: Column.h Column.c++
	$(CPP) $(C_FLAGS) Column.c++
HalfColumn.o: HalfColumn.h HalfColumn.c++
	$(CPP) $(C_FLAGS) HalfColumn.c++
FancyColumn.o: FancyColumn.h FancyColumn.c++
	$(CPP) $(C_FLAGS) FancyColumn.c++
Block.o: Block.h Block.c++
	$(CPP) $(C_FLAGS) Block.c++
Stairs.o: Stairs.h Stairs.c++
	$(CPP) $(C_FLAGS) Stairs.c++
Simpleshape.o: Simpleshape.h Simpleshape.c++
	$(CPP) $(C_FLAGS) Simpleshape.c++
SuperFancyColumn.o: SuperFancyColumn.h SuperFancyColumn.c++
	$(CPP) $(C_FLAGS) SuperFancyColumn.c++	
FirePit.o: FirePit.h FirePit.c++
	$(CPP) $(C_FLAGS) FirePit.c++	
	
clean:
	rm -f ../mvcutil/ModelView.o ../mvcutil/ModelViewWithLighting.o ./*.o