.PHONY: build clean

build: clean
	@gcc lock.c -o lock
	@chmod a+x ./runme.sh
	@echo "Имя скрипта: ./lock"

clean:
	@rm -f lock
	@rm -f file
	@rm -rf file.lck
	@rm -f result.txt
