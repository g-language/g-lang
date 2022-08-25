CFLAGS     = -Ofast -I include -Wno-unused-result
LDFLAGS    =

PRNT_GRAY := \e[0;90m
PRNT_REDD := \e[0;91m
PRNT_GREN := \e[0;92m
PRNT_YELW := \e[0;93m
PRNT_BLUE := \e[0;94m
PRNT_PRPL := \e[0;95m
PRNT_CYAN := \e[0;96m
PRNT_RSET := \e[m

SRC_C     := $(shell find src/* -type f -name '*.c')
SRC_O     := $(foreach f, $(SRC_C:.c=.o), bin/$f)

SRC_CH    := $(shell find include/* -type f -name '*.h') $(SRC_C)
FMT_D     := $(foreach f, $(SRC_CH:%=%.fmt), bin/$f)

$(shell mkdir -p bin/ $(foreach d, \
	$(dir $(FMT_D)) \
	, $d))

.PHONY = default

default: gct
format: $(FMT_D)

clean:
	rm -rf bin
	rm -f gct

bin/%.fmt: %
	@uncrustify -c uncrustify.cfg --replace --no-backup $<
	@echo 'new' > $@

-include $(SRC_O:.o=.d)

bin/%.o: %.c
	@echo "$(PRNT_RSET)[$(PRNT_PRPL) $(notdir $<) $(PRNT_RSET)]"
	@gcc -c -o $@ $< $(CFLAGS)
	@echo -n $(dir $@) > $(@:.o=.d)
	@gcc -MM $(CFLAGS) $(LDFLAGS) $< >> $(@:.o=.d)

gct: $(SRC_O)
	@echo "$(PRNT_RSET)[$(PRNT_YELW) $(notdir $@) $(PRNT_RSET)]"
	@gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)