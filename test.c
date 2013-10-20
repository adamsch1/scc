

struct foo {
  int a;
  int b;
  char *c;
};

empty() {
}

gcd( int x, int y ) {
  if( x == y ) {
    return x;
  }else if( x < y ) {
    return gcd(x,y-x);
  } else {
    return gcd(x-y,y);
  }
}

addglb(sname,id,typ,value)
  char *sname,id,typ;
  int value;
{

  printf("%s %c %c %d\n", sname, id, typ, value ); 
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

char buffer[100];

herbert( a ) char *a; {
  int k;

  k = 0;
  while( a[k] ) { 
    putchar( a[k] );
    k++;
  }
}

main() {
  char b;
  int a;

  empty();

  a = gcd(200,100);
  printf("GCD: %d\n",a);
  addglb("dude",'b', 'c',11);
  dude(5);
  dude2(5,6);
  bob(5);
  carl(6,7);
  dan('B');
  eddie('C', 69);

  herbert( "rob\n" );
}
