program RetG1;

procedure SyntaxError( errLine : Cardinal; errColumn : Cardinal; errMessage : String );
begin
  WriteLn( 'Syntax Error[L', errLine, ', C', errColumn, ']: ', errMessage );
  Halt(1);
end;

type
  Source = object
  public
    constructor Init;
    destructor Done;

    function Open( fileName : String ): Boolean;
    procedure Close;

    function ReadChar: Char;

    procedure NextToken;

  protected
    handle : File of Char;
    isOpen : Boolean;

  public
    line : Cardinal;
    column : Cardinal;

    nextLine : Cardinal;
    nextColumn : Cardinal;

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

procedure Source.NextToken;
var
  commentLevel : Cardinal;
  doneSkip : Boolean;
  startLine : Cardinal;
  startColumn : Cardinal;

begin
  if not isOpen
    then Exit;

  repeat
    doneSkip := True; { Assume that loop is complete }

    { Skip space and EOL characters }
    if ch in [#9, ' ', #13, #10] then
    begin
      while ch in [#9, ' ', #13, #10]
        do ReadChar;

      doneSkip := False; { Run loop again }
    end;

    { Skip single line comments }
    if (ch = '/') and (nextch = '/') then
    begin
      ReadChar; { Skip / }
      ReadChar; { Skip / }

      while not (ch in [#0, #13, #10]) do
      begin
        ReadChar; { Skip everything }
      end;

      ReadChar; { Skip #13 or #10 }

      doneSkip := False; { Run loop again }
    end;

    { Skip multi-line, nested, comments }
    if (ch = '/') and (nextch = '*') then
    begin
      startLine := line;
      startColumn := column;

      ReadChar; { Skip / }
      ReadChar; { Skip * }

      commentLevel := 1;

      while commentLevel <> 0 do
      begin
        if (ch = '/') and (nextch = '*') then
        begin
          if commentLevel = Cardinal(-1)
            then SyntaxError( startLine, startColumn, 'Maximum comment depth reached.' );
          Inc( commentLevel );

          ReadChar; { Skip / }
          ReadChar; { Skip * }
          continue;
        end;

        if (ch = '*') and (nextch = '/') then
        begin
          if commentLevel = 0
            then SyntaxError( line, column, 'Close comment without matching open comment' );
          Dec( commentLevel );

          ReadChar; { Skip * }
          ReadChar; { Skip / }
          continue;
        end;

        ReadChar;
      end;
    end;
  until doneSkip;
WriteLn( line, ' ', column, ' ', Ord(ch), ': ', ch, ' ', Ord(nextch), ': ', nextch );
end;

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

  retSource.NextToken;
WriteLn( Ord(retSource.ch) );

  retSource.Close;
  retSource.Done;
end.
