run:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Debug ..;\
		make
	@./build/The-Celestial-Hero

deploy:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Release ..;\
		make
	@./build/The-Celestial-Hero
