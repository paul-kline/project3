CPP = g++
INC = -I../cryphutil -I../fontutil -I../glslutil -I../mvcutil -I ../imageutil
C_FLAGS = -fPIC -g -c -DGL_GLEXT_PROTOTYPES $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = -L../lib -lcryph -lfont -lglsl -lmvc -limage
OGL_LIBRARIES = -lglfw -lGLU -lGL

OBJS = project4.o Simpleshape.o getMatrices.o ModelViewWithLighting.o MyController.o Column.o Block.o Stairs.o HalfColumn.o FancyColumn.o SuperFancyColumn.o FirePit.o GrassBlade.o
all:
	$(MAKE) -C ../mvcutil
	$(MAKE) project4

project4: $(OBJS) ../lib/libcryph.so ../lib/libfont.so ../lib/libglsl.so ../lib/libmvc.so
	$(LINK) -o project4 $(OBJS) $(LOCAL_UTIL_LIBRARIES) $(OGL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++
	(cd ../cryphutil; make)

../lib/libfont.so: ../fontutil/CFont.h ../fontutil/CFont.c++ ../fontutil/CGLString.h ../fontutil/CGLString.c++
	(cd ../fontutil; make)

../lib/libglsl.so: ../glslutil/ShaderIF.h ../glslutil/ShaderIF.c++
	(cd ../glslutil; make)

../lib/libmvc.so: ../mvcutil/Controller.h ../mvcutil/Controller.c++ ../mvcutil/ModelView.h ../mvcutil/ModelView.c++
	(cd ../mvcutil; make)
	
../lib/libimage.so: ../imageutil/ImageReader.h ../imageutil/ImageReader.c++ ../imageutil/BMPLoader.h ../imageutil/BMPLoader.c++
	(cd ../imageutil; make)	

project4.o: project4.c++
	$(CPP) $(C_FLAGS) project4.c++
getMatrices.o: getMatrices.c++
	$(CPP) $(C_FLAGS) getMatrices.c++
ModelViewWithLighting.o: ModelViewWithLighting.h ModelViewWithLighting.c++
	$(CPP) $(C_FLAGS) ModelViewWithLighting.c++
MyController.o: MyController.h MyController.c++
	$(CPP) $(C_FLAGS) MyController.c++
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
GrassBlade.o: GrassBlade.h GrassBlade.c++
	$(CPP) $(C_FLAGS) GrassBlade.c++	
	
clean:
	rm -f ../mvcutil/ModelView.o ../mvcutil/ModelViewWithLighting.o ./*.o