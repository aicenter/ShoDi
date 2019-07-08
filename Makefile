PROGRAM=chTest
CC=g++
CFLAGS=-std=c++11 -Wall -pedantic -g

all: compile

compile: $(PROGRAM)

$(PROGRAM): objs/IntegerCorectnessValidator.o objs/IntegerDijkstraBenchmark.o objs/IntegerCHBenchmark.o objs/BasicIntegerDijkstra.o objs/IntegerDijkstraNode.o objs/DDSGLoader.o objs/IntegerFlagsGraph.o objs/IntegerFlagsGraphWithUnpackingData.o objs/IntegerGraph.o objs/DIMACSLoader.o objs/IntegerOutputEdge.o objs/OutputShortcutEdge.o objs/IntegerPreprocessingEdgeData.o objs/QueryEdge.o objs/ShortcutEdge.o objs/SimpleGraph.o objs/IntegerUpdateableGraph.o objs/EdgeDifferenceManager.o objs/IntegerCHDistanceQueryManager.o objs/IntegerCHPathQueryManager.o objs/CHPreprocessor.o objs/Timer.o objs/IntegerHopsDijkstraNode.o objs/CHNode.o objs/CHpriorityQueue.o objs/IntegerNodeData.o objs/main.o
	$(CC) $(CFLAGS) $^ -o $@

