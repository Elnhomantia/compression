
OBJS=bit.o bitstream.o bits.o entier.o sf.o matrice.o dct.o psycho.o rle.o image.o jpg.o ondelette.o
UTILITAIRES=eprintf.o intstream.o filtres.o
CFLAGS=-Wall -g -O3


OBJSTST=$(OBJS:.o=_tst.o)
OBJSH=$(OBJS:.o=.h)

run:tests
	@if [ ! -h dct ] ; then echo "Vous avez mal récupéré les fichiers du TP, 'dct' devrait être un lien symbolique sur 'tests'" ; fi
	@if [ ! -d DONNEES ] ; then echo "Vous avez mal récupéré les fichiers du TP, il devrait y avoir un répertoire DONNEES" ; fi
	@if [ $$(id -u) = 0 ] ; then echo "ON NE TRAVAILLE JAMAIS SOUS root ! Déconnectez-vous rapidement avant que l'enseignant ne vous agresse." ; exit 1 ; fi
	@if [ $$(cat $(OBJS:.o=.c) | grep -c 'exit *( *0 *)' ) != 0 ] ; then echo "Votre code ne doit pas contenir exit(0) car cela indique qu'il n'y a pas d'erreur." ; exit 1 ; fi
	./tests

tests:tests.o $(OBJS) $(OBJSTST) $(UTILITAIRES)
	$(CC) $(CFLAGS) tests.o $(UTILITAIRES) $(OBJS) $(OBJSTST) -lm -o $@

tests.o:tests.c tests.h tests_proto.h tests_table.h

tests_proto.h tests_table.h Makefile.table:Makefile tests_genere $(OBJSH)
	./tests_genere $(OBJS)

clean:
	-rm *~ *.o xxx* tests

TAGS:tests
	-etags *.[ch]

PUBLIC=$$HOME/public_html/COURS/COURS/TRANS_COMP_IMAGE/TP/TP_DCT
copie:TAGS clean
	rm -rf /tmp/TP_DCT
	cp -a . /tmp/TP_DCT
	rm -rf /tmp/TP_DCT/.git
	for I in $(OBJS:.o=.c) ;\
	do \
	awk '/^}/{ IN=0 ; if ( R == 1 ) print "return 0 ; /* pour enlever un warning du compilateur */" ; R=0 ; } IN==0{ print $0 ; print >>"xxx.reste" ;} IN==1 && /^[ \t]*return.*[a-zA-Z0-9].*;/ { R=1;} IN==1{ print ""; print >>"xxx" ; } /^\{/{ IN=1 ; }' <$$I >/tmp/TP_DCT/$$I ; \
	done
	@echo "`wc -l <xxx` lignes de programme à faire"
	@echo "`wc -l <xxx.reste` lignes de programme fournies"
	@echo "`cat *_tst.c | wc -l` ligne de tests"
	@cp --update -r /tmp/TP_DCT/. $(PUBLIC)
	@chmod -R a+rX $(PUBLIC)
	@cd /tmp ; tar -cvf - TP_DCT | gzip -9 >$(PUBLIC)/tp_dct.tar.gz

include Makefile.table

