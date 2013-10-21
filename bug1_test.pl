var x, char ARR[10], char FAR[10], char BAR[10];

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

procedure strcmp( char a[], char b[] ) 
var v,k;
begin
  v := 0;
  k := -1;

  while v = 0 do
  begin 
    k := k + 1;
    v := a[k] - b[k];
  end  
  v := v;
end;

begin 
  FAR := "YYY";
  BAR := "YYY";
  x := strlen( FAR);
  !x ;
  x := strcmp( FAR,BAR);
  !x ;
  x := strlen( FAR);
  !x ;

end.
