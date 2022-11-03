# Default option
release: clean
    # Make the build dir and cd into it, then execute cmake
	mkdir -p ./build/release; cd ./build/release; cmake ../..; make

# Sekected with first arg as "debug"
debug: clean
    # Make the build dir and cd into it, then execute cmake
	mkdir -p ./build/debug; cd ./build/debug; cmake ../.. -DCMAKE_BUILD_TYPE=Debug; make

# Fist clean up after the last build, arguably non-optimal, though it's not that important and required by my lazy CMakeLists.txt
clean:
	if [ -d "./build" ]; then rm -r ./build; fi