
var y;

procedure square;
var x;
begin
  x := 10000000;
  while  y<x  do 
  begin
    y := y + 1;
  end
end;
 
begin
  y := 1;
  call square;
  ! y;
end.
