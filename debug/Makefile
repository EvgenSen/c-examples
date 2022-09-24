.PHONY: all build clean

CFLAGS += -Wall -Wextra -Werror -g3 -rdynamic

all: build

build: build_with_diff_opt
	@gcc $(CFLAGS) backtrace.c -pthread -o backtrace
	@gcc $(CFLAGS) array_sort.c -o array_sort

build_with_diff_opt:
	@gcc array_sort.c -o array_sort_no_flags
	@cp array_sort_no_flags array_sort_no_flags_strip && strip array_sort_no_flags_strip
	@gcc -O0 array_sort.c -o array_sort_O0
	@cp array_sort_O0 array_sort_O0_strip && strip array_sort_O0_strip
	@gcc -O1 array_sort.c -o array_sort_O1
	@cp array_sort_O1 array_sort_O1_strip && strip array_sort_O1_strip
	@gcc -O2 array_sort.c -o array_sort_O2
	@cp array_sort_O2 array_sort_O2_strip && strip array_sort_O2_strip
	@gcc -O3 array_sort.c -o array_sort_O3
	@cp array_sort_O3 array_sort_O3_strip && strip array_sort_O3_strip
	@gcc -Os array_sort.c -o array_sort_Os
	@cp array_sort_Os array_sort_Os_strip && strip array_sort_Os_strip
	@gcc -Ofast array_sort.c -o array_sort_Ofast
	@cp array_sort_Ofast array_sort_Ofast_strip && strip array_sort_Ofast_strip
	@gcc -g1 array_sort.c -o array_sort_g1
	@cp array_sort_g1 array_sort_g1_strip && strip array_sort_g1_strip
	@gcc -g2 array_sort.c -o array_sort_g2
	@cp array_sort_g2 array_sort_g2_strip && strip array_sort_g2_strip
	@gcc -g3 array_sort.c -o array_sort_g3
	@cp array_sort_g3 array_sort_g3_strip && strip array_sort_g3_strip

clean:
	@rm backtrace
	@rm array_sort
	@rm array_sort_*
