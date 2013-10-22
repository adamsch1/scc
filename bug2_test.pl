var x,y, ARR[10], BAR[10], CAR[10];

procedure strcmp( char a[], char b[] ) 
var k,j,v;
begin
  k:=0;

  v := a[k] - b[k];
  while (v = 0) & a[k] do
  begin 
    k := k + 1; 
    v := a[k] - b[k];
  end

  x := v;

end;

begin 

  BAR := "abc";
  CAR := "abc";

  x := strcmp( BAR, CAR ); 
  !x;
end.
