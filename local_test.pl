var x, y;

procedure foo( a, b )
var g;
begin
  g := 5 ;
  x := a + b / g;
end;

begin 
  y := 3;
  foo(10,y);
  x := 65 * y + x;
  ! x;
end.
