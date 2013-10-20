
const ident =1, 
      number=2, 
      lparen=3,
      rparen=4,
      times =5,
      slash =6,
      plus =7,
      minus =8,
      eql =9,
      neq =10,
      leq =11,
      gtr =12,
      geq =13,
      beginsym =14,
      semicolon =15,
      endsym =16,
      ifsym =17,
      whilesym =18,
      becomes =19,
      thensym =20,
      dosym =21,
      constsym =22,
      comma =23,
      varsym =24,
      procsym =25,
      period =26,
      oddsym =27,
      colon =28,
      bang =29,
      ob =30,
      cb=31,
      charsym=32,
      in=1,
      arr=3,
      chsym=4;

var char ch;

procedure lex( k ) 
var k;
begin
  while k > 1  do begin
    if ch # 13 then begin
      if ch # 8 then begin
        k := 1;
      end
    end
    k := getchar(0); 
  end
end;


begin
  lex(44);
  call lex;
  ! x;
  ! z;
end.
