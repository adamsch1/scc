var x, arr[10];

procedure foo ( z[] )
var y;
begin
  z[0] := 5;
end;

begin 
  foo(arr);
  x := arr[0];
  ! x;
end.
