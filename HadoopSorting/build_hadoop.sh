#!/bin/bash

rm *.class *.jar

javac -classpath $(hadoop classpath) HadoopSort.java

if [ $? -ne 0 ]
then
	exit
fi
jar cvf HadoopSort.jar *.class
