var x;

procedure lex( char a, char b )
var k;
begin
  x := a + b;
end;


begin
  lex(50,49);
  !x ;
  lex(2,49);
  !x;
end.
