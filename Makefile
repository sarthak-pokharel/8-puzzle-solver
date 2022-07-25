

build:
	rm build/* -rdf
	gcc ./src/*.c -I src/headers/ -o build/Program -lm