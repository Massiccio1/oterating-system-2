
# name of binary:
BIN=coda

# source to compile:
SRC=main.c


all: %.c
	@echo $(FILE) 
	@[ ! -e $(FILE) ] && echo "start" > $(FILE) || echo esiste
	g++ $(FILE) -o SignalProxy
	

%.c:
	@echo $(FILE) in .c
	@[ ! -e $(FILE) ] && echo "start" > $(FILE) || echo esiste
# - print informations
# - check for target folder and compile inside, rising error otherwise
# - possibly move to destination (note: if in current folder rises an error, but it's ignored) then dump content of folder

clean:
	@[ ! -e $(FILE) ] && rm $(FILE)  || echo non esiste
	@[ ! -e SignalProxy ] && rm SignalProxy || echo non esiste

