var x, data[10];

procedure foo (  )
begin
end;

procedure lex( b, c )
begin
  x := b + 61 + c;
  data[3] := x;
end;


begin 
  lex(50,50);
  x := data[3];
  ! x;
end.
