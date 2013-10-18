
int y;

square() {
  int x;
  x = 10000000;
  while( y < x ) {
    y = y + 1;
  }
}

main() {
  y = 1;
  square();
  printf("%d\n",y);
}
