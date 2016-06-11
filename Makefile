#########define
CC = gcc
CFLAGS = -Wall -g -O2
#CFLAGS = -Wall
INCLUDES = -I/usr/local/include/evhtp
LIBS = -levent -levent_openssl -levent_pthreads -lssl -lcrypto -levhtp -lpthread -lm -llua

all: objs/mnl
.PHONY: all

#########
objs/mnl: objs/mnl.o \
		objs/mnl_cbs_usra.o \
		objs/mnl_cbs_test.o \
		objs/mnl_cbs.o \
		objs/mnl_access.o \
		objs/mnl_net.o \
		objs/mnl_log.o \
		objs/mnl_string.o \
		objs/mnl_utils.o \
		objs/mnl_slock.o \
		objs/cJSON.o

	$(CC) -o objs/mnl \
		objs/mnl.o \
		objs/mnl_cbs_usra.o \
		objs/mnl_cbs_test.o \
		objs/mnl_cbs.o \
		objs/mnl_access.o \
		objs/mnl_net.o \
		objs/mnl_log.o \
		objs/mnl_string.o \
		objs/mnl_utils.o \
		objs/mnl_slock.o \
		objs/cJSON.o \
		$(LIBS)

##########
#  core  #
##########
objs/mnl.o: src/mnl.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl.o \
		src/mnl.c

objs/mnl_cbs_usra.o: src/mnl_cbs_usra.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_cbs_usra.o \
		src/mnl_cbs_usra.c

objs/mnl_cbs_test.o: src/mnl_cbs_test.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_cbs_test.o \
		src/mnl_cbs_test.c

objs/mnl_cbs.o: src/mnl_cbs.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_cbs.o \
		src/mnl_cbs.c

objs/mnl_access.o: src/mnl_access.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_access.o \
		src/mnl_access.c

objs/mnl_net.o: src/mnl_net.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_net.o \
		src/mnl_net.c

objs/mnl_log.o: src/mnl_log.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_log.o \
		src/mnl_log.c

objs/mnl_string.o: src/mnl_string.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_string.o \
		src/mnl_string.c

objs/mnl_utils.o: src/mnl_utils.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_utils.o \
		src/mnl_utils.c

objs/mnl_slock.o: src/mnl_slock.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/mnl_slock.o \
		src/mnl_slock.c

objs/cJSON.o: src/cjson/cJSON.c

	$(CC) -c $(CFLAGS) $(INCLUDES) \
		-o objs/cJSON.o \
		src/cjson/cJSON.c

#########
#  clean #
#########
clean:
	rm -f objs/mnl
	rm -f objs/*.o
