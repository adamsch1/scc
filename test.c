

dude(  )  { 
  printf("B: %d\n", -1);
}

bob( int a )  { 
  printf("B: %d\n", a);
}

carl( int a, int b  )  { 
  printf("B: %d\n", a + b);
}

dan( char a ) {
  printf("B: %c\n", a );
}

eddie( char a, int b ) {
  printf("B: %c %d\n", a, b );
}

main() {

  int a;

  dude();
  bob(5);
  carl(6,7);
  dan('B');
  eddie('C', 69);
}
