var x, arr[10];

procedure foo ( b, c )
begin
  arr[3] := b + 61 + c;
end;

begin 
  foo(50,50);
  x := arr[3];
  ! x;
end.
