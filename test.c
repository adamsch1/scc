

empty() {
}

dude( n ) int n; { 
  printf("B: %d\n", -1);
}
dude2( n,m ) int n;int m; { 
  printf("B: %d %d\n", n, m);
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

  empty();
  dude(5);
  dude2(5,6);
  bob(5);
  carl(6,7);
  dan('B');
  eddie('C', 69);
}
