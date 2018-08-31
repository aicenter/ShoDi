PROGRAM=chTest
CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -g

all: compile

compile: $(PROGRAM)

$(PROGRAM): objs/CorectnessValidator.o objs/DijkstraBenchmark.o objs/CHBenchmark.o objs/BasicDijkstra.o objs/DijkstraNode.o objs/DDSGLoader.o objs/FlagsGraph.o objs/FlagsGraphWithUnpackingData.o objs/Graph.o objs/Loader.o objs/OutputEdge.o objs/OutputShortcutEdge.o objs/PreprocessingEdgeData.o objs/QueryEdge.o objs/ShortcutEdge.o objs/SimpleGraph.o objs/UpdateableGraph.o objs/EdgeDifferenceManager.o objs/CHDistanceQueryManager.o objs/CHPathQueryManager.o objs/CHPreprocessor.o objs/Timer.o objs/HopsDijkstraNode.o objs/CHNode.o objs/CHpriorityQueue.o objs/NodeData.o objs/main.o 
	$(CC) $(CFLAGS) $^ -o $@

objs/CorectnessValidator.o: src/Benchmarking/CorectnessValidator.cpp \
 src/Benchmarking/CorectnessValidator.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/DijkstraBenchmark.o: src/Benchmarking/DijkstraBenchmark.cpp \
 src/Benchmarking/../Dijkstra/BasicDijkstra.h \
 src/Benchmarking/../Dijkstra/../GraphBuilder/Graph.h \
 src/Benchmarking/../Dijkstra/../GraphBuilder/SimpleGraph.h \
 src/Benchmarking/../Timer/Timer.h src/Benchmarking/DijkstraBenchmark.h \
 src/Benchmarking/../GraphBuilder/Graph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHBenchmark.o: src/Benchmarking/CHBenchmark.cpp \
 src/Benchmarking/CHBenchmark.h \
 src/Benchmarking/../GraphBuilder/FlagsGraph.h \
 src/Benchmarking/../GraphBuilder/QueryEdge.h \
 src/Benchmarking/../GraphBuilder/../CH/Structures/NodeData.h \
 src/Benchmarking/../GraphBuilder/Graph.h \
 src/Benchmarking/../GraphBuilder/SimpleGraph.h \
 src/Benchmarking/../CH/CHDistanceQueryManager.h \
 src/Benchmarking/../CH/../GraphBuilder/FlagsGraph.h \
 src/Benchmarking/../Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/BasicDijkstra.o: src/Dijkstra/BasicDijkstra.cpp \
 src/Dijkstra/BasicDijkstra.h src/Dijkstra/../GraphBuilder/Graph.h \
 src/Dijkstra/../GraphBuilder/SimpleGraph.h src/Dijkstra/DijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/DijkstraNode.o: src/Dijkstra/DijkstraNode.cpp src/Dijkstra/DijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/DDSGLoader.o: src/GraphBuilder/DDSGLoader.cpp \
 src/GraphBuilder/DDSGLoader.h src/GraphBuilder/Graph.h \
 src/GraphBuilder/SimpleGraph.h src/GraphBuilder/FlagsGraph.h \
 src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/NodeData.h \
 src/GraphBuilder/FlagsGraphWithUnpackingData.h \
 src/GraphBuilder/../Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/FlagsGraph.o: src/GraphBuilder/FlagsGraph.cpp \
 src/GraphBuilder/FlagsGraph.h src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/NodeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/FlagsGraphWithUnpackingData.o: \
 src/GraphBuilder/FlagsGraphWithUnpackingData.cpp \
 src/GraphBuilder/FlagsGraphWithUnpackingData.h \
 src/GraphBuilder/FlagsGraph.h src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/NodeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/Graph.o: src/GraphBuilder/Graph.cpp src/GraphBuilder/Graph.h \
 src/GraphBuilder/SimpleGraph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/Loader.o: src/GraphBuilder/Loader.cpp src/GraphBuilder/Loader.h \
 src/GraphBuilder/Graph.h src/GraphBuilder/SimpleGraph.h \
 src/GraphBuilder/UpdateableGraph.h \
 src/GraphBuilder/PreprocessingEdgeData.h src/GraphBuilder/OutputEdge.h \
 src/GraphBuilder/OutputShortcutEdge.h src/GraphBuilder/../Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/OutputEdge.o: src/GraphBuilder/OutputEdge.cpp \
 src/GraphBuilder/OutputEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/OutputShortcutEdge.o: src/GraphBuilder/OutputShortcutEdge.cpp \
 src/GraphBuilder/OutputShortcutEdge.h src/GraphBuilder/OutputEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/PreprocessingEdgeData.o: src/GraphBuilder/PreprocessingEdgeData.cpp \
 src/GraphBuilder/PreprocessingEdgeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/QueryEdge.o: src/GraphBuilder/QueryEdge.cpp src/GraphBuilder/QueryEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/ShortcutEdge.o: src/GraphBuilder/ShortcutEdge.cpp \
 src/GraphBuilder/ShortcutEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/SimpleGraph.o: src/GraphBuilder/SimpleGraph.cpp \
 src/GraphBuilder/SimpleGraph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/UpdateableGraph.o: src/GraphBuilder/UpdateableGraph.cpp \
 src/GraphBuilder/UpdateableGraph.h \
 src/GraphBuilder/PreprocessingEdgeData.h src/GraphBuilder/OutputEdge.h \
 src/GraphBuilder/OutputShortcutEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/EdgeDifferenceManager.o: src/CH/EdgeDifferenceManager.cpp \
 src/CH/EdgeDifferenceManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHDistanceQueryManager.o: src/CH/CHDistanceQueryManager.cpp \
 src/CH/CHDistanceQueryManager.h src/CH/../GraphBuilder/FlagsGraph.h \
 src/CH/../GraphBuilder/QueryEdge.h \
 src/CH/../GraphBuilder/../CH/Structures/NodeData.h \
 src/CH/../Dijkstra/DijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHPathQueryManager.o: src/CH/CHPathQueryManager.cpp \
 src/CH/CHPathQueryManager.h \
 src/CH/../GraphBuilder/FlagsGraphWithUnpackingData.h \
 src/CH/../GraphBuilder/FlagsGraph.h src/CH/../GraphBuilder/QueryEdge.h \
 src/CH/../GraphBuilder/../CH/Structures/NodeData.h \
 src/CH/../Dijkstra/DijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHPreprocessor.o: src/CH/CHPreprocessor.cpp src/CH/../Timer/Timer.h \
 src/CH/../Dijkstra/DijkstraNode.h src/CH/Structures/HopsDijkstraNode.h \
 src/CH/Structures/../../Dijkstra/DijkstraNode.h src/CH/CHPreprocessor.h \
 src/CH/../GraphBuilder/UpdateableGraph.h \
 src/CH/../GraphBuilder/PreprocessingEdgeData.h \
 src/CH/../GraphBuilder/OutputEdge.h \
 src/CH/../GraphBuilder/OutputShortcutEdge.h \
 src/CH/../GraphBuilder/ShortcutEdge.h src/CH/Structures/CHNode.h \
 src/CH/Structures/CHpriorityQueue.h src/CH/Structures/CHNode.h \
 src/CH/EdgeDifferenceManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/Timer.o: src/Timer/Timer.cpp src/Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/HopsDijkstraNode.o: src/CH/Structures/HopsDijkstraNode.cpp \
 src/CH/Structures/HopsDijkstraNode.h \
 src/CH/Structures/../../Dijkstra/DijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHNode.o: src/CH/Structures/CHNode.cpp src/CH/Structures/CHNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHpriorityQueue.o: src/CH/Structures/CHpriorityQueue.cpp \
 src/CH/Structures/CHpriorityQueue.h src/CH/Structures/CHNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/NodeData.o: src/CH/Structures/NodeData.cpp src/CH/Structures/NodeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/main.o: src/main.cpp src/GraphBuilder/DDSGLoader.h \
 src/GraphBuilder/Graph.h src/GraphBuilder/SimpleGraph.h \
 src/GraphBuilder/FlagsGraph.h src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/NodeData.h \
 src/GraphBuilder/FlagsGraphWithUnpackingData.h src/GraphBuilder/Loader.h \
 src/GraphBuilder/UpdateableGraph.h \
 src/GraphBuilder/PreprocessingEdgeData.h src/GraphBuilder/OutputEdge.h \
 src/GraphBuilder/OutputShortcutEdge.h src/Dijkstra/BasicDijkstra.h \
 src/Dijkstra/../GraphBuilder/Graph.h src/Benchmarking/CHBenchmark.h \
 src/Benchmarking/../GraphBuilder/FlagsGraph.h \
 src/Benchmarking/../GraphBuilder/Graph.h \
 src/Benchmarking/CorectnessValidator.h \
 src/Benchmarking/DijkstraBenchmark.h src/Timer/Timer.h \
 src/CH/CHPreprocessor.h src/CH/../GraphBuilder/UpdateableGraph.h \
 src/CH/../GraphBuilder/ShortcutEdge.h src/CH/Structures/CHNode.h \
 src/CH/Structures/CHpriorityQueue.h src/CH/Structures/CHNode.h \
 src/CH/CHPathQueryManager.h \
 src/CH/../GraphBuilder/FlagsGraphWithUnpackingData.h \
 src/CH/CHDistanceQueryManager.h src/CH/../GraphBuilder/FlagsGraph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@


objs:
	mkdir objs

clean:
	rm -rf $(PROGRAM) objs/ 2>/dev/null
