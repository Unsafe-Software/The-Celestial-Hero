run:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Debug ..;\
		./confs ../cfg/settings.conf ../src/settings.conf.h && ./confs ../cfg/worldgen.conf ../src/worldgen.conf.h &&\
		make && ./The-Celestial-Hero

deploy:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Release ..;\
		./confs ../cfg/settings.conf ../src/settings.conf.h && ./confs ../cfg/worldgen.conf ../src/worldgen.conf.h &&\
		make && ./The-Celestial-Hero
