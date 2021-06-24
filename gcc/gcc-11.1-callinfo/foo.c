#include <stdio.h>

void hello_GCC (void)
{
 fgetc(stdin);
}

void zoo (void)
{
 fputc('c',stdout);
 hello_GCC();
}

void bar (void)
{
zoo();
}

void foo (void)
{
bar();
 fputc('c',stdout);
}

int main (int argc, char *argv[])
{

 printf("ll");
 foo();
 bar();
 return 0;
}
