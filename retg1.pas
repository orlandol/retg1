
program RetG1;

procedure SyntaxError( errLine : Cardinal; errColumn : Cardinal; errMessage : String );
begin
  WriteLn( 'Syntax Error[L', errLine, ', C', errColumn, ']: ', errMessage );
  Halt(1);
end;

type
  SymbolTable = object
  end;

type
  Assembler = object
  { Methods }
  public
    constructor Init;
    destructor Done;

    function Create( fileName : String ): Boolean;
    procedure Close;

  { Member variables }
  protected
    handle : File of Char;
    isOpen : Boolean;
  end;

constructor Assembler.Init;
begin
  isOpen := False;
end;

destructor Assembler.Done;
begin
  IsOpen := False;
end;

function Assembler.Create( fileName : String ): Boolean;
begin
  Create := False;
end;

procedure Assembler.Close;
begin
end;

type
  Parser = object
  { Methods }
  public
    constructor Init;
    destructor Done;

    function Open( fileName : String ): Boolean;
    procedure Close;

  protected
    function ReadChar: Char;

    function ReadIdent( var toIdent : String ): Boolean;

    procedure NextToken;

  public
    procedure Build;

  { Member variables }
  protected
    handle : File of Char;
    isOpen : Boolean;

  protected
    startLine : Cardinal;
    startColumn : Cardinal;

    line : Cardinal;
    column : Cardinal;

    nextLine : Cardinal;
    nextColumn : Cardinal;

    ch : Char;
    nextch : Char;
  end;

constructor Parser.Init;
begin
  isOpen := False;

  startLine := 1;
  startColumn := 1;

  line := 1;
  column := 1;

  nextLine := 1;
  nextColumn := 1;

  ch := #0;
  nextch := #0;
end;

destructor Parser.Done;
begin
  isOpen := False;

  startLine := 1;
  startColumn := 1;

  line := 1;
  column := 1;

  nextLine := 1;
  nextColumn := 1;

  ch := #0;
  nextch := #0;
end;

{$PUSH I}{$I-}
function Parser.Open( fileName : String ): Boolean;
begin
  Open := False;

  if isOpen then
  begin
    Parser.Close;
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

      startLine := line;
      startColumn := column;
    end;
  end;

  Open := isOpen;
end;
{$POP I}

procedure Parser.Close;
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

{$PUSH I}{$I-}
function Parser.ReadChar: Char;
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

function Parser.ReadIdent( var toIdent : String ): Boolean;
begin
  ReadIdent := false;
  toIdent := '';

  case ch of
  '_', 'a'..'z', 'A'..'Z':
    ;
  otherwise
    Exit;
  end;

  while ch <> #0 do
  begin
    if Length(toIdent) = 255
      then Exit;

    case ch of
    '_', 'a'..'z', 'A'..'Z', '0'..'9':
      toIdent := toIdent + ch;
    otherwise
      Exit;
    end;

    ReadChar;
  end;

  ReadIdent := True;
end;

procedure Parser.NextToken;
var
  commentLevel : Cardinal;
  doneSkip : Boolean;

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
            then SyntaxError( line, column, 'Comment closed without a matching comment opening.' );
          Dec( commentLevel );

          ReadChar; { Skip * }
          ReadChar; { Skip / }
          continue;
        end;

        if ch = #0 then
        begin
          if commentLevel <> 0
            then SyntaxError( startLine, startColumn, 'Unexpected EOF in comment.' );
        end;

        ReadChar;
      end;

      doneSkip := False; { Run loop again }
    end;
  until doneSkip;

  startLine := line;
  startColumn := column;
end;

procedure Parser.Build;
var
  identName : String;

begin
  NextToken;
  identName := '';

  if (not ReadIdent(identName)) and (identName <> 'program')
    then SyntaxError( startLine, startColumn, 'Expected keyword program' );
end;

var
  retParser : Parser;

begin
  retParser.Init;

  if not retParser.Open('test.ret') then
  begin
    WriteLn( 'Error opening "test.ret".' );
    Halt(1);
  end;

  retParser.Build;

  retParser.Close;
  retParser.Done;
end.
