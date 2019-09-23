makesrc:
	cd src && $(MAKE)

tests: makesrc
	cd test && $(MAKE)

.PHONY: cleansrc cleantests clean
cleansrc:
	cd src && $(MAKE) clean

cleantests:
	cd test && $(MAKE) clean

clean: cleansrc cleantests
	rm -rf *.o
