run:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Debug ..;\
		make && ./confs ../cfg/settings.conf ../src/settings.conf.h && ./confs ../cfg/worldgen.conf ../src/worldgen.conf.h && ./The-Celestial-Hero

deploy:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Release ..;\
		make && ./confs ../cfg/settings.conf ../src/settings.conf.h && ./confs ../cfg/worldgen.conf ../src/worldgen.conf.h && ./The-Celestial-Hero
