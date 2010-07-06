MYSRC=main.c config.c vars.c mail.c network.c utils.c web.c compat.c blog.c tidy.c mysql.c
MYHDR=blog.h
PROTOFILE=proto.h

MYOBJS=main.o config.o vars.o mail.o network.o utils.o web.o compat.o blog.o tidy.o mysql.o
SUPPOBJS=mongoose.o 

OBJS=${MYOBJS} ${SUPPOBJS}

PROG=blog

SYSTEM=`uname -s`

DEBUGFLAGS=-g #-DDEBUG
INCLUDEDIRS= -I/usr/local/include -I/usr/local/include/tidy
MYLDFLAGS=-lpthread -ltidy -L/usr/local/lib
MYSQLCFLAGS=`mysql_config --cflags`
MYSQLLDFLAGS=`mysql_config --libs_r`
MYSQLINC=`mysql_config --include`
JSONCFLAGS=`pkg-config json --cflags`
JSONLDFLAGS=`pkg-config json --libs`
OSDEFS= -D${SYSTEM}

CFLAGS=-O2 ${DEBUGFLAGS} $(MYSQLCFLAGS) ${INCLUDEDIRS} ${JSONCFLAGS} ${OSDEFS} -std=c99 
LDFLAGS=${MYSQLLDFLAGS} ${JSONLDFLAGS} ${MYLDFLAGS}

all: proto $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(PROG) $(OBJS) *~ *.BAK *.pid *.log ${PROTOFILE}

proto:  
	-cproto $(INCLUDEDIRS) $(MYSQLINC) ${OSDEFS} -o ${PROTOFILE} -de ${MYSRC} 

indent: clean
	for i in ${MYSRC} ${MYHDR}; do echo Fixing $$i; indent $$i; done
