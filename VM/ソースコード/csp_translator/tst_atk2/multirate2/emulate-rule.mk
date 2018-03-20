check: $(TRACEDUMP)
	$(ATK2_TRACECHECK) -b $(BASE_RATE) $<

forcecheck: clean_check
	$(MAKE) MAKEFLAGS="$(MAKEFLAGS)" check

clean_check:
	rm -f $(TRACEDUMP)

run: $(TRACEDUMP)

$(TRACEDUMP): $(ATK2_PROGRAM)
	./$(ATK2_PROGRAM) -s $(RUN_TIMES) > $@
