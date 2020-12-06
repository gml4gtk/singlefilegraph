// when using swig generating java and make swigjava
// Compile all the Java files and run:
// $ javac *.java
// $ java runsfg
// see the makefile and swig docs

public class runme {
  static {
    System.loadLibrary("sfg");
  }

  public static void main(String argv[]) {
    System.out.println(sfg.version());
  }
}

