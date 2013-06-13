SUBDIRS = hiredis

all clean: 
	make $(SUBDIRS) TARGET=$@

$(SUBDIRS): 
	cd $@; make $(TARGET)

.PHONY: $(SUBDIRS)
