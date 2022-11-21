/usr/local/bin/glslc shader.vert -o vert.spv
/usr/local/bin/glslc shader.frag -o frag.spv

clang++ -g -std=c++17 -O2 *.cpp -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lSDL2main -lSDL2