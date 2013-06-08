SUBDIRS = src test

all clean: 
	make $(SUBDIRS) TARGET=$@

$(SUBDIRS): 
	cd $@; make $(TARGET)

.PHONY: $(SUBDIRS)
