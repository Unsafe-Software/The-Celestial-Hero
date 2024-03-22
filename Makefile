run:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Debug ..;\
		make && ./The-Celestial-Hero

deploy:
	@mkdir -p ./build
	@cd ./build;\
		cmake -DCMAKE_BUILD_TYPE=Release ..;\
		make

macos_icon:
	mkdir -p ./data/assets/icons/Icon.iconset
	sips -z 16 16     ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_16x16.png
	sips -z 32 32     ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_16x16@2x.png
	sips -z 32 32     ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_32x32.png
	sips -z 64 64     ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_32x32@2x.png
	sips -z 128 128   ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_128x128.png
	sips -z 256 256   ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_128x128@2x.png
	sips -z 256 256   ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_256x256.png
	sips -z 512 512   ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_256x256@2x.png
	sips -z 512 512   ./data/assets/icons/icon1024.png --out ./data/assets/icons/Icon.iconset/icon_512x512.png
	iconutil -c icns ./data/assets/icons/Icon.iconset
	SetFile -a C ./data/assets/icons/Icon.icns ./build/The-Celestial-Hero
