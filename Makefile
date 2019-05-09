.PHONY: clean

examples: 
	mkdir examples
	wget "http://sferics.idsia.ch/Files/ttcomp2002/IC_Problem/competition.zip"
	unzip competition.zip -d examples
	rm competition.zip
	wget "http://sferics.idsia.ch/Files/ttcomp2002/IC_Problem/competition2.zip"
	unzip competition2.zip -d examples
	rm competition2.zip

clean:
	rm -r examples