# Default option
release:
    # Make the build dir and cd into it, then execute cmake
	mkdir -p ./build/release; cd ./build/release; cmake ../..; make

# Sekected with first arg as "debug"
debug:
    # Make the build dir and cd into it, then execute cmake
	mkdir -p ./build/debug; cd ./build/debug; cmake ../.. -DCMAKE_BUILD_TYPE=Debug; make

fresh-debug: clean debug

fresh-release: clean release


# Optionally clean up after the last build
clean:
	if [ -d "./build" ]; then rm -r ./build; fi