objs/IntegerCorectnessValidator.o: src/Benchmarking/IntegerCorectnessValidator.cpp \
 src/Benchmarking/IntegerCorectnessValidator.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerDijkstraBenchmark.o: src/Benchmarking/IntegerDijkstraBenchmark.cpp \
 src/Benchmarking/../Dijkstra/BasicIntegerDijkstra.h \
 src/Benchmarking/../Dijkstra/../GraphBuilder/IntegerGraph.h \
 src/Benchmarking/../Dijkstra/../GraphBuilder/SimpleGraph.h \
 src/Benchmarking/../Timer/Timer.h src/Benchmarking/IntegerDijkstraBenchmark.h \
 src/Benchmarking/../GraphBuilder/IntegerGraph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerCHBenchmark.o: src/Benchmarking/IntegerCHBenchmark.cpp \
 src/Benchmarking/IntegerCHBenchmark.h \
 src/Benchmarking/../GraphBuilder/IntegerFlagsGraph.h \
 src/Benchmarking/../GraphBuilder/QueryEdge.h \
 src/Benchmarking/../GraphBuilder/../CH/Structures/IntegerNodeData.h \
 src/Benchmarking/../GraphBuilder/IntegerGraph.h \
 src/Benchmarking/../GraphBuilder/SimpleGraph.h \
 src/Benchmarking/../CH/IntegerCHDistanceQueryManager.h \
 src/Benchmarking/../CH/../GraphBuilder/IntegerFlagsGraph.h \
 src/Benchmarking/../Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/BasicIntegerDijkstra.o: src/Dijkstra/BasicIntegerDijkstra.cpp \
 src/Dijkstra/BasicIntegerDijkstra.h src/Dijkstra/../GraphBuilder/IntegerGraph.h \
 src/Dijkstra/../GraphBuilder/SimpleGraph.h src/Dijkstra/IntegerDijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerDijkstraNode.o: src/Dijkstra/IntegerDijkstraNode.cpp src/Dijkstra/IntegerDijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/DDSGLoader.o: src/GraphBuilder/DDSGLoader.cpp \
 src/GraphBuilder/DDSGLoader.h src/GraphBuilder/IntegerGraph.h \
 src/GraphBuilder/SimpleGraph.h src/GraphBuilder/IntegerFlagsGraph.h \
 src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/IntegerNodeData.h \
 src/GraphBuilder/IntegerFlagsGraphWithUnpackingData.h \
 src/GraphBuilder/../Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerFlagsGraph.o: src/GraphBuilder/IntegerFlagsGraph.cpp \
 src/GraphBuilder/IntegerFlagsGraph.h src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/IntegerNodeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerFlagsGraphWithUnpackingData.o: \
 src/GraphBuilder/IntegerFlagsGraphWithUnpackingData.cpp \
 src/GraphBuilder/IntegerFlagsGraphWithUnpackingData.h \
 src/GraphBuilder/IntegerFlagsGraph.h src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/IntegerNodeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerGraph.o: src/GraphBuilder/IntegerGraph.cpp src/GraphBuilder/IntegerGraph.h \
 src/GraphBuilder/SimpleGraph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/DIMACSLoader.o: src/GraphBuilder/DIMACSLoader.cpp src/GraphBuilder/DIMACSLoader.h \
 src/GraphBuilder/IntegerGraph.h src/GraphBuilder/SimpleGraph.h \
 src/GraphBuilder/IntegerUpdateableGraph.h \
 src/GraphBuilder/IntegerPreprocessingEdgeData.h src/GraphBuilder/IntegerOutputEdge.h \
 src/GraphBuilder/OutputShortcutEdge.h src/GraphBuilder/../Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerOutputEdge.o: src/GraphBuilder/IntegerOutputEdge.cpp \
 src/GraphBuilder/IntegerOutputEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/OutputShortcutEdge.o: src/GraphBuilder/OutputShortcutEdge.cpp \
 src/GraphBuilder/OutputShortcutEdge.h src/GraphBuilder/IntegerOutputEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerPreprocessingEdgeData.o: src/GraphBuilder/IntegerPreprocessingEdgeData.cpp \
 src/GraphBuilder/IntegerPreprocessingEdgeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/QueryEdge.o: src/GraphBuilder/QueryEdge.cpp src/GraphBuilder/QueryEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/ShortcutEdge.o: src/GraphBuilder/ShortcutEdge.cpp \
 src/GraphBuilder/ShortcutEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/SimpleGraph.o: src/GraphBuilder/SimpleGraph.cpp \
 src/GraphBuilder/SimpleGraph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerUpdateableGraph.o: src/GraphBuilder/IntegerUpdateableGraph.cpp \
 src/GraphBuilder/IntegerUpdateableGraph.h \
 src/GraphBuilder/IntegerPreprocessingEdgeData.h src/GraphBuilder/IntegerOutputEdge.h \
 src/GraphBuilder/OutputShortcutEdge.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/EdgeDifferenceManager.o: src/CH/EdgeDifferenceManager.cpp \
 src/CH/EdgeDifferenceManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerCHDistanceQueryManager.o: src/CH/IntegerCHDistanceQueryManager.cpp \
 src/CH/IntegerCHDistanceQueryManager.h src/CH/../GraphBuilder/IntegerFlagsGraph.h \
 src/CH/../GraphBuilder/QueryEdge.h \
 src/CH/../GraphBuilder/../CH/Structures/IntegerNodeData.h \
 src/CH/../Dijkstra/IntegerDijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerCHPathQueryManager.o: src/CH/IntegerCHPathQueryManager.cpp \
 src/CH/IntegerCHPathQueryManager.h \
 src/CH/../GraphBuilder/IntegerFlagsGraphWithUnpackingData.h \
 src/CH/../GraphBuilder/IntegerFlagsGraph.h src/CH/../GraphBuilder/QueryEdge.h \
 src/CH/../GraphBuilder/../CH/Structures/IntegerNodeData.h \
 src/CH/../Dijkstra/IntegerDijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHPreprocessor.o: src/CH/CHPreprocessor.cpp src/CH/../Timer/Timer.h \
 src/CH/../Dijkstra/IntegerDijkstraNode.h src/CH/Structures/IntegerHopsDijkstraNode.h \
 src/CH/Structures/../../Dijkstra/IntegerDijkstraNode.h src/CH/CHPreprocessor.h \
 src/CH/../GraphBuilder/IntegerUpdateableGraph.h \
 src/CH/../GraphBuilder/IntegerPreprocessingEdgeData.h \
 src/CH/../GraphBuilder/IntegerOutputEdge.h \
 src/CH/../GraphBuilder/OutputShortcutEdge.h \
 src/CH/../GraphBuilder/ShortcutEdge.h src/CH/Structures/CHNode.h \
 src/CH/Structures/CHpriorityQueue.h src/CH/Structures/CHNode.h \
 src/CH/EdgeDifferenceManager.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/Timer.o: src/Timer/Timer.cpp src/Timer/Timer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerHopsDijkstraNode.o: src/CH/Structures/IntegerHopsDijkstraNode.cpp \
 src/CH/Structures/IntegerHopsDijkstraNode.h \
 src/CH/Structures/../../Dijkstra/IntegerDijkstraNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHNode.o: src/CH/Structures/CHNode.cpp src/CH/Structures/CHNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CHpriorityQueue.o: src/CH/Structures/CHpriorityQueue.cpp \
 src/CH/Structures/CHpriorityQueue.h src/CH/Structures/CHNode.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/IntegerNodeData.o: src/CH/Structures/IntegerNodeData.cpp src/CH/Structures/IntegerNodeData.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/main.o: src/main.cpp src/GraphBuilder/DDSGLoader.h \
 src/GraphBuilder/IntegerGraph.h src/GraphBuilder/SimpleGraph.h \
 src/GraphBuilder/IntegerFlagsGraph.h src/GraphBuilder/QueryEdge.h \
 src/GraphBuilder/../CH/Structures/IntegerNodeData.h \
 src/GraphBuilder/IntegerFlagsGraphWithUnpackingData.h src/GraphBuilder/DIMACSLoader.h \
 src/GraphBuilder/IntegerUpdateableGraph.h \
 src/GraphBuilder/IntegerPreprocessingEdgeData.h src/GraphBuilder/IntegerOutputEdge.h \
 src/GraphBuilder/OutputShortcutEdge.h src/Dijkstra/BasicIntegerDijkstra.h \
 src/Dijkstra/../GraphBuilder/IntegerGraph.h src/Benchmarking/IntegerCHBenchmark.h \
 src/Benchmarking/../GraphBuilder/IntegerFlagsGraph.h \
 src/Benchmarking/../GraphBuilder/IntegerGraph.h \
 src/Benchmarking/IntegerCorectnessValidator.h \
 src/Benchmarking/IntegerDijkstraBenchmark.h src/Timer/Timer.h \
 src/CH/CHPreprocessor.h src/CH/../GraphBuilder/IntegerUpdateableGraph.h \
 src/CH/../GraphBuilder/ShortcutEdge.h src/CH/Structures/CHNode.h \
 src/CH/Structures/CHpriorityQueue.h src/CH/Structures/CHNode.h \
 src/CH/IntegerCHPathQueryManager.h \
 src/CH/../GraphBuilder/IntegerFlagsGraphWithUnpackingData.h \
 src/CH/IntegerCHDistanceQueryManager.h src/CH/../GraphBuilder/IntegerFlagsGraph.h | objs
	$(CC) $(CFLAGS) -c $< -o $@


objs:
	mkdir objs

clean:
	rm -rf $(PROGRAM) objs/ 2>/dev/null
