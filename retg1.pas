program RetG1;

type
  Source = object
  public
    constructor Init;
    destructor Done;

    function Open( fileName : String ): Boolean;
    procedure Close;

    function ReadChar: Char;

  protected
    handle : File of Char;
    isOpen : Boolean;

  public
    line : Integer;
    column : Integer;

    nextLine : Integer;
    nextColumn : Integer;

    ch : Char;
    nextch : Char;
  end;

constructor Source.Init;
begin
  isOpen := False;

  line := 1;
  column := 1;

  nextLine := 1;
  nextColumn := 1;

  ch := #0;
  nextch := #0;
end;

destructor Source.Done;
begin
  isOpen := False;

  line := 1;
  column := 1;

  nextLine := 1;
  nextColumn := 1;

  ch := #0;
  nextch := #0;
end;

{$PUSH I}{$I-}
function Source.Open( fileName : String ): Boolean;
begin
  Open := False;

  if isOpen then
  begin
    Source.Close;
    isOpen := False;
  end;

  if length(fileName) > 0 then
  begin
    Assign( handle, fileName );
    Reset( handle );

    if IOResult = 0 then
    begin
      isOpen := True;

      ReadChar;

      line := 1;
      column := 1;
      nextLine := 1;
      nextColumn := 1;

      ReadChar;
    end;
  end;

  Open := isOpen;
end;
{$POP I}

{$PUSH I}{$I-}
function Source.ReadChar: Char;
begin
  line := nextLine;
  column := nextColumn;

  ch := nextch;
  nextch := #0;

  if isOpen then
  begin
    Read( handle, nextch );
    if IOResult <> 0
      then nextch := #0;

    if ch = #13 then
    begin
      ch := #10;

      if nextch = #10 then
      begin
        Read( handle, nextch );
        if IOResult <> 0
          then nextch := #0;
      end;
    end;

    if ch = #10 then
    begin
      Inc( nextLine );
      nextColumn := 0;
    end;

    Inc( nextColumn );
  end;

  ReadChar := ch;
end;
{$POP I}

procedure Source.Close;
begin
  if isOpen then
  begin
    System.Close( handle );
    isOpen := False;
  end;

  line := 1;
  column := 1;

  nextLine := 1;
  nextColumn := 1;

  ch := #0;
  nextch := #0;
end;

var
  retSource : Source;

begin
  retSource.Init;

  if not retSource.Open('test.ret') then
  begin
    WriteLn( 'Error opening "test.ret".' );
    Halt(1);
  end;

  while retSource.ch <> #0 do
  begin
    WriteLn( '(L', retSource.line, ',C', retSource.column, '): ', Ord(retSource.ch) );
    if retSource.ch <> #0
      then retSource.ReadChar;
  end;
  WriteLn;

  retSource.Close;
  retSource.Done;
end.
