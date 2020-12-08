#!/bin/sh -x
javac sfg.java
javac main.java
jar cfm sfg.jar Manifest.txt *.class
java -jar sfg.jar

