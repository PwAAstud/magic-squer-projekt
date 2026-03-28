src    = mainTread.c \
generatory/brutforce/force.c \
generatory/permutation/lexicalPerm.c \
generatory/smartPermu/smartPermu.c
obj = $(addprefix .build/,$(notdir $(src:.c=.o)))
target = a.out

vpath %.c $(dir $(src))

$(target): $(obj) $(src)
	gcc -o $@ $(obj)

.build:
	mkdir .build

# $$(filter %$$*.c,$(src))
.build/%.o: %.c | .build
	gcc -c $< -o $@

clear:
	rm -rd .build
	rm -f a.out
