var x, char ARR[10];

procedure strlen( char a[] ) 
var v,k;
begin
  k :=0;
  while a[k] # 0 do 
  begin
    k := k + 1;
    v := a[k];
  end 
  k := k;
end;

begin 
  ARR := "FUCKERS";
  x := strlen(ARR);
  !x ;
end.
