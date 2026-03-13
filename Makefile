dependency:
	cd build && cmake .. --graphviz=graph.dot && dot -Tpng graph.dot -o graphimg.png

prepare:
	rm -rf build
	mkdir build
	cd build