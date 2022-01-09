t: c.c
	gcc -o $@ -g -DSHA_DEBUG -mavx2 $<

ses: ses.c
	gcc -o $@ -g $<
	@./ses