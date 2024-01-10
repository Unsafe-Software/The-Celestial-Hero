run:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Debug ..;\
		make && ./The-Celestial-Hero

deploy:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Release ..;\
		make && ./The-Celestial-Hero
